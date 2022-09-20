// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_ContractAbi.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "UObject/Package.h"
// =============================================================================
#include "Crypto/Hash/TSBC_HashFunctionLibrary.h"
#include "Data/TSBC_ContractAbiTypes.h"
#include "Kismet/KismetStringLibrary.h"
#include "Math/TSBC_BaseConverter.h"
#include "Module/TSBC_RuntimeLogCategories.h"
#include "Util/TSBC_StringUtils.h"

const FString JsonKey_Anonymous = "anonymous";
const FString JsonKey_Components = "components";
const FString JsonKey_Indexed = "indexed";
const FString JsonKey_Inputs = "inputs";
const FString JsonKey_Name = "name";
const FString JsonKey_Outputs = "outputs";
const FString JsonKey_StateMutability = "stateMutability";
const FString JsonKey_Type = "type";
// NOTE: Unused for now
// const FString JsonKey_StateMutabilityNonPayable = "nonpayable";
// const FString JsonKey_StateMutabilityPure = "pure";
// const FString JsonKey_StateMutabilityPayable = "payable";
// const FString JsonKey_StateMutabilityView = "view";

const FString AbiType_Address = "address";
const FString AbiType_String = "string";
const FString AbiType_Bool = "bool";
const FString AbiType_Uint = "uint";
const FString AbiType_Int = "int";
const FString AbiType_Bytes = "bytes";
const FString AbiType_Tuple = "tuple";

constexpr int32 AbiSegmentBytesLength = 32;
constexpr int32 AbiSegmentCharLength = 64;
constexpr int32 EthereumAddressCharLength = 40;

bool CTSBC_ContractAbi::GetFunctionsFromAbi(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    FString& ErrorMessage,
    FTSBC_ContractAbiFunction& FunctionStruct)
{
    FString FunctionType;
    if(!JsonObject->TryGetStringField(JsonKey_Type, FunctionType))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }
    FString FunctionName;
    if(!JsonObject->TryGetStringField(JsonKey_Name, FunctionName))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Name);
        return false;
    }
    FString FunctionMutability;
    if(!JsonObject->TryGetStringField(JsonKey_StateMutability, FunctionMutability))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_StateMutability);
        return false;
    }

    FunctionStruct.Type = FunctionType;
    FunctionStruct.Name = FunctionName;
    FunctionStruct.StateMutability = FunctionMutability;

    TArray<FTSBC_ContractAbiFunctionInput> FunctionInput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Inputs,
        ErrorMessage,
        FunctionInput))
    {
        FunctionStruct.Inputs = FunctionInput;
    }

    TArray<FTSBC_ContractAbiFunctionInput> FunctionOutput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Outputs,
        ErrorMessage,
        FunctionOutput))
    {
        FunctionStruct.Outputs = FunctionOutput;
    }

    return true;
}

bool CTSBC_ContractAbi::GetReceiveFromAbi(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    FString& ErrorMessage,
    FTSBC_ContractAbiFunction& ReceiveStruct)
{
    FString FunctionType;
    if(!JsonObject->TryGetStringField(JsonKey_Type, FunctionType))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }
    FString FunctionName;
    if(JsonObject->TryGetStringField(JsonKey_Name, FunctionName))
    {
        ReceiveStruct.Name = FunctionName;
    }
    FString FunctionMutability;
    if(!JsonObject->TryGetStringField(JsonKey_StateMutability, FunctionMutability))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }
    ReceiveStruct.Type = FunctionType;
    ReceiveStruct.StateMutability = FunctionMutability;

    TArray<FTSBC_ContractAbiFunctionInput> FunctionInput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Inputs,
        ErrorMessage,
        FunctionInput))
    {
        ReceiveStruct.Inputs = FunctionInput;
    }

    TArray<FTSBC_ContractAbiFunctionInput> FunctionOutput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Outputs,
        ErrorMessage,
        FunctionOutput))
    {
        ReceiveStruct.Outputs = FunctionOutput;
    }

    return true;
}

bool CTSBC_ContractAbi::GetConstructorFromAbi(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    FString& ErrorMessage,
    FTSBC_ContractAbiFunction& ConstructorStruct)
{
    FString ConstructorType;
    if(!JsonObject->TryGetStringField(JsonKey_Type, ConstructorType))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }
    FString ConstructorMutability;
    if(!JsonObject->TryGetStringField(JsonKey_StateMutability, ConstructorMutability))
    {
        ErrorMessage = FString::Printf(TEXT("GetConstructorFromAbi Could not find key '%s'"), *JsonKey_StateMutability);
        return false;
    }
    ConstructorStruct.Type = ConstructorType;
    ConstructorStruct.StateMutability = ConstructorMutability;

    TArray<FTSBC_ContractAbiFunctionInput> FunctionInput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Inputs,
        ErrorMessage,
        FunctionInput))
    {
        ConstructorStruct.Inputs = FunctionInput;
    }

    return true;
}

bool CTSBC_ContractAbi::GetFallbacksFromAbi(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    FString& ErrorMessage,
    FTSBC_ContractAbiFunction& FallbackStruct)
{
    FString ContractFallbackType;
    if(!JsonObject->TryGetStringField(JsonKey_Type, ContractFallbackType))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }

    FString ContractFallbackMutability;
    if(!JsonObject->TryGetStringField(JsonKey_StateMutability, ContractFallbackMutability))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_StateMutability);
        return false;
    }

    FallbackStruct.Type = ContractFallbackType;
    FallbackStruct.StateMutability = ContractFallbackMutability;

    return true;
}

bool CTSBC_ContractAbi::GetEventsFromAbi(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    FString& ErrorMessage,
    FTSBC_ContractAbiEvent& EventStruct)
{
    FString EventType;
    if(!JsonObject->TryGetStringField(JsonKey_Type, EventType))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }

    FString EventName;
    if(!JsonObject->TryGetStringField(JsonKey_Name, EventName))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Name);
        return false;
    }

    bool EventAnonymous;
    if(!JsonObject->TryGetBoolField(JsonKey_Anonymous, EventAnonymous))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Anonymous);
        return false;
    }

    EventStruct.Type = EventType;
    EventStruct.Name = EventName;
    EventStruct.Anonymous = EventAnonymous;

    TArray<FTSBC_ContractAbiFunctionInput> FunctionInput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Inputs,
        ErrorMessage,
        FunctionInput))
    {
        EventStruct.Inputs = FunctionInput;
    }

    return true;
}

bool CTSBC_ContractAbi::GetErrorsFromAbi(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    FString& ErrorMessage,
    FTSBC_ContractAbiError& ErrorStruct)
{
    FString ContractErrorType;
    if(!JsonObject->TryGetStringField(JsonKey_Type, ContractErrorType))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }

    FString ContractErrorName;
    if(!JsonObject->TryGetStringField(JsonKey_Name, ContractErrorName))
    {
        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Type);
        return false;
    }

    ErrorStruct.Type = ContractErrorType;
    ErrorStruct.Name = ContractErrorName;

    TArray<FTSBC_ContractAbiFunctionInput> FunctionInput;
    if(GetFunctionInputOrOutput(JsonObject, JsonKey_Inputs, ErrorMessage, FunctionInput))
    {
        ErrorStruct.Inputs = FunctionInput;
    }

    return true;
}

bool CTSBC_ContractAbi::GetFunctionInputOrOutput(
    const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
    const FString FieldName,
    FString& ErrorMessage,
    TArray<FTSBC_ContractAbiFunctionInput>& FunctionInput)
{
    const TArray<TSharedPtr<FJsonValue>>* FunctionInputs;
    if(JsonObject->TryGetArrayField(FieldName, FunctionInputs))
    {
        for(const auto& Input : *FunctionInputs)
        {
            FTSBC_ContractAbiFunctionInput LocalFunctionInput;
            const TSharedPtr<FJsonObject, ESPMode::Fast>* InputObject;
            if(!Input->TryGetObject(InputObject))
            {
                ErrorMessage = "Corrupted Contract ABI JSON: Could not find object.";
                return false;
            }
            FString InputType;
            if(!InputObject->Get()->TryGetStringField(JsonKey_Type, InputType))
            {
                ErrorMessage = FString::Printf(TEXT("Could not find keysssss '%s'"), *JsonKey_Type);
                return false;
            }
            FString InputName;
            if(!InputObject->Get()->TryGetStringField(JsonKey_Name, InputName))
            {
                ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Name);
                return false;
            }
            bool InputIndexed;
            if(InputObject->Get()->TryGetBoolField(JsonKey_Indexed, InputIndexed))
            {
                LocalFunctionInput.Indexed = InputIndexed;
            }
            LocalFunctionInput.Component.Type = InputType;
            LocalFunctionInput.Component.Name = InputName;

            const TArray<TSharedPtr<FJsonValue>>* InputComponent;
            if(InputObject->Get()->TryGetArrayField(JsonKey_Components, InputComponent))
            {
                for(const auto& Component : *InputComponent)
                {
                    const TSharedPtr<FJsonObject>* ComponentObject;
                    if(!Component->TryGetObject(ComponentObject))
                    {
                        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Name);
                        return false;
                    }
                    FTSBC_ContractAbiInputComponent LocalComponentInputStruct;
                    FString ComponentType;
                    if(!ComponentObject->Get()->TryGetStringField(JsonKey_Type, ComponentType))
                    {
                        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Name);
                        return false;
                    }
                    FString ComponentName;
                    if(!ComponentObject->Get()->TryGetStringField(JsonKey_Name, ComponentName))
                    {
                        ErrorMessage = FString::Printf(TEXT("Could not find key '%s'"), *JsonKey_Name);
                        return false;
                    }
                    LocalComponentInputStruct.Type = ComponentType;
                    LocalComponentInputStruct.Name = ComponentName;
                    LocalFunctionInput.Components.Add(LocalComponentInputStruct);
                }
            }
            FunctionInput.Add(LocalFunctionInput);
        }
    }
    return true;
}

bool CTSBC_ContractAbi::IsFunctionAvailable(
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    FTSBC_ContractAbiFunction& FunctionRef)
{
    TArray<FTSBC_ContractAbiFunction> FunctionsRef = ContractAbi.ContractAbiFunctions;

    for(const FTSBC_ContractAbiFunction& Function : FunctionsRef)
    {
        if(Function.Name == FunctionName)
        {
            FunctionRef = Function;
            return true;
        }
    }
    return false;
}

void CTSBC_ContractAbi::ParseAbiFromJson(
    bool& bSuccess,
    FString& ErrorMessage,
    const FString& ContractAbiJson,
    FTSBC_ContractAbi& ContractAbi)
{
    bSuccess = false;
    ErrorMessage = "";

    // Create a reader pointer to read the json data
    TSharedPtr<FJsonValue> JsonValue;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ContractAbiJson);
    if(!FJsonSerializer::Deserialize(Reader, JsonValue))
    {
        ErrorMessage = "Invalid JSON: Could not deserialize JSON.";
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* JsonArray;
    if(!JsonValue->TryGetArray(JsonArray))
    {
        ErrorMessage = "Invalid JSON: Not a Contract ABI.";
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    for(const TSharedPtr<FJsonValue, ESPMode::Fast>& JsonValueElement : *JsonArray)
    {
        const TSharedPtr<FJsonObject, ESPMode::Fast>* JsonObject;
        if(!JsonValueElement->TryGetObject(JsonObject))
        {
            ErrorMessage = "Corrupted Contract ABI JSON: Could not find object.";
            TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
            return;
        }

        // Loop through all JSON objects and get the data regarding Contract ABI Function Types
        FString FunctionType;
        if(!JsonObject->Get()->TryGetStringField(JsonKey_Type, FunctionType))
        {
            ErrorMessage = FString::Printf(
                TEXT("Corrupted Contract ABI JSON: Could not find key '%s'."),
                *JsonKey_Type);
            TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
            return;
        }

        const FName TypeConvertedToName = FName(*FunctionType);
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EContractABIFunctionTypes"), true);
        if(!EnumPtr)
        {
            ErrorMessage = "Invalid Enum Pointer";
            TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
            return;
        }
        const EContractAbiFunctionTypes Index = static_cast<EContractAbiFunctionTypes>(EnumPtr->GetIndexByName(
            TypeConvertedToName));
        switch(Index)
        {
        case EContractAbiFunctionTypes::Constructor:
            {
                FTSBC_ContractAbiFunction ConstructorStruct;
                if(!GetConstructorFromAbi(*JsonObject, ErrorMessage, ConstructorStruct))
                {
                    TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                    return;
                }
                ContractAbi.ContractAbiFunctions.Add(ConstructorStruct);
                break;
            }

        case EContractAbiFunctionTypes::Error:
            {
                FTSBC_ContractAbiError ErrorStruct;
                if(!GetErrorsFromAbi(*JsonObject, ErrorMessage, ErrorStruct))
                {
                    TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                    return;
                }
                ContractAbi.ContractAbiErrors.Add(ErrorStruct);
                break;
            }

        case EContractAbiFunctionTypes::Event:
            {
                FTSBC_ContractAbiEvent EventStruct;
                if(!GetEventsFromAbi(*JsonObject, ErrorMessage, EventStruct))
                {
                    TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                    return;
                }
                ContractAbi.ContractAbiEvents.Add(EventStruct);
                break;
            }

        case EContractAbiFunctionTypes::Function:
            {
                FTSBC_ContractAbiFunction FunctionStruct;
                if(!GetFunctionsFromAbi(*JsonObject, ErrorMessage, FunctionStruct))
                {
                    TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                    return;
                }
                ContractAbi.ContractAbiFunctions.Add(FunctionStruct);
                break;
            }

        case EContractAbiFunctionTypes::Fallback:
            {
                FTSBC_ContractAbiFunction LocalFallbackStruct;
                if(!GetFallbacksFromAbi(*JsonObject, ErrorMessage, LocalFallbackStruct))
                {
                    TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                    return;
                }

                ContractAbi.ContractAbiFunctions.Add(LocalFallbackStruct);
                break;
            }

        case EContractAbiFunctionTypes::Receive:
            {
                FTSBC_ContractAbiFunction ReceiveStruct;
                if(!GetReceiveFromAbi(*JsonObject, ErrorMessage, ReceiveStruct))
                {
                    TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                    return;
                }
                ContractAbi.ContractAbiFunctions.Add(ReceiveStruct);
                break;
            }

        default:
            {
                ErrorMessage = FString::Printf(TEXT("Type '%s' is not supported"), *FunctionType);
                TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
                return;
            }
        }
    }

    bSuccess = true;
}

void CTSBC_ContractAbi::EncodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    const TArray<FString>& FunctionArguments,
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
    if(!IsFunctionAvailable(ContractAbi, FunctionName, FunctionRef))
    {
        ErrorMessage = FString("Function not found");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    // check if the number of arguments passed in FunctionArguments array matches the function arguments in the ABI.
    if(FunctionRef.Inputs.Num() != FunctionArguments.Num())
    {
        ErrorMessage = FString::Printf(
            TEXT("Error passing arguments, Expecting %d but found %d"),
            FunctionRef.Inputs.Num(),
            FunctionArguments.Num());

        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FString InputsRef;
    // get the input types for the function to use in making the function selector(Hash).
    for(int32 i = 0; i < FunctionRef.Inputs.Num(); i++)
    {
        // if the type is tuple we need to add it's internal types to the function selector as one object.
        if(FunctionRef.Inputs[i].Component.Type == AbiType_Tuple)
        {
            InputsRef.Append("(");
            for(int32 j = 0; j < FunctionRef.Inputs[i].Components.Num(); j++)
            {
                if(j + 1 != FunctionRef.Inputs[i].Components.Num())
                {
                    InputsRef.Append(FunctionRef.Inputs[i].Components[j].Type + ",");
                    continue;
                }
                InputsRef.Append(FunctionRef.Inputs[i].Components[j].Type);
            }
            InputsRef.Append(")");
            if(i + 1 != FunctionRef.Inputs.Num())
            {
                InputsRef.Append(",");
            }
            continue;
        }

        if(i + 1 != FunctionRef.Inputs.Num())
        {
            InputsRef.Append(FunctionRef.Inputs[i].Component.Type + ",");
            continue;
        }

        InputsRef.Append(FunctionRef.Inputs[i].Component.Type);
    }


    const FString Signature = FString::Printf(
        TEXT("%s(%s)"),
        *FunctionRef.Name,
        *InputsRef);

    // Create the function selector Hash
    FString FunctionSelector = UTSBC_HashFunctionLibrary::Keccak256FromString(Signature, false);
    FunctionSelector = FunctionSelector.LeftChop(56);
    FunctionSelectorAndEncodedArguments = FString("0x").Append(FunctionSelector);
    FString EncodedArguments = "";

    // Encode the arguments passed relative to their types then add them to the function selector string.
    if(!EncodeArguments(FunctionRef.Inputs, FunctionArguments, EncodedArguments))
    {
        ErrorMessage = "Error encoding arguments";
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FunctionSelectorAndEncodedArguments.Append(EncodedArguments);

    bSuccess = true;
}

void CTSBC_ContractAbi::DecodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    const FString& DataToDecode,
    TArray<FString>& DecodedValues)
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

    if(DataToDecodeSanitized.IsEmpty())
    {
        ErrorMessage = FString(
            "Nothing to decode");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    FTSBC_ContractAbiFunction Function;
    if(!IsFunctionAvailable(ContractAbi, FunctionName, Function))
    {
        ErrorMessage = FString("Function not found");
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    TArray<EContractAbiTypes> ArgumentsTypes = {};
    const TArray<FTSBC_ContractAbiFunctionInput> FunctionOutputs = Function.Outputs;

    // loop on function types and set CurrentTypes to switch on them
    GetArgumentsTypes(FunctionOutputs, ArgumentsTypes);

    DecodeDataRegardingTypes(
        ArgumentsTypes,
        false,
        FunctionOutputs,
        DataToDecodeSanitized,
        DecodedValues);

    bSuccess = true;
}

bool CTSBC_ContractAbi::EncodeArguments(
    const TArray<FTSBC_ContractAbiFunctionInput>& FunctionInputTypes,
    const TArray<FString>& Arguments,
    FString& EncodedArguments)
{
    TArray<EContractAbiTypes> ArgumentsTypes = {};
    GetArgumentsTypes(FunctionInputTypes, ArgumentsTypes);

    TArray<uint32> DynamicTypesIndexes;
    if(!EncodeStaticTypes(
        FunctionInputTypes,
        false,
        0,
        ArgumentsTypes,
        Arguments,
        DynamicTypesIndexes,
        EncodedArguments))
    {
        return false;
    }

    if(!EncodeDynamicTypes(
        FunctionInputTypes,
        ArgumentsTypes,
        Arguments,
        DynamicTypesIndexes,
        EncodedArguments))
    {
        return false;
    }

    return true;
}

bool CTSBC_ContractAbi::EncodeStaticTypes(
    const TArray<FTSBC_ContractAbiFunctionInput> FunctionInputTypes,
    const bool bEncodingTuple,
    const uint32 TupleIndex,
    const TArray<EContractAbiTypes>& ArgumentsTypes,
    const TArray<FString>& Arguments,
    TArray<uint32>& DynamicTypesIndexes,
    FString& EncodedArguments)
{
    uint32 PaddingNextValue = 0;

    for(int32 i = 0; i < ArgumentsTypes.Num(); i++)
    {
        uint32 InputCount;
        FString FunctionType;
        if(!bEncodingTuple)
        {
            FunctionType = FunctionInputTypes[i].Component.Type;
            InputCount = FunctionInputTypes.Num();
        }
        else
        {
            // Get the input type and count from the components inside the tuple type.
            FunctionType = FunctionInputTypes[TupleIndex].Components[i].Type;
            InputCount = FunctionInputTypes[TupleIndex].Components.Num();
        }

        bool bFunctionTypeHandled = false;
        bFunctionTypeHandled = EncodeDynamicArrayData(
            Arguments[i],
            InputCount,
            i,
            FunctionType,
            ArgumentsTypes,
            PaddingNextValue,
            EncodedArguments);
        if(bFunctionTypeHandled)
        {
            DynamicTypesIndexes.Add(i);
            continue;
        }

        bFunctionTypeHandled = EncodeStaticData(
            Arguments[i],
            InputCount,
            i,
            FunctionType,
            ArgumentsTypes,
            PaddingNextValue,
            DynamicTypesIndexes,
            EncodedArguments);
        if(bFunctionTypeHandled)
        {
            continue;
        }

        if(!bFunctionTypeHandled)
        {
            TSBC_LOG(Error, TEXT("Unsupported Type '%s' in function"), *FunctionType);
            return false;
        }
    }
    return true;
}

bool CTSBC_ContractAbi::EncodeDynamicTypes(
    const TArray<FTSBC_ContractAbiFunctionInput> InTypes,
    const TArray<EContractAbiTypes>& CurrentTypes,
    const TArray<FString>& Arguments,
    TArray<uint32>& DynamicTypesIndexes,
    FString& EncodedArguments)
{
    for(int32 i = 0; i < DynamicTypesIndexes.Num(); i++)
    {
        switch(CurrentTypes[DynamicTypesIndexes[i]])
        {
        case EContractAbiTypes::String:
            {
                if(!EncodeString(Arguments[DynamicTypesIndexes[i]].TrimStartAndEnd(), EncodedArguments))
                {
                    TSBC_LOG(Error, TEXT("Error encoding String %s"), *Arguments[DynamicTypesIndexes[i]]);
                    return false;
                }

                break;
            }

        case EContractAbiTypes::Bytes:
            {
                if(!EncodeBytes(Arguments[DynamicTypesIndexes[i]].TrimStartAndEnd(), EncodedArguments))
                {
                    TSBC_LOG(Error, TEXT("Error encoding Bytes %s"), *Arguments[DynamicTypesIndexes[i]]);
                    return false;
                }
                break;
            }

        case EContractAbiTypes::Tuple:
            {
                TArray<EContractAbiTypes> CurrentTupleTypes = {};
                TArray<uint32> DynamicTupleTypesIndexes;
                bool bIsDynamic;
                GetTupleTypes(InTypes[i].Components, bIsDynamic, CurrentTupleTypes);
                TArray<FString> OutArray;
                Arguments[i].ParseIntoArray(OutArray, TEXT(","), true);
                // Encode Tuple static types
                if(!EncodeStaticTypes(
                    InTypes,
                    true,
                    i,
                    CurrentTupleTypes,
                    OutArray,
                    DynamicTupleTypesIndexes,
                    EncodedArguments))
                {
                    TSBC_LOG(
                        Error,
                        TEXT("Error encoding static types for Tuple %s"),
                        *Arguments[DynamicTypesIndexes[i]]);
                    return false;
                }

                // Encode Tuple dynamic types
                if(!EncodeDynamicTypes(
                    InTypes,
                    CurrentTupleTypes,
                    OutArray,
                    DynamicTupleTypesIndexes,
                    EncodedArguments))
                {
                    TSBC_LOG(
                        Error,
                        TEXT("Error encoding dynamic types for Tuple %s"),
                        *Arguments[DynamicTypesIndexes[i]]);
                    return false;
                }
                break;
            }

        case EContractAbiTypes::AddressArray:
            {
                TArray<FString> OutArray;
                GetDynamicArrayLength(Arguments[DynamicTypesIndexes[i]], OutArray, EncodedArguments);
                for(const FString& Element : OutArray)
                {
                    if(!EncodeAddress(Element, EncodedArguments))
                    {
                        TSBC_LOG(Error, TEXT("Error encoding Address element %s"), *Element);
                        return false;
                    }
                }
                break;
            }

        case EContractAbiTypes::UintArray:
            {
                TArray<FString> OutArray;
                GetDynamicArrayLength(Arguments[DynamicTypesIndexes[i]], OutArray, EncodedArguments);
                for(const FString& Element : OutArray)
                {
                    if(!EncodeUint(Element, EncodedArguments))
                    {
                        TSBC_LOG(Error, TEXT("Error encoding Uint element %s"), *Element);
                        return false;
                    }
                }
                break;
            }

        case EContractAbiTypes::BoolArray:
            {
                TArray<FString> OutArray;
                GetDynamicArrayLength(Arguments[DynamicTypesIndexes[i]], OutArray, EncodedArguments);
                for(const FString& Element : OutArray)
                {
                    if(!EncodeBool(Element, EncodedArguments))
                    {
                        TSBC_LOG(Error, TEXT("Error encoding Bool element %s"), *Element);
                        return false;
                    }
                }
                break;
            }

        case EContractAbiTypes::StringArray:
            {
                TArray<FString> OutArray;
                TArray<int32> DynamicTypesStringIndexes;
                GetDynamicArrayLength(Arguments[DynamicTypesIndexes[i]], OutArray, EncodedArguments);
                int32 CurrentStringPadding = 0;
                int32 PaddingStringNextValue = 0;

                // Encode String data location
                for(const FString& Element : OutArray)
                {
                    FString ElementTemp = Element.TrimStartAndEnd();
                    CurrentStringPadding = PaddingStringNextValue;
                    if(!EncodeStringDataLocation(OutArray.Num(), CurrentStringPadding, EncodedArguments))
                    {
                        return false;
                    }

                    // calculate number of segments a string is going to use
                    float NumberOfStringSegments = FMath::CeilToInt(
                        static_cast<float>(ElementTemp.Len()) / AbiSegmentBytesLength);
                    DynamicTypesStringIndexes.Add(i);
                    PaddingStringNextValue = CurrentStringPadding + 1 + NumberOfStringSegments;
                }
                // Encode String data
                for(const FString& Element : OutArray)
                {
                    FString ElementTemp = Element.TrimStartAndEnd();
                    if(!EncodeString(ElementTemp, EncodedArguments))
                    {
                        TSBC_LOG(Error, TEXT("Error encoding String element %s"), *Element);
                        return false;
                    }
                }
                break;
            }

        case EContractAbiTypes::BytesArray:
            {
                TArray<FString> OutArray;
                TArray<int32> DynamicTypesStringIndexes;
                GetDynamicArrayLength(Arguments[DynamicTypesIndexes[i]], OutArray, EncodedArguments);
                int32 CurrentStringPadding = 0;
                int32 PaddingStringNextValue = 0;
                // Encode Bytes data location
                for(const FString& _ : OutArray)
                {
                    CurrentStringPadding = PaddingStringNextValue;
                    if(!EncodeStringDataLocation(OutArray.Num(), CurrentStringPadding, EncodedArguments))
                    {
                        return false;
                    }
                    DynamicTypesStringIndexes.Add(i);
                    PaddingStringNextValue = CurrentStringPadding + 2;
                }
                // Encode Bytes data
                for(const FString& Element : OutArray)
                {
                    FString ElementTemp = Element.TrimStartAndEnd();
                    if(!EncodeBytes(ElementTemp, EncodedArguments))
                    {
                        TSBC_LOG(Error, TEXT("Error encoding Bytes element %s"), *ElementTemp);
                        return false;
                    }
                }
                break;
            }

        // case EContractAbiTypes::IntArray:
        //     {
        //         TArray<FString> OutArray;
        //         GetDynamicArrayLength(Arguments[DynamicTypesIndexes[i]], OutArray, EncodedArguments);
        //         for(FString element : OutArray)
        //         {
        //             element = element.TrimStartAndEnd();
        //             if(!EncodeUint(element, EncodedArguments))
        //             {
        //                 TSBC_LOG(Error, TEXT("Error encoding int element %s"), *element);
        //                 return false;
        //             }
        //         }
        //         break;
        //     }

        // case EContractAbiTypes::TupleArray:
        //     {
        //         // TODO Deal with Tuple Array
        //         TSBC_LOG(Warning, TEXT("Type is Tuple Array currently not supported"));
        //         break;
        //     }

        default:
            {
                checkNoEntry();
                break;
            }
        }

        EncodedArguments = EncodedArguments.ToLower();
    }
    return true;
}

void CTSBC_ContractAbi::GetArgumentsTypes(
    TArray<FTSBC_ContractAbiFunctionInput> FunctionInputTypes,
    TArray<EContractAbiTypes>& ArgumentsTypes)
{
    for(int32 i = 0; i < FunctionInputTypes.Num(); i++)
    {
        FString ArgumentType = FunctionInputTypes[i].Component.Type;

        if(ArgumentType.StartsWith(AbiType_Address))
        {
            if(ArgumentType.EndsWith("]"))
            {
                ArgumentsTypes.Add(EContractAbiTypes::AddressArray);
                continue;
            }

            ArgumentsTypes.Add(EContractAbiTypes::Address);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Uint))
        {
            if(ArgumentType.EndsWith("]"))
            {
                ArgumentsTypes.Add(EContractAbiTypes::UintArray);
                continue;
            }

            ArgumentsTypes.Add(EContractAbiTypes::Uint);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Bool))
        {
            if(ArgumentType.EndsWith("]"))
            {
                ArgumentsTypes.Add(EContractAbiTypes::BoolArray);
                continue;
            }
            ArgumentsTypes.Add(EContractAbiTypes::Bool);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_String))
        {
            if(ArgumentType.EndsWith("]"))
            {
                ArgumentsTypes.Add(EContractAbiTypes::StringArray);
                continue;
            }

            ArgumentsTypes.Add(EContractAbiTypes::String);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Bytes))
        {
            if(ArgumentType.EndsWith("]"))
            {
                ArgumentsTypes.Add(EContractAbiTypes::BytesArray);
                continue;
            }

            ArgumentsTypes.Add(EContractAbiTypes::Bytes);
            continue;
        }

        // if(ArgumentType.StartsWith(AbiType_Int))
        // {
        //     if(ArgumentType.EndsWith("]"))
        //     {
        //         ArgumentsTypes.Add(EContractAbiTypes::IntArray);
        //         continue;
        //     }
        //     ArgumentsTypes.Add(EContractAbiTypes::Int);
        //     continue;
        // }

        if(ArgumentType.StartsWith(AbiType_Tuple))
        {
            // if(ArgumentType.EndsWith("]"))
            // {
            //     ArgumentsTypes.Add(EContractAbiTypes::TupleArray);
            //     continue;
            // }

            ArgumentsTypes.Add(EContractAbiTypes::Tuple);
        }
    }
}

void CTSBC_ContractAbi::GetTupleTypes(
    TArray<FTSBC_ContractAbiInputComponent> InTypes,
    bool& bContainsDynamic,
    TArray<EContractAbiTypes>& CurrentTypes)
{
    bContainsDynamic = false;
    for(int32 i = 0; i < InTypes.Num(); i++)
    {
        FString OutputType = InTypes[i].Type;

        if(OutputType.StartsWith(AbiType_Address))
        {
            if(OutputType.EndsWith("]"))
            {
                bContainsDynamic = true;
                CurrentTypes.Add(EContractAbiTypes::AddressArray);
                continue;
            }

            CurrentTypes.Add(EContractAbiTypes::Address);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Uint))
        {
            if(OutputType.EndsWith("]"))
            {
                bContainsDynamic = true;
                CurrentTypes.Add(EContractAbiTypes::UintArray);
                continue;
            }

            CurrentTypes.Add(EContractAbiTypes::Uint);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Bool))
        {
            if(OutputType.EndsWith("]"))
            {
                bContainsDynamic = true;
                CurrentTypes.Add(EContractAbiTypes::BoolArray);
                continue;
            }

            CurrentTypes.Add(EContractAbiTypes::Bool);
            continue;
        }

        if(OutputType.StartsWith(AbiType_String))
        {
            bContainsDynamic = true;
            if(OutputType.EndsWith("]"))
            {
                CurrentTypes.Add(EContractAbiTypes::StringArray);
                continue;
            }

            CurrentTypes.Add(EContractAbiTypes::String);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Bytes))
        {
            bContainsDynamic = true;
            if(OutputType.EndsWith("]"))
            {
                CurrentTypes.Add(EContractAbiTypes::BytesArray);
                continue;
            }

            CurrentTypes.Add(EContractAbiTypes::Bytes);
            continue;
        }

        // if(OutputType.StartsWith(AbiType_Int))
        // {
        //     if(OutputType.EndsWith("]"))
        //     {
        //         bContainsDynamic = true;
        //         CurrentTypes.Add(EContractAbiTypes::IntArray);
        //         continue;
        //     }
        //     CurrentTypes.Add(EContractAbiTypes::Int);
        //     continue;
        // }

        if(OutputType.StartsWith(AbiType_Tuple))
        {
            // if(OutputType.EndsWith("]"))
            // {
            //     CurrentTypes.Add(EContractAbiTypes::TupleArray);
            //     continue;
            // }

            CurrentTypes.Add(EContractAbiTypes::Tuple);
        }
    }
}

bool CTSBC_ContractAbi::EncodeAddress(
    FString Address,
    FString& EncodedArguments)
{
    Address = Address.TrimStartAndEnd();
    if(Address.StartsWith("0x"))
    {
        Address = Address.RightChop(2);
    }
    if(Address.Len() != EthereumAddressCharLength)
    {
        TSBC_LOG(Error, TEXT("%s is not an address"), *Address);
        return false;
    }

    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(Address, AbiSegmentCharLength));
    return true;
}

bool CTSBC_ContractAbi::EncodeUint(
    FString Uint,
    FString& EncodedArguments)
{
    Uint = Uint.TrimStartAndEnd();

    if(Uint.StartsWith("0x"))
    {
        Uint = Uint.RightChop(2);
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

bool CTSBC_ContractAbi::EncodeBool(
    const FString& Bool,
    FString& EncodedArguments)
{
    const FString DecHexValue = Bool.TrimStartAndEnd().ToBool() ? "1" : "0";
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    return true;
}

bool CTSBC_ContractAbi::EncodeStringDataLocation(
    const int& FunctionInputCount,
    const int& CurrentPadding,
    FString& EncodedArguments)
{
    const int32 DataPadding = (FunctionInputCount * AbiSegmentBytesLength) + (CurrentPadding * AbiSegmentBytesLength);
    FString DecHexValue = "";
    if(!CTSBC_BaseConverter::Dec2HexConverter().Convert(
        FString::FromInt(DataPadding),
        DecHexValue,
        1))
    {
        return false;
    }

    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    return true;
}

bool CTSBC_ContractAbi::EncodeString(
    const FString& String,
    FString& EncodedArguments)
{
    FString DecHexValue = "";
    CTSBC_BaseConverter::Dec2HexConverter().Convert(FString::FromInt(String.Len()), DecHexValue, 1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    const FString StringHex = TSBC_StringUtils::StringToHex(String, false);
    const float NumberOfSegments = FMath::CeilToInt(
        static_cast<float>(String.Len()) / AbiSegmentBytesLength);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadRight(StringHex, AbiSegmentCharLength * NumberOfSegments));
    return true;
}

bool CTSBC_ContractAbi::EncodeBytes(
    const FString& Bytes,
    FString& EncodedArguments)
{
    FString InBytes = Bytes;
    if(!TSBC_StringUtils::IsHexString(Bytes, true))
    {
        return false;
    }
    InBytes = InBytes.RightChop(2);
    FString DecHexValue = "";
    if(InBytes.Len() % 2 != 0)
    {
        InBytes = FString("0").Append(InBytes);
    }
    CTSBC_BaseConverter::Dec2HexConverter().Convert(FString::FromInt(InBytes.Len() / 2), DecHexValue, 1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
    // const FString StringHex = TSBC_StringUtils::StringToHex(Bytes, false);
    if(InBytes.Len() > AbiSegmentCharLength)
    {
        TSBC_LOG(Error, TEXT("Error encoding Bytes Argument with length %d"), InBytes.Len());
        return false;
    }
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadRight(InBytes, AbiSegmentCharLength));
    return true;
}

bool CTSBC_ContractAbi::IsFunctionTypeStaticArray(const FString& Type)
{
    return GetStaticArrayLength(Type) == 0;
}

int32 CTSBC_ContractAbi::GetStaticArrayLength(const FString& Type)
{
    int32 Bracket = 0;
    Type.FindChar(TEXT('['), Bracket);
    FString ArrayLength = Type.RightChop(Bracket);

    int32 FirstBracket = 0;
    int32 SecondBracket = 0;
    ArrayLength.FindChar(TEXT('['), FirstBracket);
    ArrayLength.RemoveAt(FirstBracket);
    ArrayLength.FindChar(TEXT(']'), SecondBracket);
    ArrayLength.RemoveAt(SecondBracket);

    return FCString::Atoi(*ArrayLength);
}

void CTSBC_ContractAbi::GetDynamicArrayLength(
    const FString& Argument,
    TArray<FString>& OutArray,
    FString& EncodedArguments)
{
    Argument.ParseIntoArray(OutArray, TEXT(","), true);
    FString DecHexValue = "";
    CTSBC_BaseConverter::Dec2HexConverter().Convert(FString::FromInt(OutArray.Num()), DecHexValue, 1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
}

bool CTSBC_ContractAbi::EncodeDynamicArrayData(
    const FString& Argument,
    const int32& InputCount,
    const int32& CurrentIndex,
    const FString& FunctionType,
    const TArray<EContractAbiTypes>& ArgumentsTypes,
    uint32& PaddingNextValue,
    FString& EncodedArguments)
{
    switch(ArgumentsTypes[CurrentIndex])
    {
    case EContractAbiTypes::AddressArray:
    case EContractAbiTypes::UintArray:
    case EContractAbiTypes::BoolArray:
    case EContractAbiTypes::StringArray:
    case EContractAbiTypes::BytesArray:
        // case EContractAbiTypes::IntArray:
        // case EContractAbiTypes::TupleArray:
        break;
    default:
        return false;
    }

    if(!IsFunctionTypeStaticArray(FunctionType))
    {
        return false;
    }

    uint32 DynamicArrayLength;
    EncodeDynamicArrayDataLocation(
        Argument,
        InputCount,
        PaddingNextValue,
        DynamicArrayLength,
        EncodedArguments);
    // padding by length of array (1) and number of elements in array
    PaddingNextValue = PaddingNextValue + DynamicArrayLength + 1;
    return true;
}

bool CTSBC_ContractAbi::EncodeStaticData(
    const FString& Argument,
    const int32& InputCount,
    const int32& CurrentIndex,
    const FString& FunctionType,
    const TArray<EContractAbiTypes>& ArgumentsTypes,
    uint32& PaddingNextValue,
    TArray<uint32>& DynamicTypesIndexes,
    FString& EncodedArguments)
{
    const uint32 StaticArrayLength = GetStaticArrayLength(FunctionType);
    FString ArgumentSanitized = Argument.TrimStartAndEnd();
    switch(ArgumentsTypes[CurrentIndex])
    {
    case EContractAbiTypes::Address:
        {
            if(!EncodeAddress(ArgumentSanitized, EncodedArguments))
            {
                return false;
            }

            break;
        }

    case EContractAbiTypes::Uint:
        {
            if(!EncodeUint(ArgumentSanitized, EncodedArguments))
            {
                return false;
            }

            break;
        }

    case EContractAbiTypes::Bool:
        {
            if(!EncodeBool(ArgumentSanitized, EncodedArguments))
            {
                return false;
            }

            break;
        }

    case EContractAbiTypes::String:
        {
            if(!EncodeStringDataLocation(InputCount, PaddingNextValue, EncodedArguments))
            {
                return false;
            }

            // calculate number of segments a string is going to use
            const float NumberOfSegments = FMath::CeilToInt(
                static_cast<float>(ArgumentSanitized.Len()) / AbiSegmentBytesLength);
            DynamicTypesIndexes.Add(CurrentIndex);
            // one for string length and one for it's value
            PaddingNextValue = PaddingNextValue + 1 + NumberOfSegments;
            break;
        }

    case EContractAbiTypes::Bytes:
        {
            const FString BytesCount = FunctionType.RightChop(AbiType_Bytes.Len());
            // Bytes type is Static and we need to encode in-place
            if(!BytesCount.IsEmpty())
            {
                int32 CharactersLength = FCString::Atoi(*BytesCount) * 2;
                // FString StringHex = TSBC_StringUtils::StringToHex(ArgumentSanitized);
                if(!TSBC_StringUtils::IsHexString(ArgumentSanitized, true))
                {
                    TSBC_LOG(Error, TEXT("Bytes argument `%s` should be in hex"), *ArgumentSanitized);
                    return false;
                }
                ArgumentSanitized.MidInline(2);
                if(CharactersLength != ArgumentSanitized.Len())
                {
                    TSBC_LOG(Error, TEXT("Bytes length %d should be %d"), CharactersLength, ArgumentSanitized.Len());
                    return false;
                }
                EncodedArguments.Append(TSBC_StringUtils::ZeroPadRight(ArgumentSanitized, AbiSegmentCharLength));
                break;
            }

            // Bytes type is dynamic and we need to point for data location
            if(!EncodeStringDataLocation(InputCount, PaddingNextValue, EncodedArguments))
            {
                return false;
            }

            DynamicTypesIndexes.Add(CurrentIndex);
            // one for string length and one for it's value
            PaddingNextValue = PaddingNextValue + 2;
            break;
        }

    // case EContractAbiTypes::Int:
    //     {
    //         if(!EncodeUint(ArgumentSanitized, EncodedArguments))
    //         {
    //             return false;
    //         }
    //         break;
    //     }

    case EContractAbiTypes::Tuple:
        {
            if(!EncodeStringDataLocation(InputCount, PaddingNextValue, EncodedArguments))
            {
                return false;
            }

            DynamicTypesIndexes.Add(CurrentIndex);
            PaddingNextValue = PaddingNextValue + 2;
            break;
        }

    case EContractAbiTypes::AddressArray:
        {
            // Address Array is Static and we need to encode in-place
            TArray<FString> OutArray;
            ArgumentSanitized.ParseIntoArray(OutArray, TEXT(","), true);
            if(StaticArrayLength != OutArray.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Address Array expecting %d but arguments are %d"),
                    StaticArrayLength,
                    OutArray.Num());
                return false;
            }

            for(const FString& Element : OutArray)
            {
                if(!EncodeAddress(Element, EncodedArguments))
                {
                    TSBC_LOG(Error, TEXT("Error encoding Address element %s"), *Element);
                    return false;
                }
            }

            break;
        }

    case EContractAbiTypes::UintArray:
        {
            // Uint Array is Static and we need to encode in-place
            TArray<FString> OutArray;
            ArgumentSanitized.ParseIntoArray(OutArray, TEXT(","), true);
            if(StaticArrayLength != OutArray.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Uint Array expecting %d but arguments are %d"),
                    StaticArrayLength,
                    OutArray.Num());
                return false;
            }

            for(const FString& Element : OutArray)
            {
                if(!EncodeUint(Element, EncodedArguments))
                {
                    TSBC_LOG(Error, TEXT("Error encoding Uint element %s"), *Element);
                    return false;
                }
            }

            break;
        }

    case EContractAbiTypes::BoolArray:
        {
            // Bool Array is Static and we need to encode in-place
            TArray<FString> OutArray;
            ArgumentSanitized.ParseIntoArray(OutArray, TEXT(","), true);
            if(StaticArrayLength != OutArray.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Bool Array expecting %d but arguments are %d"),
                    StaticArrayLength,
                    OutArray.Num());
                return false;
            }

            for(const FString& Element : OutArray)
            {
                if(!EncodeBool(Element, EncodedArguments))
                {
                    TSBC_LOG(Error, TEXT("Error encoding bool element %s"), *Element);
                    return false;
                }
            }

            break;
        }

    case EContractAbiTypes::StringArray:
        {
            // String Array is Static and we need to encode in-place
            TArray<FString> OutArray;
            ArgumentSanitized.ParseIntoArray(OutArray, TEXT(","), true);
            if(StaticArrayLength != OutArray.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("String Array expecting %d but arguments are %d"),
                    StaticArrayLength,
                    OutArray.Num());
                return false;
            }

            for(const FString& Element : OutArray)
            {
                FString ElementTemp = Element.TrimStartAndEnd();
                const FString StringHex = TSBC_StringUtils::StringToHex(ElementTemp, false);
                EncodedArguments.Append(TSBC_StringUtils::ZeroPadRight(StringHex, AbiSegmentCharLength));
            }
            break;
        }

    case EContractAbiTypes::BytesArray:
        {
            // Check if Bytes is expecting fixed number of bytes
            const FString NumOfBytesAndArrayLen = FunctionType.Mid(AbiType_Bytes.Len(), FunctionType.Len());
            int32 OpenBracketIndex = 0;
            if(!NumOfBytesAndArrayLen.FindChar(TEXT('['), OpenBracketIndex))
            {
                TSBC_LOG(
                    Error,
                    TEXT("Bytes Array syntax error"));
                return false;
            }
            // Bytes Array is Static and we need to encode in-place
            TArray<FString> OutArray;
            ArgumentSanitized.ParseIntoArray(OutArray, TEXT(","), true);
            if(StaticArrayLength != OutArray.Num())
            {
                TSBC_LOG(
                    Error,
                    TEXT("Bytes Array expecting %d but arguments are %d"),
                    StaticArrayLength,
                    OutArray.Num());
                return false;
            }

            for(const FString& Element : OutArray)
            {
                FString ElementTemp = Element.TrimStartAndEnd();
                if(!ElementTemp.StartsWith("0x"))
                {
                    TSBC_LOG(
                        Error,
                        TEXT("element %s is not hex value"),
                        *ElementTemp);
                    return false;
                }
                ElementTemp.MidInline(2);
                EncodedArguments.Append(TSBC_StringUtils::ZeroPadRight(ElementTemp, AbiSegmentCharLength));
            }
            break;
        }

    // case EContractAbiTypes::IntArray:
    //     {
    //         // Uint Array is Static and we need to encode in-place
    //         TArray<FString> OutArray;
    //         ArgumentSanitized.ParseIntoArray(OutArray, TEXT(","), true);
    //         if(StaticArrayLength != OutArray.Num())
    //         {
    //             TSBC_LOG(
    //
    //                 Error,
    //                 TEXT("Int Array expecting %d but arguments are %d"),
    //                 StaticArrayLength,
    //                 OutArray.Num());
    //             return false;
    //         }
    //         for(FString Element : OutArray)
    //         {
    //             Element = Element.TrimStartAndEnd();
    //             if(!EncodeUint(Element, EncodedArguments))
    //             {
    //                 TSBC_LOG(Error, TEXT("Error encoding Uint element %s"), *Element);
    //                 return false;
    //             }
    //         }
    //         break;
    //     }
    // case EContractAbiTypes::TupleArray:
    //     {
    //         TSBC_LOG(Warning, TEXT("Type is Tuple Array currently not supported"));
    //         break;
    //     }

    default:
        {
            checkNoEntry();
            break;
        }
    }

    return true;
}

void CTSBC_ContractAbi::EncodeDynamicArrayDataLocation(
    const FString& Argument,
    const int& FunctionInputCount,
    const int& CurrentPadding,
    uint32& ArrayLength,
    FString& EncodedArguments)
{
    TArray<FString> OutArray;
    Argument.ParseIntoArray(OutArray, TEXT(","), true);
    const int32 DataPadding = (FunctionInputCount * AbiSegmentBytesLength) +
                              (CurrentPadding * AbiSegmentBytesLength);
    ArrayLength = OutArray.Num();
    FString DecHexValue = "";
    CTSBC_BaseConverter::Dec2HexConverter().Convert(
        FString::FromInt(DataPadding),
        DecHexValue,
        1);
    EncodedArguments.Append(TSBC_StringUtils::ZeroPadLeft(DecHexValue, AbiSegmentCharLength));
}

void CTSBC_ContractAbi::DecodeDataRegardingTypes(
    const TArray<EContractAbiTypes>& ArgumentsTypes,
    const bool bIsDynamic,
    const TArray<FTSBC_ContractAbiFunctionInput>& FunctionOutputs,
    const FString& DataToDecode,
    TArray<FString>& DecodedValues)
{
    FString RemainingDataToDecode = DataToDecode;
    if(bIsDynamic)
    {
        // MainData = MainData.Mid(AbiSegmentCharLength, MainData.Len());
        RemainingDataToDecode = RemainingDataToDecode.Mid(AbiSegmentCharLength, DataToDecode.Len());
    }

    FString RemainingResult;
    for(int32 ArgumentTypeIndex = 0; ArgumentTypeIndex < ArgumentsTypes.Num(); ArgumentTypeIndex++)
    {
        int32 SegmentsProcessed = 1;
        // Check if this is not the first value that we are getting
        if(!RemainingResult.IsEmpty())
        {
            RemainingDataToDecode = RemainingResult;
        }

        switch(ArgumentsTypes[ArgumentTypeIndex])
        {
        case EContractAbiTypes::Address:
            {
                FString DecodedValue;
                DecodeAddress(RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(DecodedValue);
                break;
            }
        case EContractAbiTypes::AddressArray:
            {
                int32 ArrayDataLocation;
                int32 ArrayLength;
                GetArrayData(
                    DataToDecode,
                    RemainingDataToDecode,
                    ArrayDataLocation,
                    ArrayLength);
                FString ArrayData = DataToDecode.Mid(ArrayDataLocation + AbiSegmentCharLength);
                // chop to array data location then chop the array length data
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeAddress(ArrayData, DecodedValue);
                    // Add the value to the returned array
                    DecodedValues.Add(DecodedValue);
                    ArrayData.MidInline(AbiSegmentCharLength);
                }
                break;
            }
        case EContractAbiTypes::Uint:
            {
                FString DecodedValue;
                DecodeUint(RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(DecodedValue);
                break;
            }
        case EContractAbiTypes::UintArray:
            {
                int32 ArrayDataLocation;
                int32 ArrayLength;
                GetArrayData(
                    DataToDecode,
                    RemainingDataToDecode,
                    ArrayDataLocation,
                    ArrayLength);
                FString ArrayData = DataToDecode.Mid(ArrayDataLocation + AbiSegmentCharLength);
                // chop to array data location then chop the array length data
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeUint(ArrayData, DecodedValue);
                    // Add the value to the returned array
                    DecodedValues.Add(DecodedValue);
                    ArrayData.MidInline(AbiSegmentCharLength);
                }
                break;
            }
        case EContractAbiTypes::Bool:
            {
                FString DecodedValue;
                DecodeBool(RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(DecodedValue);
                break;
            }
        case EContractAbiTypes::BoolArray:
            {
                int32 ArrayDataLocation;
                int32 ArrayLength;
                GetArrayData(
                    DataToDecode,
                    RemainingDataToDecode,
                    ArrayDataLocation,
                    ArrayLength);
                FString ArrayData = DataToDecode.Mid(ArrayDataLocation + AbiSegmentCharLength);
                // chop to array data location then chop the array length data
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeBool(ArrayData, DecodedValue);
                    // Add the value to the returned array
                    DecodedValues.Add(DecodedValue);
                    ArrayData.MidInline(AbiSegmentCharLength);
                }
                break;
            }
        case EContractAbiTypes::String:
            {
                FString DecodedValue;
                DecodeString(
                    DataToDecode,
                    RemainingDataToDecode,
                    SegmentsProcessed,
                    DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(DecodedValue);
                break;
            }
        case EContractAbiTypes::StringArray:
            {
                int32 ArrayDataLocation;
                int32 ArrayLength;
                GetArrayData(
                    DataToDecode,
                    RemainingDataToDecode,
                    ArrayDataLocation,
                    ArrayLength);
                FString RemainingArrayData = DataToDecode.Mid(ArrayDataLocation + AbiSegmentCharLength);
                // chop to array data location then chop the array length data
                FString ArrayData = RemainingArrayData;
                int32 SegmentsProcessedInArray = 0;
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeString(ArrayData, RemainingArrayData, SegmentsProcessedInArray, DecodedValue);
                    DecodedValues.Add(DecodedValue);
                    RemainingArrayData.MidInline(AbiSegmentCharLength);
                }

                break;
            }
        case EContractAbiTypes::Bytes:
            {
                FString DecodedValue;
                DecodeBytes(DataToDecode, RemainingDataToDecode, DecodedValue);
                // Add the value to the returned array
                DecodedValues.Add(DecodedValue);
                break;
            }
        case EContractAbiTypes::BytesArray:
            {
                int32 ArrayDataLocation;
                int32 ArrayLength;
                GetArrayData(
                    DataToDecode,
                    RemainingDataToDecode,
                    ArrayDataLocation,
                    ArrayLength);
                FString RemainingArrayData = DataToDecode.Mid(ArrayDataLocation + AbiSegmentCharLength);
                // chop to array data location then chop the array length data
                FString ArrayData = RemainingArrayData;
                for(int32 ArrayIndex = 0; ArrayIndex < ArrayLength; ArrayIndex++)
                {
                    FString DecodedValue;
                    DecodeBytes(ArrayData, RemainingArrayData, DecodedValue);
                    // Add the value to the returned array
                    DecodedValues.Add(DecodedValue);
                    RemainingArrayData.MidInline(AbiSegmentCharLength);
                }
                break;
            }
        // case EContractAbiTypes::Int:
        //     {
        //         FString DecodedValue;
        //         DecodeUint(DataToDecode, DecodedValue);
        //
        //         // Add the value to the returned array
        //         DecodedAbiElements.Add(DecodedValue);
        //         break;
        //     }
        // case EContractAbiTypes::IntArray:
        //     {
        //         int32 ArrayDataLocation;
        //         int32 ArrayLength;
        //         GetArrayData(
        //             DataToDecode,
        //             MainData,
        //             ArrayDataLocation,
        //             ArrayLength);
        //         FString DataLocation = DataToDecode;
        //         DataLocation.MidInline(ArrayDataLocation + AbiSegmentCharLength);
        //         // chop to array data location then chop the array length data
        //         for(int32 j = 0; j < ArrayLength; j++)
        //         {
        //             FString DecodedValue;
        //             DecodeUint(DataLocation, DecodedValue);
        //             // Add the value to the returned array
        //             DecodedAbiElements.Add(DecodedValue);
        //             DataLocation.MidInline(AbiSegmentCharLength);
        //         }
        //         break;
        //     }
        case EContractAbiTypes::Tuple:
            {
                TArray<EContractAbiTypes> TupleTypes;
                bool bContainsDynamic;
                GetTupleTypes(FunctionOutputs[ArgumentTypeIndex].Components, bContainsDynamic, TupleTypes);
                // remove the first segment since it's tuple location
                DecodeDataRegardingTypes(
                    TupleTypes,
                    bContainsDynamic,
                    FunctionOutputs,
                    DataToDecode,
                    DecodedValues);
                break;
            }
        // case EContractAbiTypes::TupleArray:
        //     {
        //         TSBC_LOG(Warning, TEXT("Type is Tuple Array currently not supported"));
        //         break;
        //     }

        default:
            {
                checkNoEntry();
                break;
            }
        }
        // go to next data segment
        RemainingResult = RemainingDataToDecode.Mid(AbiSegmentCharLength * SegmentsProcessed);
    }
}

void CTSBC_ContractAbi::DecodeAddress(
    const FString& RemainingDataToDecode,
    FString& DecodedValue)
{
    DecodedValue = FString("0x").Append(
        RemainingDataToDecode.Mid(
            AbiSegmentCharLength - EthereumAddressCharLength,
            EthereumAddressCharLength));
}

void CTSBC_ContractAbi::DecodeUint(
    const FString& RemainingDataToDecode,
    FString& DecodedValue)
{
    // Get the Uint data
    const FString UintHexData = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    // Convert String hex value to Decimal
    CTSBC_BaseConverter::Hex2DecConverter().Convert(UintHexData, DecodedValue, 1);
}

void CTSBC_ContractAbi::DecodeBool(
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

void CTSBC_ContractAbi::DecodeString(
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
    DecodedValue = TSBC_StringUtils::BytesToStringUtf8(
        TSBC_StringUtils::HexStringToBytes(StringValueAsHex));
}

void CTSBC_ContractAbi::DecodeBytes(
    const FString& DataToDecode,
    const FString& RemainingDataToDecode,
    FString& DecodedValue)
{
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
    DecodedValue = FString("0x").Append(TSBC_StringUtils::StripTrailingZeroes(BytesValue));
}

void CTSBC_ContractAbi::GetArrayData(
    const FString& DataToDecode,
    const FString& RemainingDataToDecode,
    int32& ArrayDataLocation,
    int32& ArrayLength)
{
    // Get the location of where Array data is being stored
    const FString ArrayDataLocationHex = RemainingDataToDecode.Mid(0, AbiSegmentCharLength);
    FString DataLocation;
    CTSBC_BaseConverter::Hex2DecConverter().Convert(ArrayDataLocationHex, DataLocation, 1);
    const int32 DataLocationInt = FCString::Atoi(*DataLocation);
    ArrayDataLocation = DataLocationInt * 2;
    // Get the length of the Array (how many elements).
    const FString ArrayLengthHex = DataToDecode.Mid(ArrayDataLocation, AbiSegmentCharLength);
    FString ArrayLengthDecimal;
    CTSBC_BaseConverter::Hex2DecConverter().Convert(ArrayLengthHex, ArrayLengthDecimal, 1);
    ArrayLength = FCString::Atoi(*ArrayLengthDecimal);
}