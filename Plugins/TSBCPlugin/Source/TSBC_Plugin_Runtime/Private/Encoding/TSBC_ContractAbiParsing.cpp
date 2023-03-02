// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_ContractAbiParsing.h"
#include "Module/TSBC_RuntimeLogCategories.h"
#include "Util/TSBC_TypeUtils.h"

const FString CTSBC_ContractAbiParsing::JsonKey_Anonymous = "anonymous";
const FString CTSBC_ContractAbiParsing::JsonKey_Components = "components";
const FString CTSBC_ContractAbiParsing::JsonKey_Indexed = "indexed";
const FString CTSBC_ContractAbiParsing::JsonKey_Inputs = "inputs";
const FString CTSBC_ContractAbiParsing::JsonKey_Name = "name";
const FString CTSBC_ContractAbiParsing::JsonKey_Outputs = "outputs";
const FString CTSBC_ContractAbiParsing::JsonKey_StateMutability = "stateMutability";
const FString CTSBC_ContractAbiParsing::JsonKey_Type = "type";
// NOTE: Unused for now
// const FString CTSBC_ContractAbiParsing::JsonKey_StateMutabilityNonPayable = "nonpayable";
// const FString CTSBC_ContractAbiParsing::JsonKey_StateMutabilityPure = "pure";
// const FString CTSBC_ContractAbiParsing::JsonKey_StateMutabilityPayable = "payable";
// const FString CTSBC_ContractAbiParsing::JsonKey_StateMutabilityView = "view";

void CTSBC_ContractAbiParsing::ParseAbiFromJson(
    const FString& ContractAbiJson,
    bool& bSuccess,
    FString& ErrorMessage,
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

    for(const TSharedPtr<FJsonValue>& JsonValueElement : *JsonArray)
    {
        const TSharedPtr<FJsonObject>* JsonObject;
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
        const UEnum* EnumPtr = TSBC_TypeUtils::FindEnumSafe<ETSBC_ContractAbiFunctionType>(nullptr, true);
        if(EnumPtr == nullptr)
        {
            ErrorMessage = "Invalid Enum Pointer";
            TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
            return;
        }

        const ETSBC_ContractAbiFunctionType Index =
            static_cast<ETSBC_ContractAbiFunctionType>(EnumPtr->GetIndexByName(TypeConvertedToName));
        switch(Index)
        {
        case ETSBC_ContractAbiFunctionType::Constructor:
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

        case ETSBC_ContractAbiFunctionType::Error:
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

        case ETSBC_ContractAbiFunctionType::Event:
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

        case ETSBC_ContractAbiFunctionType::Function:
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

        case ETSBC_ContractAbiFunctionType::Fallback:
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

        case ETSBC_ContractAbiFunctionType::Receive:
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

bool CTSBC_ContractAbiParsing::GetFunctionsFromAbi(
    const TSharedPtr<FJsonObject> JsonObject,
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

    TArray<FTSBC_SolidityFunctionSignature> FunctionInput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Inputs,
        ErrorMessage,
        FunctionInput))
    {
        FunctionStruct.Inputs = FunctionInput;
    }

    TArray<FTSBC_SolidityFunctionSignature> FunctionOutput;
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

bool CTSBC_ContractAbiParsing::GetReceiveFromAbi(
    const TSharedPtr<FJsonObject> JsonObject,
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

    TArray<FTSBC_SolidityFunctionSignature> FunctionInput;
    if(GetFunctionInputOrOutput(
        JsonObject,
        JsonKey_Inputs,
        ErrorMessage,
        FunctionInput))
    {
        ReceiveStruct.Inputs = FunctionInput;
    }

    TArray<FTSBC_SolidityFunctionSignature> FunctionOutput;
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

bool CTSBC_ContractAbiParsing::GetConstructorFromAbi(
    const TSharedPtr<FJsonObject> JsonObject,
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

    TArray<FTSBC_SolidityFunctionSignature> FunctionInput;
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

bool CTSBC_ContractAbiParsing::GetFallbacksFromAbi(
    const TSharedPtr<FJsonObject> JsonObject,
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

bool CTSBC_ContractAbiParsing::GetEventsFromAbi(
    const TSharedPtr<FJsonObject> JsonObject,
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

    TArray<FTSBC_SolidityFunctionSignature> FunctionInput;
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

bool CTSBC_ContractAbiParsing::GetErrorsFromAbi(
    const TSharedPtr<FJsonObject> JsonObject,
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

    TArray<FTSBC_SolidityFunctionSignature> FunctionInput;
    if(GetFunctionInputOrOutput(JsonObject, JsonKey_Inputs, ErrorMessage, FunctionInput))
    {
        ErrorStruct.Inputs = FunctionInput;
    }

    return true;
}

bool CTSBC_ContractAbiParsing::GetFunctionInputOrOutput(
    const TSharedPtr<FJsonObject> JsonObject,
    const FString& FieldName,
    FString& ErrorMessage,
    TArray<FTSBC_SolidityFunctionSignature>& FunctionSignatures)
{
    const TArray<TSharedPtr<FJsonValue>>* FunctionInputs;
    if(JsonObject->TryGetArrayField(FieldName, FunctionInputs))
    {
        for(const auto& Input : *FunctionInputs)
        {
            FTSBC_SolidityFunctionSignature LocalFunctionInput;
            const TSharedPtr<FJsonObject>* InputObject;
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
            LocalFunctionInput.Variable.Type = InputType;
            LocalFunctionInput.Variable.Name = InputName;

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
                    FTSBC_SolidityVariable LocalComponentInputStruct;
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
                    LocalFunctionInput.TupleVariables.Add(LocalComponentInputStruct);
                }
            }
            FunctionSignatures.Add(LocalFunctionInput);
        }
    }
    return true;
}