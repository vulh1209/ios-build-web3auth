// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_ContractAbiTypes.h"

/**
 * This class implements Ethereum Smart Contract ABI helper functions.
 *
 * Features:
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

class TSBC_PLUGIN_RUNTIME_API CTSBC_ContractAbiDecoding
{
public:
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
        TArray<FTSBC_SolidityValueList>& DecodedValues);

private:
    /**
     * Gets the array data (length and data location) to decode it.
     */
    static void GetArrayData(
        const FString& FunctionParamType,
        const FString& DataToDecode,
        const FString& RemainingDataToDecode,
        bool& bArrayStatic,
        int32& ArrayDataLocation,
        int32& ArrayLength);

    /**
     * Decode data from an Ethereum function response callback.
     */
    static void DecodeDataRegardingTypes(
        const TArray<ETSBC_SolidityDataType>& SolidityDataTypes,
        const TArray<FTSBC_SolidityFunctionSignature>& FunctionOutputs,
        const FString& DataToDecode,
        const bool& bDecodingTuple,
        const uint32& TupleParameterIndex,
        TArray<FTSBC_SolidityValueList>& DecodedValues);

    /**
     * Decode Address.
     */
    static void DecodeAddress(const FString& DataToDecode, FString& DecodedValue);

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
        const FString& FunctionParamType,
        const FString& DataToDecode,
        const FString& RemainingDataToDecode,
        FString& DecodedValue);

};