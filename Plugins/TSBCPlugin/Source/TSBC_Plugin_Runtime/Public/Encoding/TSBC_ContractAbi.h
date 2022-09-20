// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_ContractAbiTypes.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
// =============================================================================

/**
 * This class implements Ethereum Smart Contract ABI helper functions.
 *
 * Features:
 * - Parse ABI from JSON
 * - Encode ABI
 * - Decode ABI
 * - Supported types:
 *   - Address
 *   - Address[]
 *   - String
 *   - String[]
 *   - Uint
 *   - Uint[]
 *   - Bool
 *   - Bool[]
 *   - Bytes
 *   - Bytes[]
 *   - Tuple
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_ContractAbi
{
private:
    TArray<FString> Constructors;
    TArray<FString> Events;
    TArray<FString> Functions;

public:
    /**
     * Tries to parse an Ethereum Smart Contract ABI from the given JSON.
     * 
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbiJson The Ethereum Smart Contract ABI in JSON format to parse.
     * @param ContractAbi The Contract ABI.
     */
    static void ParseAbiFromJson(
        bool& bSuccess,
        FString& ErrorMessage,
        const FString& ContractAbiJson,
        FTSBC_ContractAbi& ContractAbi);

    /**
     * Generates a "Function Selector" with argument encoding for a specific function of an Ethereum Smart Contract.
     * 
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbi The Contract ABI.
     * @param FunctionName The function name to hash.
     * @param FunctionArguments Function arguments to encode.
     * @param FunctionSelectorAndEncodedArguments The "Function Selector" with encoded arguments.
     */
    static void EncodeAbi(
        bool& bSuccess,
        FString& ErrorMessage,
        const FTSBC_ContractAbi& ContractAbi,
        const FString& FunctionName,
        const TArray<FString>& FunctionArguments,
        FString& FunctionSelectorAndEncodedArguments);

    /**
     * Decodes the resulting data returned from an Ethereum RPC-call.
     *
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbi The Contract ABI.
     * @param FunctionName The function name that was used to call the Ethereum Smart Contract function which returned
     *                     the data that should now be decoded here.
     * @param DataToDecode The data to decode.
     * @param DecodedValues Array of the decoded values from the data.
     */
    static void DecodeAbi(
        bool& bSuccess,
        FString& ErrorMessage,
        const FTSBC_ContractAbi& ContractAbi,
        const FString& FunctionName,
        const FString& DataToDecode,
        TArray<FString>& DecodedValues);

private:
    /**
     * Gets all "Functions" from the Contract ABI JSON.
     */
    static bool GetFunctionsFromAbi(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& FunctionStruct);

    /**
     * Gets all "Receive" from the Contract ABI JSON.
     */
    static bool GetReceiveFromAbi(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& ReceiveStruct);


    /**
     * Gets the "Constructor" from the Contract ABI JSON.
     */
    static bool GetConstructorFromAbi(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& ConstructorStruct);

    /**
     * Gets all "Fallbacks" from the Contract ABI JSON.
     */
    static bool GetFallbacksFromAbi(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiFunction& FallbackStruct);

    /**
     * Gets all "Events" from the Contract ABI JSON.
     */
    static bool GetEventsFromAbi(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiEvent& EventStruct);

    /**
     * Gets all "Errors" from the Contract ABI JSON.
     */
    static bool GetErrorsFromAbi(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        FString& ErrorMessage,
        FTSBC_ContractAbiError& ErrorStruct);

    /**
     * Gets the function input or output types from JSON.
     */
    static bool GetFunctionInputOrOutput(
        const TSharedPtr<FJsonObject, ESPMode::Fast> JsonObject,
        const FString FieldName,
        FString& ErrorMessage,
        TArray<FTSBC_ContractAbiFunctionInput>& FunctionInput);

    /**
     * Check if a function is available in generated Contract Abi.
     */
    static bool IsFunctionAvailable(
        const FTSBC_ContractAbi& ContractAbi,
        const FString& FunctionName,
        FTSBC_ContractAbiFunction& FunctionRef);

    /**
     * Encode arguments for the given function inputs.
     */
    static bool EncodeArguments(
        const TArray<FTSBC_ContractAbiFunctionInput>& FunctionInputTypes,
        const TArray<FString>& Arguments,
        FString& EncodedArguments);

    /**
     * Get the types of all arguments for this specific function regarding its inputs.
     */
    static void GetArgumentsTypes(
        TArray<FTSBC_ContractAbiFunctionInput> FunctionInputTypes,
        TArray<EContractAbiTypes>& ArgumentsTypes);

    /**
     * Loop on current types and encoded in-place static types and make head for dynamic types
     */
    static bool EncodeStaticTypes(
        const TArray<FTSBC_ContractAbiFunctionInput> FunctionInputTypes,
        bool bEncodingTuple,
        const uint32 TupleIndex,
        const TArray<EContractAbiTypes>& ArgumentsTypes,
        const TArray<FString>& Arguments,
        TArray<uint32>& DynamicTypesIndexes,
        FString& EncodedArguments);

    /**
     * Loop on dynamic types to encode at a separately allocated location (tails)
     */
    static bool EncodeDynamicTypes(
        const TArray<FTSBC_ContractAbiFunctionInput> InTypes,
        const TArray<EContractAbiTypes>& CurrentTypes,
        const TArray<FString>& Arguments,
        TArray<uint32>& DynamicTypesIndexes,
        FString& EncodedArguments);

    /**
     * Gets the types inside the tuple.
     */
    static void GetTupleTypes(
        TArray<FTSBC_ContractAbiInputComponent> InTypes,
        bool& bContainsDynamic,
        TArray<EContractAbiTypes>& CurrentTypes);

    /**
     * Encode Address.
     */
    static bool EncodeAddress(FString Address, FString& EncodedArguments);

    /**
     * Encode Uint.
     */
    static bool EncodeUint(FString Uint, FString& EncodedArguments);

    /**
     * Encode Bool.
     */
    static bool EncodeBool(const FString& Bool, FString& EncodedArguments);

    /**
     * Encode the data location of the string where it's going to be stored at.
     */
    static bool EncodeStringDataLocation(
        const int& FunctionInputCount,
        const int& CurrentPadding,
        FString& EncodedArguments);

    /**
     * Encode String.
     */
    static bool EncodeString(const FString& String, FString& EncodedArguments);

    /**
     * Encode Bytes.
     */
    static bool EncodeBytes(const FString& Bytes, FString& EncodedArguments);

    /**
     * Returns true if the function type is a static array.
     */
    static bool IsFunctionTypeStaticArray(const FString& Type);

    /**
     * Gets the length of the static array if found.
     */
    static int32 GetStaticArrayLength(const FString& Type);

    /**
     * Gets the length of the dynamic array.
     */
    static void GetDynamicArrayLength(const FString& Argument, TArray<FString>& OutArray, FString& EncodedArguments);

    /**
     * Encode dynamic array data.
     */
    static bool EncodeDynamicArrayData(
        const FString& Argument,
        const int32& InputCount,
        const int32& CurrentIndex,
        const FString& FunctionType,
        const TArray<EContractAbiTypes>& ArgumentsTypes,
        uint32& PaddingNextValue,
        FString& EncodedArguments);

    /**
     * Encode static data.
     */
    static bool EncodeStaticData(
        const FString& Argument,
        const int32& InputCount,
        const int32& CurrentIndex,
        const FString& FunctionType,
        const TArray<EContractAbiTypes>& ArgumentsTypes,
        uint32& PaddingNextValue,
        TArray<uint32>& DynamicTypesIndexes,
        FString& EncodedArguments);

    /**
     * Encode the data location of the dynamic array where it's going to be stored at.
     */
    static void EncodeDynamicArrayDataLocation(
        const FString& Argument,
        const int& FunctionInputCount,
        const int& CurrentPadding,
        uint32& ArrayLength,
        FString& EncodedArguments);

    /**
     * Decode data from an Ethereum function response callback.
     */
    static void DecodeDataRegardingTypes(
        const TArray<EContractAbiTypes>& ArgumentsTypes,
        const bool bIsDynamic,
        const TArray<FTSBC_ContractAbiFunctionInput>& FunctionOutputs,
        const FString& DataToDecode,
        TArray<FString>& DecodedValues);

    /**
     * Decode Address.
     */
    static void DecodeAddress(const FString& RemainingDataToDecode, FString& DecodedValue);

    /**
     * Decode Uint.
     */
    static void DecodeUint(const FString& RemainingDataToDecode, FString& DecodedValue);

    /**
     * Decode Bool.
     */
    static void DecodeBool(const FString& RemainingDataToDecode, FString& DecodedValue);

    /**
     * Decode String.
     */
    static void DecodeString(
        const FString& DataToDecode,
        const FString& RemainingDataToDecode,
        int32& SegmentsProcessed,
        FString& DecodedValue);

    /**
     * Decode Bytes.
     */
    static void DecodeBytes(
        const FString& DataToDecode,
        const FString& RemainingDataToDecode,
        FString& DecodedValue);

    /**
     * Gets the array data (length and data location) to decode it.
     */
    static void GetArrayData(
        const FString& DataToDecode,
        const FString& RemainingDataToDecode,
        int32& ArrayDataLocation,
        int32& ArrayLength);
};