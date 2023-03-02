// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_ContractAbiEncoding.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "UObject/Package.h"
// =============================================================================
#include "Crypto/Hash/TSBC_HashFunctionLibrary.h"
#include "Encoding/TSBC_ContractAbiHelper.h"
#include "Math/TSBC_BaseConverter.h"
#include "Module/TSBC_RuntimeLogCategories.h"
#include "Util/TSBC_StringUtils.h"

void CTSBC_ContractAbiEncoding::EncodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    const TArray<FTSBC_SolidityValueList>& Arguments,
    FString& FunctionSelectorAndEncodedArguments)
{
    bSuccess = false;
    ErrorMessage = "";
    // check if there are functions saved in the Contract ABI Context struct.
    if(ContractAbi.ContractAbiFunctions.Num() <= 0)
    {
        ErrorMessage = FString("ABI Functions List is empty, make sure to generate the ABI first");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FTSBC_ContractAbiFunction FunctionRef;
    if(!CTSBC_ContractAbiHelper::IsFunctionAvailable(ContractAbi, FunctionName, FunctionRef))
    {
        ErrorMessage = FString("Function not found");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FString InputsRef;
    // Get the input types for the function to use in making the function selector (Hash).
    for(int32 i = 0; i < FunctionRef.Inputs.Num(); i++)
    {
        // If the type is tuple we need to add its internal types to the function selector as one object.
        if(FunctionRef.Inputs[i].Variable.Type == AbiType_Tuple)
        {
            InputsRef.Append("(");
            for(int32 j = 0; j < FunctionRef.Inputs[i].TupleVariables.Num(); j++)
            {
                if(j + 1 < FunctionRef.Inputs[i].TupleVariables.Num())
                {
                    InputsRef.Append(FunctionRef.Inputs[i].TupleVariables[j].Type + ",");
                    continue;
                }
                InputsRef.Append(FunctionRef.Inputs[i].TupleVariables[j].Type);
            }
            InputsRef.Append(")");
            if(i + 1 < FunctionRef.Inputs.Num())
            {
                InputsRef.Append(",");
            }
            continue;
        }

        if(i + 1 < FunctionRef.Inputs.Num())
        {
            InputsRef.Append(FunctionRef.Inputs[i].Variable.Type + ",");
            continue;
        }

        InputsRef.Append(FunctionRef.Inputs[i].Variable.Type);
    }

    const FString Signature = FString::Printf(
        TEXT("%s(%s)"),
        *FunctionRef.Name,
        *InputsRef);

    // Create the function Hash
    const FString FunctionHash = UTSBC_HashFunctionLibrary::Keccak256FromString(Signature, false);
    // Function selector is the first 4 bytes of the function hash
    FString FunctionSelector = FunctionHash.Mid(0, 8);
    FunctionSelectorAndEncodedArguments = FString("0x").Append(FunctionSelector);

    TArray<ETSBC_SolidityDataType> SolidityDataTypes;
    bool bContainsTuple;
    CTSBC_ContractAbiHelper::GetSolidityDataTypes(
        FunctionRef.Inputs,
        SolidityDataTypes,
        bContainsTuple);

    if(bContainsTuple && SolidityDataTypes.Num() > 1)
    {
        ErrorMessage = FString::Printf(
            TEXT("Encoding function \"%s\" is not possible due to Blueprints limitations."),
            *FunctionRef.Name);
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FString EncodedArguments = "";
    uint32 DataSegmentsUsed = 0;

    // make sure that params matches arguments. also make sure that it's not tuple, tuples are checked later.
    if(FunctionRef.Inputs.Num() != Arguments.Num() && SolidityDataTypes[0] != ETSBC_SolidityDataType::Tuple)
    {
        ErrorMessage = FString::Printf(
            TEXT("Error passing arguments, Expecting %d but found %d"),
            FunctionRef.Inputs.Num(),
            Arguments.Num());

        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    // Encode the arguments passed relative to their types. Then add them to the function selector string.
    FTSBC_ContractAbiEncodeContext EncodeContext;
    EncodeContext.FunctionParameters = FunctionRef.Inputs;
    EncodeContext.ArgumentsTypes = SolidityDataTypes;
    EncodeContext.Arguments = Arguments;
    EncodeContext.bEncodingTuple = false;
    EncodeContext.TupleParameterIndex = 0;
    if(!EncodeArguments(
        EncodeContext,
        DataSegmentsUsed,
        EncodedArguments))
    {
        ErrorMessage = "Error encoding arguments";
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FunctionSelectorAndEncodedArguments.Append(EncodedArguments);

    bSuccess = true;
}

void CTSBC_ContractAbiEncoding::EncodeDynamicDataLocation(
    const int& ParametersCount,
    const int& CurrentPointer,
    FString& EncodedArguments)
{
    const int32 DataPadding = (ParametersCount + CurrentPointer) * AbiSegmentBytesLength;
    FString DecHexValue = "";
    CTSBC_BaseConverter::Dec2HexConverter().Convert(
        FString::FromInt(DataPadding),
        DecHexValue,
        1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
}

void CTSBC_ContractAbiEncoding::GetDynamicArrayLength(
    const FTSBC_SolidityValueList& Arguments,
    FString& EncodedArguments)
{
    FString DecHexValue = "";
    CTSBC_BaseConverter::Dec2HexConverter().Convert(FString::FromInt(Arguments.Values.Num()), DecHexValue, 1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
}


bool CTSBC_ContractAbiEncoding::EncodeArguments(
    FTSBC_ContractAbiEncodeContext& EncodeContext,
    uint32& DataSegmentsUsed,
    FString& EncodedArguments)
{
    uint32 HeaderSectionsCount;
    int32 ArgumentTypesCount;
    if(EncodeContext.bEncodingTuple)
    {
        ArgumentTypesCount = EncodeContext.TupleArgumentsTypes.Num();
        HeaderSectionsCount = EncodeContext.FunctionParameters[EncodeContext.TupleParameterIndex].TupleVariables.Num();

        // make sure that tuple params matches tuple arguments
        if(HeaderSectionsCount != EncodeContext.Arguments.Num())
        {
            TSBC_LOG(
                Error,
                TEXT("Error passing tuple arguments, Expecting %d but found %d"),
                HeaderSectionsCount,
                EncodeContext.Arguments.Num());
            return false;
        }
    }
    else
    {
        ArgumentTypesCount = EncodeContext.ArgumentsTypes.Num();
        HeaderSectionsCount = EncodeContext.FunctionParameters.Num();
    }

    TArray<FTSBC_EncodedArgumentData> EncodedArgumentsData;
    FString HeaderDataSection = "";
    FString BodyDataSection = "";

    for(int32 i = 0; i < ArgumentTypesCount; i++)
    {
        FString L_HeaderEncodedArgument = "";
        FString L_BodyEncodedArgument = "";
        uint32 L_DataSegmentsUsed = 0;
        bool bEncodeInPlace = false;

        EncodeContext.ParameterIndex = i;
        const bool bEncodeSuccess = EncodeArgument(
            EncodeContext,
            bEncodeInPlace,
            L_DataSegmentsUsed,
            L_HeaderEncodedArgument,
            L_BodyEncodedArgument);

        if(!bEncodeSuccess)
        {
            return false;
        }

        EncodedArgumentsData.Add(
            FTSBC_EncodedArgumentData(
                L_DataSegmentsUsed,
                bEncodeInPlace,
                L_HeaderEncodedArgument));

        if(bEncodeInPlace)
        {
            // Remove argument pointer segment and replace it with count of header encoded arguments
            HeaderSectionsCount -= 1;
            HeaderSectionsCount += L_DataSegmentsUsed;
        }

        BodyDataSection.Append(L_BodyEncodedArgument);
        DataSegmentsUsed += L_DataSegmentsUsed;
    }

    uint32 L_Pointer = 0;
    for(const FTSBC_EncodedArgumentData& Element : EncodedArgumentsData)
    {
        if(Element.bEncodedInPlace)
        {
            HeaderDataSection.Append(Element.HeaderData);
        }
        else
        {
            FString DataLocation = "";
            EncodeDynamicDataLocation(HeaderSectionsCount, L_Pointer, DataLocation);
            HeaderDataSection.Append(DataLocation);
            L_Pointer += Element.SegmentsCount;
        }
    }

    EncodedArguments.Append(HeaderDataSection).Append(BodyDataSection);

    return true;
}

bool CTSBC_ContractAbiEncoding::EncodeArgument(
    FTSBC_ContractAbiEncodeContext& EncodeContext,
    bool& bEncodeInPlace,
    uint32& DataSegmentsUsed,
    FString& EncodedHeaderSection,
    FString& EncodedBodySection)
{
    FString FunctionParamType;
    DataSegmentsUsed = 0;

    // store the argument data to encode regarding the argument type.
    TArray<FString> ArgumentData = EncodeContext.Arguments[EncodeContext.ParameterIndex].Values;
    if(ArgumentData.Num() == 0 || ArgumentData.GetData()->IsEmpty())
    {
        return false;
    }

    ETSBC_SolidityDataType DataTypeToEncode;
    if(EncodeContext.bEncodingTuple)
    {
        DataTypeToEncode = EncodeContext.TupleArgumentsTypes[EncodeContext.ParameterIndex];
        FunctionParamType = EncodeContext
            .FunctionParameters[EncodeContext.TupleParameterIndex]
            .TupleVariables[EncodeContext.ParameterIndex]
            .Type;
    }
    else
    {
        DataTypeToEncode = EncodeContext.ArgumentsTypes[EncodeContext.ParameterIndex];
        FunctionParamType = EncodeContext
                            .FunctionParameters[EncodeContext.ParameterIndex]
                            .Variable
                            .Type;
    }

    bEncodeInPlace = true;

    switch(DataTypeToEncode)
    {
    case ETSBC_SolidityDataType::Address:
        {
            // Encode only the first element since it is not an array.
            if(!EncodeAddress(ArgumentData[0], EncodedHeaderSection))
            {
                return false;
            }
            // static element uses 1 segment of data
            DataSegmentsUsed = 1;
            break;
        }
    case ETSBC_SolidityDataType::AddressArray:
        {
            const int32 StaticArrayLength = CTSBC_ContractAbiHelper::GetStaticArrayLength(FunctionParamType);
            if(StaticArrayLength != 0 && StaticArrayLength != ArgumentData.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Error passing arguments, Expecting %d but found %d"),
                    StaticArrayLength,
                    ArgumentData.Num());
                return false;
            }
            // if array is static store the number of elements, no need for array lenght
            DataSegmentsUsed = StaticArrayLength;
            // if the Array is dynamic then encode array length
            if(StaticArrayLength == 0)
            {
                bEncodeInPlace = false;
                // Data used is number of elements and a section for the array length
                DataSegmentsUsed = EncodeContext.Arguments[EncodeContext.ParameterIndex].Values.Num() + 1;
                FString ArrayLength = "";
                GetDynamicArrayLength(EncodeContext.Arguments[EncodeContext.ParameterIndex], ArrayLength);
                EncodedBodySection.Append(ArrayLength);
            }
            for(const FString& Element : ArgumentData)
            {
                FString EncodedElement = "";
                if(!EncodeAddress(Element, EncodedElement))
                {
                    TSBC_LOG(Error, TEXT("Error encoding Address element %s"), *Element);
                    return false;
                }

                // if the array is dynamic then add the encoded element to the body section, else add to header.
                if(StaticArrayLength == 0)
                {
                    EncodedBodySection.Append(EncodedElement);
                    continue;
                }
                EncodedHeaderSection.Append(EncodedElement);
            }
            break;
        }
    case ETSBC_SolidityDataType::String:
        {
            bEncodeInPlace = false;
            // Encode only the first element since it is not an array.
            if(!EncodeString(
                ArgumentData[0],
                DataSegmentsUsed,
                EncodedBodySection))
            {
                return false;
            }

            // Add a section for the segment holding the char count
            DataSegmentsUsed += 1;

            break;
        }
    case ETSBC_SolidityDataType::StringArray:
        {
            bEncodeInPlace = false;
            const int32 StaticArrayLength = CTSBC_ContractAbiHelper::GetStaticArrayLength(FunctionParamType);
            if(StaticArrayLength != 0 && StaticArrayLength != ArgumentData.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Error passing arguments, Expecting %d but found %d"),
                    StaticArrayLength,
                    ArgumentData.Num());
                return false;
            }
            FString L_HeaderData = "";
            // if the Array is dynamic then encode array length
            if(StaticArrayLength == 0)
            {
                FString ArrayLength = "";
                GetDynamicArrayLength(EncodeContext.Arguments[EncodeContext.ParameterIndex], ArrayLength);
                // allocate first data segment for array length
                DataSegmentsUsed = 1;
                L_HeaderData.Append(ArrayLength);
            }

            int32 L_Pointer = 0;
            FString L_BodyData = "";
            for(const FString& Element : ArgumentData)
            {
                FString L_HeaderElement = "";
                FString L_EncodedElement = "";
                uint32 L_SegmentsUsed = 0;

                EncodeDynamicDataLocation(
                    EncodeContext.Arguments[EncodeContext.ParameterIndex].Values.Num(),
                    L_Pointer,
                    L_HeaderElement);
                // Add a section for the dynamic data location
                DataSegmentsUsed += 1;

                if(!EncodeString(
                    Element,
                    L_SegmentsUsed,
                    L_EncodedElement))
                {
                    return false;
                }

                // for internal pointer add the number of segments used and 1 segment for char count
                L_Pointer += L_SegmentsUsed + 1;

                // Add a section for the segment holding the char count
                DataSegmentsUsed += L_SegmentsUsed + 1;

                L_HeaderData.Append(L_HeaderElement);
                L_BodyData.Append(L_EncodedElement);
            }
            L_HeaderData.Append(L_BodyData);
            EncodedBodySection = L_HeaderData;
            break;
        }
    case ETSBC_SolidityDataType::Bool:
        {
            // Encode only the first element since it is not an array.
            if(!EncodeBool(ArgumentData[0], EncodedHeaderSection))
            {
                return false;
            }
            // static element uses 1 segment of data
            DataSegmentsUsed = 1;
            break;
        }
    case ETSBC_SolidityDataType::BoolArray:
        {
            const int32 StaticArrayLength = CTSBC_ContractAbiHelper::GetStaticArrayLength(FunctionParamType);
            if(StaticArrayLength != 0 && StaticArrayLength != ArgumentData.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Error passing arguments, Expecting %d but found %d"),
                    StaticArrayLength,
                    ArgumentData.Num());
                return false;
            }
            // if array is static store the number of elements, no need for array lenght
            DataSegmentsUsed = StaticArrayLength;
            // if the Array is not static then treat array data as dynamic.
            if(StaticArrayLength == 0)
            {
                bEncodeInPlace = false;
                // Data used is number of elements and a section for the array length
                DataSegmentsUsed = EncodeContext.Arguments[EncodeContext.ParameterIndex].Values.Num() + 1;
                FString ArrayLength = "";
                GetDynamicArrayLength(EncodeContext.Arguments[EncodeContext.ParameterIndex], ArrayLength);
                EncodedBodySection.Append(ArrayLength);
            }
            for(const FString& Element : ArgumentData)
            {
                FString EncodedElement = "";
                if(!EncodeBool(Element, EncodedElement))
                {
                    TSBC_LOG(Error, TEXT("Error encoding Bool element %s"), *Element);
                    return false;
                }
                // if the array is dynamic then add the encoded element to the body section, else add to header.
                if(StaticArrayLength == 0)
                {
                    EncodedBodySection.Append(EncodedElement);
                    continue;
                }
                EncodedHeaderSection.Append(EncodedElement);
            }
            break;
        }
    case ETSBC_SolidityDataType::Uint:
        {
            // Encode only the first element since it is not an array.
            if(!EncodeUint(ArgumentData[0], EncodedHeaderSection))
            {
                return false;
            }
            // static element uses 1 segment of data
            DataSegmentsUsed = 1;
            break;
        }
    case ETSBC_SolidityDataType::UintArray:
        {
            const int32 StaticArrayLength = CTSBC_ContractAbiHelper::GetStaticArrayLength(FunctionParamType);
            if(StaticArrayLength != 0 && StaticArrayLength != ArgumentData.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Error passing arguments, Expecting %d but found %d"),
                    StaticArrayLength,
                    ArgumentData.Num());
                return false;
            }
            // if array is static store the number of elements, no need for array lenght
            DataSegmentsUsed = StaticArrayLength;
            // if the Array is not static then treat array data as dynamic.
            if(StaticArrayLength == 0)
            {
                bEncodeInPlace = false;
                // Data used is number of elements and a section for the array length
                DataSegmentsUsed = EncodeContext.Arguments[EncodeContext.ParameterIndex].Values.Num() + 1;
                FString ArrayLength = "";
                GetDynamicArrayLength(EncodeContext.Arguments[EncodeContext.ParameterIndex], ArrayLength);
                EncodedBodySection.Append(ArrayLength);
            }
            for(const FString& Element : ArgumentData)
            {
                FString EncodedElement = "";
                if(!EncodeUint(Element, EncodedElement))
                {
                    TSBC_LOG(Error, TEXT("Error encoding Uint element %s"), *Element);
                    return false;
                }
                // if the array is dynamic then add the encoded element to the body section, else add to header.
                if(StaticArrayLength == 0)
                {
                    EncodedBodySection.Append(EncodedElement);
                    continue;
                }
                EncodedHeaderSection.Append(EncodedElement);
            }
            break;
        }
    case ETSBC_SolidityDataType::Bytes:
        {
            bool bStatic = false;
            FString EncodedArgument;
            if(!EncodeBytes(
                FunctionParamType,
                ArgumentData[0],
                bStatic,
                EncodedArgument))
            {
                return false;
            }

            // if a bytes are static, no need to add char count segment
            DataSegmentsUsed = bStatic ? 1 : 2;

            if(!bStatic)
            {
                bEncodeInPlace = false;
                EncodedBodySection = EncodedArgument;
                break;
            }
            EncodedHeaderSection = EncodedArgument;
            break;
        }
    case ETSBC_SolidityDataType::BytesArray:
        {
            const int32 StaticArrayLength = CTSBC_ContractAbiHelper::GetStaticArrayLength(FunctionParamType);
            if(StaticArrayLength != 0 && StaticArrayLength != ArgumentData.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Error passing arguments, Expecting %d but found %d"),
                    StaticArrayLength,
                    ArgumentData.Num());
                return false;
            }

            FString HeaderData = "";
            uint32 L_DataSegmentsUsed = 0;
            // if the Array is dynamic then encode array length
            if(StaticArrayLength == 0)
            {
                bEncodeInPlace = false;
                FString ArrayLength = "";
                GetDynamicArrayLength(EncodeContext.Arguments[EncodeContext.ParameterIndex], ArrayLength);
                L_DataSegmentsUsed = 1;
                HeaderData.Append(ArrayLength);
            }

            int32 L_Pointer = 0;
            FString L_BodyData = "";

            // Remove square brackets from the parameter type.
            int32 SquareBracket;
            FunctionParamType.FindChar(TEXT('['), SquareBracket);
            FunctionParamType.MidInline(0, SquareBracket);

            for(const FString& Element : ArgumentData)
            {
                FString L_HeaderData = "";
                FString EncodedElement = "";

                bool bStatic;
                if(!EncodeBytes(
                    FunctionParamType,
                    Element,
                    bStatic,
                    EncodedElement))
                {
                    return false;
                }

                L_DataSegmentsUsed += bStatic ? 1 : 2;
                // if the element is not static then we need to add the data location first.
                if(!bStatic)
                {
                    EncodeDynamicDataLocation(
                        EncodeContext.Arguments[EncodeContext.ParameterIndex].Values.Num(),
                        L_Pointer,
                        L_HeaderData);
                    bEncodeInPlace = false;
                    L_DataSegmentsUsed += 1;
                    L_Pointer += bStatic ? 1 : 2;
                }

                HeaderData.Append(L_HeaderData);
                L_BodyData.Append(EncodedElement);
            }
            DataSegmentsUsed = L_DataSegmentsUsed;

            // if the elements are static then we encode them in-place.
            if(bEncodeInPlace)
            {
                EncodedHeaderSection = L_BodyData;
                break;
            }
            HeaderData.Append(L_BodyData);
            EncodedBodySection = HeaderData;

            break;
        }
    case ETSBC_SolidityDataType::Tuple:
        {
            bEncodeInPlace = false;
            FString EncodedTuple = "";
            bool bDynamic;
            CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
                EncodeContext.FunctionParameters[EncodeContext.ParameterIndex].TupleVariables,
                bDynamic,
                EncodeContext.TupleArgumentsTypes);

            EncodeContext.bEncodingTuple = true;
            EncodeContext.TupleParameterIndex = EncodeContext.ParameterIndex;

            uint32 L_DataSegments;
            EncodeArguments(
                EncodeContext,
                L_DataSegments,
                EncodedTuple);

            DataSegmentsUsed = L_DataSegments;
            EncodedBodySection.Append(EncodedTuple);
            break;
        }
    default:
        {
            checkNoEntry();
            break;
        }
    }

    return true;
}

bool CTSBC_ContractAbiEncoding::EncodeAddress(
    FString Address,
    FString& EncodedArguments)
{
    if(Address.IsEmpty())
    {
        return false;
    }
    Address = Address.TrimStartAndEnd();
    if(Address.StartsWith("0x"))
    {
        Address = Address.Mid(2, Address.Len());
    }
    if(Address.Len() != EthereumAddressCharLength)
    {
        TSBC_LOG(Error, TEXT("%s is not an address"), *Address);
        return false;
    }

    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(Address, AbiSegmentCharLength));
    return true;
}

bool CTSBC_ContractAbiEncoding::EncodeUint(
    FString Uint,
    FString& EncodedArguments)
{
    if(Uint.IsEmpty())
    {
        return false;
    }

    Uint = Uint.TrimStartAndEnd();
    if(Uint.StartsWith("0x"))
    {
        Uint = Uint.Mid(2, Uint.Len());
        EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(Uint, AbiSegmentCharLength));
        return true;
    }

    FString DecHexValue = "";
    if(CTSBC_BaseConverter::Dec2HexConverter().Convert(Uint, DecHexValue, 1))
    {
        EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
        return true;
    }

    return false;
}

bool CTSBC_ContractAbiEncoding::EncodeBool(
    const FString& Bool,
    FString& EncodedArguments)
{
    if(Bool.IsEmpty())
    {
        return false;
    }

    const FString DecHexValue = Bool.TrimStartAndEnd().ToBool() ? "1" : "0";
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    return true;
}

bool CTSBC_ContractAbiEncoding::EncodeString(
    const FString& String,
    uint32& DataSegmentsUsed,
    FString& EncodedArguments)
{
    if(String.IsEmpty())
    {
        return false;
    }
    const FString InString = String.TrimStartAndEnd();
    FString DecHexValue = "";
    CTSBC_BaseConverter::Dec2HexConverter().Convert(FString::FromInt(InString.Len()), DecHexValue, 1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    const FString StringHex = TSBC_StringUtils::StringToHex(InString, false);
    // data segments used plus the segment of characters length
    DataSegmentsUsed = FMath::CeilToInt(
        static_cast<float>(InString.Len()) / AbiSegmentBytesLength);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadRight(StringHex, AbiSegmentCharLength * DataSegmentsUsed));
    return true;
}

bool CTSBC_ContractAbiEncoding::EncodeBytes(
    const FString& ParameterType,
    const FString& Bytes,
    bool& bStatic,
    FString& EncodedArgument)
{
    if(Bytes.IsEmpty())
    {
        return false;
    }
    FString InBytes = Bytes.TrimStartAndEnd();
    bStatic = false;
    const FString BytesCount = ParameterType.Mid(AbiType_Bytes.Len());
    // Bytes type is Static and we need to encode in-place
    if(!BytesCount.IsEmpty())
    {
        const int32 CharactersLength = FCString::Atoi(*BytesCount) * 2;
        if(!TSBC_StringUtils::IsHexString(InBytes, true))
        {
            TSBC_LOG(Error, TEXT("Bytes argument `%s` should be in hex"), *InBytes);
            return false;
        }
        InBytes.MidInline(2);
        if(CharactersLength != InBytes.Len())
        {
            TSBC_LOG(
                Error,
                TEXT("Bytes length %d should be %d"),
                CharactersLength,
                InBytes.Len());
            return false;
        }
        EncodedArgument.Append(TSBC_StringUtils::ZeroPadRight(InBytes, AbiSegmentCharLength));
        bStatic = true;
        return true;
    }

    if(!TSBC_StringUtils::IsHexString(Bytes, true))
    {
        return false;
    }
    InBytes = InBytes.Mid(2, InBytes.Len());
    FString DecHexValue = "";
    if(InBytes.Len() % 2 != 0)
    {
        InBytes = FString("0").Append(InBytes);
    }
    CTSBC_BaseConverter::Dec2HexConverter().Convert(FString::FromInt(InBytes.Len() / 2), DecHexValue, 1);
    EncodedArgument.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    if(InBytes.Len() > AbiSegmentCharLength)
    {
        TSBC_LOG(Error, TEXT("Error encoding Bytes Argument with length %d"), InBytes.Len());
        return false;
    }
    EncodedArgument.Append(TSBC_StringUtils::ZeroPadRight(InBytes, AbiSegmentCharLength));
    return true;
}