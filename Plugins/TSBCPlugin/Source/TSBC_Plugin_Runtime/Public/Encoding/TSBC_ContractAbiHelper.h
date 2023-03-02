// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

#include "Data/TSBC_ContractAbiTypes.h"

const FString static AbiType_Address = "address";
const FString static AbiType_String = "string";
const FString static AbiType_Bool = "bool";
const FString static AbiType_Uint = "uint";
const FString static AbiType_Int = "int";
const FString static AbiType_Bytes = "bytes";
const FString static AbiType_Tuple = "tuple";

constexpr int32 static AbiSegmentBytesLength = 32;
constexpr int32 static AbiSegmentCharLength = 64;
constexpr int32 static EthereumAddressCharLength = 40;

/**
 * Helper class for contract abi encoding and decoding functionality.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_ContractAbiHelper
{
public:
    /**
     * Gets the Solidity data types from a list of Solidity function signatures.
     *
     * @param FunctionSignatures List of Solidity function signatures.
     * @param SolidityDataTypes returns a list of Solidity data types.
     * @param bContainsTuple true if data types contains at least 1 tuple.
     */
    static void GetSolidityDataTypes(
        const TArray<FTSBC_SolidityFunctionSignature>& FunctionSignatures,
        TArray<ETSBC_SolidityDataType>& SolidityDataTypes,
        bool& bContainsTuple);

    /**
     * Check if a function is available in generated Contract Abi.
     */
    static bool IsFunctionAvailable(
        const FTSBC_ContractAbi& ContractAbi,
        const FString& FunctionName,
        FTSBC_ContractAbiFunction& FunctionRef);

    /**
     * Gets the Solidity data types from Solidity tuple variables.
     */
    static void GetSolidityDataTypesFromTuple(
        TArray<FTSBC_SolidityVariable> InSolidityTupleVariables,
        bool& bOutContainsDynamic,
        TArray<ETSBC_SolidityDataType>& OutSolidityDataTypes);

    /**
     * Gets the length of the static array if found.
     */
    static int32 GetStaticArrayLength(const FString& Type);

};