// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
// =============================================================================
#include "Data/TSBC_ContractAbiTypes.h"

/**
 * This class allows to parse Smart Contract ABI data from JSON.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_ContractAbiParsing
{
public:
    static const FString JsonKey_Anonymous;
    static const FString JsonKey_Components;
    static const FString JsonKey_Indexed;
    static const FString JsonKey_Inputs;
    static const FString JsonKey_Name;
    static const FString JsonKey_Outputs;
    static const FString JsonKey_StateMutability;
    static const FString JsonKey_Type;
    // NOTE: Unused for now
    // static const FString JsonKey_StateMutabilityNonPayable;
    // static const FString JsonKey_StateMutabilityPure;
    // static const FString JsonKey_StateMutabilityPayable;
    // static const FString JsonKey_StateMutabilityView;

private:
    TArray<FString> Constructors;
    TArray<FString> Events;
    TArray<FString> Functions;

public:
    /**
     * Tries to parse an Ethereum Smart Contract ABI from the given JSON.
     * 
     * @param ContractAbiJson The Ethereum Smart Contract ABI in JSON format to parse.
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbi The Contract ABI.
     */
    static void ParseAbiFromJson(
        const FString& ContractAbiJson,
        bool& bSuccess,
        FString& ErrorMessage,
        FTSBC_ContractAbi& ContractAbi);

private:
    /**
     * Gets all "Functions" from the Contract ABI JSON.
     */
    static bool GetFunctionsFromAbi(
        const TSharedPtr<FJsonObject> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& FunctionStruct);

    /**
     * Gets all "Receive" from the Contract ABI JSON.
     */
    static bool GetReceiveFromAbi(
        const TSharedPtr<FJsonObject> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& ReceiveStruct);

    /**
     * Gets the "Constructor" from the Contract ABI JSON.
     */
    static bool GetConstructorFromAbi(
        const TSharedPtr<FJsonObject> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& ConstructorStruct);

    /**
     * Gets all "Fallbacks" from the Contract ABI JSON.
     */
    static bool GetFallbacksFromAbi(
        const TSharedPtr<FJsonObject> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& FallbackStruct);

    /**
     * Gets all "Events" from the Contract ABI JSON.
     */
    static bool GetEventsFromAbi(
        const TSharedPtr<FJsonObject> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiEvent& EventStruct);

    /**
     * Gets all "Errors" from the Contract ABI JSON.
     */
    static bool GetErrorsFromAbi(
        const TSharedPtr<FJsonObject> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiError& ErrorStruct);

    /**
     * Gets the function input or output types from JSON.
     */
    static bool GetFunctionInputOrOutput(
        const TSharedPtr<FJsonObject> JsonObject,
        const FString& FieldName,
        FString& ErrorMessage,
        TArray<FTSBC_SolidityFunctionSignature>& FunctionSignatures);
};