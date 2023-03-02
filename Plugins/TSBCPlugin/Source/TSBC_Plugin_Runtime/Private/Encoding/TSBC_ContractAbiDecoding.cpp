// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_ContractAbiDecoding.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "UObject/Package.h"
// =============================================================================
#include "Data/TSBC_ContractAbiTypes.h"
#include "Encoding/TSBC_ContractAbiHelper.h"
#include "Math/TSBC_BaseConverter.h"
#include "Module/TSBC_RuntimeLogCategories.h"
#include "Util/TSBC_StringUtils.h"

void CTSBC_ContractAbiDecoding::DecodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    const FString& DataToDecode,
    TArray<FTSBC_SolidityValueList>& DecodedValues)
{
    bSuccess = false;
    ErrorMessage = "";
    FString DataToDecodeSanitized = DataToDecode.TrimStartAndEnd();
    if(!DataToDecodeSanitized.StartsWith("0x"))
    {
        ErrorMessage = FString("ResultToDecode variable is not correct");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }
    DataToDecodeSanitized.MidInline(2);

    // Sanitized data should have segments of length 64, otherwise this is not a valid data from a smart contract.
    if(DataToDecodeSanitized.Len() % 64)
    {
        ErrorMessage = FString(
            "Error decoding data");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }


    if(DataToDecodeSanitized.IsEmpty() || DataToDecodeSanitized.Len() % 64)
    {
        ErrorMessage = FString(
            "Nothing to decode");
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

    const TArray<FTSBC_SolidityFunctionSignature> FunctionOutputs = FunctionRef.Outputs;
    TArray<ETSBC_SolidityDataType> SolidityDataTypes;
    bool bContainsTuple;
    CTSBC_ContractAbiHelper::GetSolidityDataTypes(
        FunctionOutputs,
        SolidityDataTypes,
        bContainsTuple);

    if(bContainsTuple && SolidityDataTypes.Num() > 1)
    {
        ErrorMessage = FString::Printf(
            TEXT("Encoding function \"%s\" is not possible due to blueprints limitations."),
            *FunctionRef.Name);
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    // loop on function types and set CurrentTypes to switch on them
    DecodeDataRegardingTypes(
        SolidityDataTypes,
        FunctionOutputs,
        DataToDecodeSanitized,
        false,
        0,
        DecodedValues);

    bSuccess = true;
}

void CTSBC_ContractAbiDecoding::GetArrayData(
    const FString& FunctionParamType,
    const FString& DataToDecode,
    const FString& RemainingDataToDecode,
    bool& bArrayStatic,
    int32& ArrayDataLocation,
    int32& ArrayLength)
{
    const int32 StaticArrayLength = CTSBC_ContractAbiHelper::GetStaticArrayLength(FunctionParamType);
    bArrayStatic = true;
    ArrayLength = StaticArrayLength;
    // Get the location of where Array data is being stored
    const FString ArrayDataLocationHex = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    FString DataLocation;
    CTSBC_BaseConverter::Hex2DecConverter().Convert(ArrayDataLocationHex, DataLocation, 1);
    const int32 DataLocationInt = FCString::Atoi(*DataLocation);
    ArrayDataLocation = DataLocationInt * 2;

    if(StaticArrayLength == 0)
    {
        bArrayStatic = false;
        // Get the length of the Array (how many elements).
        const FString ArrayLengthHex = DataToDecode.Mid(ArrayDataLocation, AbiSegmentCharLength);
        FString ArrayLengthDecimal;
        CTSBC_BaseConverter::Hex2DecConverter().Convert(ArrayLengthHex, ArrayLengthDecimal, 1);
        ArrayLength = FCString::Atoi(*ArrayLengthDecimal);
    }
}

void CTSBC_ContractAbiDecoding::DecodeDataRegardingTypes(
    const TArray<ETSBC_SolidityDataType>& SolidityDataTypes,
    const TArray<FTSBC_SolidityFunctionSignature>& FunctionOutputs,
    const FString& DataToDecode,
    const bool& bDecodingTuple,
    const uint32& TupleParameterIndex,
    TArray<FTSBC_SolidityValueList>& DecodedValues)
{
    FString RemainingDataToDecode = DataToDecode;
    FString RemainingResult;
    for(int32 ArgumentTypeIndex = 0; ArgumentTypeIndex < SolidityDataTypes.Num(); ArgumentTypeIndex++)
    {
        FString FunctionParamType;
        if(bDecodingTuple)
        {
            FunctionParamType = FunctionOutputs[TupleParameterIndex].TupleVariables[ArgumentTypeIndex].Type;
        }
        else
        {
            FunctionParamType = FunctionOutputs[ArgumentTypeIndex].Variable.Type;
        }

        int32 SegmentsProcessed = 1;
        // Check if this is not the first value that we are getting
        if(!RemainingResult.IsEmpty())
        {
            RemainingDataToDecode = RemainingResult;
        }

        switch(SolidityDataTypes[ArgumentTypeIndex])
        {
        case ETSBC_SolidityDataType::Address:
            {
                FString DecodedValue;
                DecodeAddress(RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(FTSBC_SolidityValueList({DecodedValue}));
                break;
            }
        case ETSBC_SolidityDataType::AddressArray:
            {
                int32 ArrayLength;
                FString ArrayData;
                int32 ArrayDataLocation;
                bool bArrayStatic;

                GetArrayData(
                    FunctionParamType,
                    DataToDecode,
                    RemainingDataToDecode,
                    bArrayStatic,
                    ArrayDataLocation,
                    ArrayLength);

                if(bArrayStatic)
                {
                    SegmentsProcessed = ArrayLength;
                }
                ArrayData = DataToDecode.Mid(bArrayStatic ? 0 : (ArrayDataLocation + AbiSegmentCharLength));
                // chop to array data location then chop the array length data
                FTSBC_SolidityValueList AddressArrayData{};
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeAddress(ArrayData, DecodedValue);

                    // Add the value to the returned array then remove the decoded segment
                    AddressArrayData.Values.Add(DecodedValue);
                    ArrayData.MidInline(AbiSegmentCharLength);
                }
                DecodedValues.Add(AddressArrayData);
                break;
            }
        case ETSBC_SolidityDataType::Uint:
            {
                FString DecodedValue;
                DecodeUint(RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(FTSBC_SolidityValueList({DecodedValue}));
                break;
            }
        case ETSBC_SolidityDataType::UintArray:
            {
                int32 ArrayLength;
                FString ArrayData = DataToDecode;
                // If array is dynamic, get data location and remove the segment.
                int32 ArrayDataLocation;
                bool bArrayStatic;

                GetArrayData(
                    FunctionParamType,
                    DataToDecode,
                    RemainingDataToDecode,
                    bArrayStatic,
                    ArrayDataLocation,
                    ArrayLength);

                if(bArrayStatic)
                {
                    SegmentsProcessed = ArrayLength;
                }
                ArrayData = DataToDecode.Mid(bArrayStatic ? 0 : (ArrayDataLocation + AbiSegmentCharLength));
                // chop to array data location then chop the array length data
                FTSBC_SolidityValueList UintArrayData{};
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeUint(ArrayData, DecodedValue);
                    // Add the value to the returned array then remove the decoded segment
                    UintArrayData.Values.Add(DecodedValue);
                    ArrayData.MidInline(AbiSegmentCharLength);
                }
                DecodedValues.Add(UintArrayData);
                break;
            }
        case ETSBC_SolidityDataType::Bool:
            {
                FString DecodedValue;
                DecodeBool(RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(FTSBC_SolidityValueList({DecodedValue}));
                break;
            }
        case ETSBC_SolidityDataType::BoolArray:
            {
                int32 ArrayLength;
                FString ArrayData = DataToDecode;
                int32 ArrayDataLocation;
                bool bArrayStatic;

                GetArrayData(
                    FunctionParamType,
                    DataToDecode,
                    RemainingDataToDecode,
                    bArrayStatic,
                    ArrayDataLocation,
                    ArrayLength);

                if(bArrayStatic)
                {
                    SegmentsProcessed = ArrayLength;
                }

                ArrayData = DataToDecode.Mid(bArrayStatic ? 0 : (ArrayDataLocation + AbiSegmentCharLength));
                // chop to array data location then chop the array length data
                FTSBC_SolidityValueList BoolArrayData{};
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeBool(ArrayData, DecodedValue);
                    // Add the value to the returned array then remove the decoded segment
                    BoolArrayData.Values.Add(DecodedValue);
                    ArrayData.MidInline(AbiSegmentCharLength);
                }
                DecodedValues.Add(BoolArrayData);
                break;
            }
        case ETSBC_SolidityDataType::String:
            {
                FString DecodedValue;
                int32 StringSegmentsCount;
                DecodeString(
                    DataToDecode,
                    RemainingDataToDecode,
                    StringSegmentsCount,
                    DecodedValue);
                // Add the value to the returned array then remove the decoded segment
                DecodedValues.Add(FTSBC_SolidityValueList({DecodedValue}));
                break;
            }
        case ETSBC_SolidityDataType::StringArray:
            {
                int32 ArrayLength;
                FString RemainingArrayData;
                int32 ArrayDataLocation;
                bool bArrayStatic;

                GetArrayData(
                    FunctionParamType,
                    DataToDecode,
                    RemainingDataToDecode,
                    bArrayStatic,
                    ArrayDataLocation,
                    ArrayLength);

                // if static array, chop only array data location, else chop to array data location then chop the array length data
                RemainingArrayData = DataToDecode.Mid(
                    bArrayStatic ? ArrayDataLocation : (ArrayDataLocation + AbiSegmentCharLength));

                FString ArrayData = RemainingArrayData;
                int32 SegmentsProcessedInArray = 0;
                FTSBC_SolidityValueList StringArrayData{};
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeString(
                        ArrayData,
                        RemainingArrayData,
                        SegmentsProcessedInArray,
                        DecodedValue);
                    // Add the value to the returned array then remove the decoded segment
                    StringArrayData.Values.Add(DecodedValue);
                    RemainingArrayData.MidInline(AbiSegmentCharLength);
                }
                DecodedValues.Add(StringArrayData);
                break;
            }
        case ETSBC_SolidityDataType::Bytes:
            {
                FString DecodedValue;
                DecodeBytes(
                    FunctionParamType,
                    DataToDecode,
                    RemainingDataToDecode,
                    DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(FTSBC_SolidityValueList({DecodedValue}));
                break;
            }
        case ETSBC_SolidityDataType::BytesArray:
            {
                // Get bytes count
                int32 SquareBracket;
                FunctionParamType.FindChar(TEXT('['), SquareBracket);
                FString ByteParamType = FunctionParamType.Mid(0, SquareBracket);
                const FString BytesCount = ByteParamType.Mid(AbiType_Bytes.Len());
                bool bStaticBytes = false;
                if(!BytesCount.IsEmpty())
                {
                    bStaticBytes = true;
                }

                // Get array data location and length
                int32 ArrayLength;
                int32 ArrayDataLocation;
                bool bArrayStatic;
                FString RemainingArrayData;
                GetArrayData(
                    FunctionParamType,
                    DataToDecode,
                    RemainingDataToDecode,
                    bArrayStatic,
                    ArrayDataLocation,
                    ArrayLength);

                // only if bytes are of fixed size and the array is static, treat all elements as static types.
                if(bArrayStatic && bStaticBytes)
                {
                    SegmentsProcessed = ArrayLength;
                    RemainingArrayData = RemainingDataToDecode;
                }

                if(!bArrayStatic || !bStaticBytes)
                {
                    RemainingArrayData = DataToDecode.Mid(
                        bArrayStatic ? ArrayDataLocation : ArrayDataLocation + AbiSegmentCharLength);
                }

                // chop to array data location then chop the array length data
                FString ArrayData = RemainingArrayData;
                FTSBC_SolidityValueList BytesArrayData;
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeBytes(
                        ByteParamType,
                        ArrayData,
                        RemainingArrayData,
                        DecodedValue);
                    // Add the value to the returned array then remove the decoded segment
                    BytesArrayData.Values.Add(DecodedValue);
                    RemainingArrayData.MidInline(AbiSegmentCharLength);
                }
                DecodedValues.Add(BytesArrayData);
                break;
            }
        // case EContractAbiTypes::Int:
        //     {
        //         break;
        //     }
        // case EContractAbiTypes::IntArray:
        //     {
        //         break;
        //     }
        case ETSBC_SolidityDataType::Tuple:
            {
                // Remove the first header segment since it is tuple's location
                FString TupleDataToDecode = DataToDecode.Mid(AbiSegmentCharLength, DataToDecode.Len());

                TArray<ETSBC_SolidityDataType> TupleTypes;
                bool bContainsDynamic;
                CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
                    FunctionOutputs[ArgumentTypeIndex].TupleVariables,
                    bContainsDynamic,
                    TupleTypes);

                DecodeDataRegardingTypes(
                    TupleTypes,
                    FunctionOutputs,
                    TupleDataToDecode,
                    bContainsDynamic,
                    ArgumentTypeIndex,
                    DecodedValues);
                break;
            }

        default:
            {
                checkNoEntry();
                break;
            }
        }
        // go to next header segment
        RemainingResult = RemainingDataToDecode.Mid(AbiSegmentCharLength * SegmentsProcessed);
    }
}

void CTSBC_ContractAbiDecoding::DecodeAddress(
    const FString& DataToDecode,
    FString& DecodedValue)
{
    DecodedValue = FString("0x").Append(
        DataToDecode.Mid(
            AbiSegmentCharLength - EthereumAddressCharLength,
            EthereumAddressCharLength));
}

void CTSBC_ContractAbiDecoding::DecodeUint(
    const FString& RemainingDataToDecode,
    FString& DecodedValue)
{
    // Get the Uint data
    const FString UintHexData = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    // Convert String hex value to Decimal
    CTSBC_BaseConverter::Hex2DecConverter().Convert(UintHexData, DecodedValue, 1);
}

void CTSBC_ContractAbiDecoding::DecodeBool(
    const FString& RemainingDataToDecode,
    FString& DecodedValue)
{
    // Get the Bool data
    const FString BoolHexData = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    FString InDecodedValue;
    // Convert String hex value to int
    CTSBC_BaseConverter::Hex2DecConverter().Convert(BoolHexData, InDecodedValue, 1);
    DecodedValue = FCString::Atoi(*InDecodedValue) == 1 ? "True" : "False";
}

void CTSBC_ContractAbiDecoding::DecodeString(
    const FString& DataToDecode,
    const FString& RemainingDataToDecode,
    int32& SegmentsProcessed,
    FString& DecodedValue)
{
    FString Tmp;

    // Get the location of where String data is being stored
    const FString DataStartingOffset = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    CTSBC_BaseConverter::Hex2DecConverter().Convert(DataStartingOffset, Tmp, 1);
    const int32 DataStartingOffsetAsInt = FCString::Atoi(*Tmp);

    // Chop the Result string to where the data location is which contains the String length and its actual value
    const FString DataWithStringLengthAndValue = DataToDecode.Mid(DataStartingOffsetAsInt * 2);

    // Get the length of String (how many characters).
    const FString StringLength = DataWithStringLengthAndValue.Mid(0, AbiSegmentCharLength);
    CTSBC_BaseConverter::Hex2DecConverter().Convert(StringLength, Tmp, 1);
    const int32 StringLengthAsInt = FCString::Atoi(*Tmp);

    // Is the String taking 1 segment(32 Hex Bytes or more) SegmentCharLength
    SegmentsProcessed = FMath::CeilToInt(
        static_cast<float>(StringLengthAsInt) / AbiSegmentBytesLength);

    const FString StringValueAsHex = DataWithStringLengthAndValue.Mid(
        AbiSegmentCharLength,
        AbiSegmentCharLength * SegmentsProcessed);

    // Remove the length data and keep the actual String data
    FString StringSanitized = TSBC_StringUtils::BytesToStringUtf8(
        TSBC_StringUtils::HexToBytes(StringValueAsHex)).Mid(0, StringLengthAsInt);

    DecodedValue = StringSanitized;
}

void CTSBC_ContractAbiDecoding::DecodeBytes(
    const FString& FunctionParamType,
    const FString& DataToDecode,
    const FString& RemainingDataToDecode,
    FString& DecodedValue)
{
    const FString BytesCount = FunctionParamType.Mid(AbiType_Bytes.Len());
    if(!BytesCount.IsEmpty())
    {
        const int32 BytesCountAsInt = FCString::Atoi(*BytesCount);
        // if bytes are static, just take the first segment and decode it.
        FString BytesValue = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
        // when bytes is static, take only bytes count.
        BytesValue.MidInline(0, BytesCountAsInt * 2);
        DecodedValue = FString("0x").Append(BytesValue);
        return;
    }

    FString Tmp;
    // Get the location of where Bytes data is being stored
    const FString DataStartingOffset = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    CTSBC_BaseConverter::Hex2DecConverter().Convert(DataStartingOffset, Tmp, 1);
    const int32 DataStartingOffsetAsInt = FCString::Atoi(*Tmp);

    // Chop the Result Bytes to where the data location is which contains the Bytes length and its actual value
    const FString DataWithBytesLengthAndValue = DataToDecode.Mid(
        DataStartingOffsetAsInt * 2,
        RemainingDataToDecode.Len());

    // Get the length of Bytes (how many characters).
    const FString BytesLength = DataWithBytesLengthAndValue.Mid(0, AbiSegmentCharLength);
    CTSBC_BaseConverter::Hex2DecConverter().Convert(BytesLength, Tmp, 1);
    const int32 BytesLengthAsInt = FCString::Atoi(*Tmp);

    const FString BytesData = DataWithBytesLengthAndValue.Mid(
        AbiSegmentCharLength,
        AbiSegmentCharLength * BytesLengthAsInt);

    // Remove the length data and keep the actual Bytes data
    const FString BytesValue = BytesData.Mid(0, AbiSegmentCharLength);
    FString StrippedValue = TSBC_StringUtils::StripTrailingZeroes(BytesValue);
    if(StrippedValue.Len() % 2)
    {
        StrippedValue.Append("0");
    }
    DecodedValue = FString("0x").Append(StrippedValue);
}