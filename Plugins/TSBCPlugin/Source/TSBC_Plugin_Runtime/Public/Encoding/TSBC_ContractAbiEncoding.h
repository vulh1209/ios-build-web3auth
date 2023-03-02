// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_ContractAbiTypes.h"

/**
 * This class implements Ethereum Smart Contract ABI helper functions.
 *
 * Features:
 * - Encode ABI
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

class TSBC_PLUGIN_RUNTIME_API CTSBC_ContractAbiEncoding
{
public:
    /**
     * Generates a "Function Selector" with argument encoding for a specific function of an Ethereum Smart Contract.
     * 
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbi The Contract ABI.
     * @param FunctionName The function name to hash.
     * @param Arguments Function arguments to encode.
     * @param FunctionSelectorAndEncodedArguments The "Function Selector" with encoded arguments.
     */
    static void EncodeAbi(
        bool& bSuccess,
        FString& ErrorMessage,
        const FTSBC_ContractAbi& ContractAbi,
        const FString& FunctionName,
        const TArray<FTSBC_SolidityValueList>& Arguments,
        FString& FunctionSelectorAndEncodedArguments);

private:
    /**
     * Gets the length of the dynamic array.
     */
    static void GetDynamicArrayLength(const FTSBC_SolidityValueList& Arguments, FString& EncodedArguments);

    /**
     * Encode the data location of the dynamic data where it's going to be stored at.
     */
    static void EncodeDynamicDataLocation(
        const int& ParametersCount,
        const int& CurrentPointer,
        FString& EncodedArguments);

    /**
     * Encode arguments for the given function inputs.
     */
    static bool EncodeArguments(
        FTSBC_ContractAbiEncodeContext& EncodeContext,
        uint32& DataSegmentsUsed,
        FString& EncodedArguments);

    /**
     * Encodes a single argument for the given function inputs.
     */
    static bool EncodeArgument(
        FTSBC_ContractAbiEncodeContext& EncodeContext,
        bool& bEncodeInPlace,
        uint32& DataSegmentsUsed,
        FString& EncodedHeaderSection,
        FString& EncodedBodySection);

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
     * Encode String.
     */
    static bool EncodeString(const FString& String, uint32& DataSegmentsUsed, FString& EncodedArguments);

    /**
     * Encode Bytes.
     */
    static bool EncodeBytes(
        const FString& ParameterType,
        const FString& Bytes,
        bool& bStatic,
        FString& EncodedArgument);

};