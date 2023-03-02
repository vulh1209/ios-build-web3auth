// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#include "TSBC_ContractAbiTypes.generated.h"

/**
 * Data types used in Solidity.
 *
 * NOTE: Not all types have been implemented.
 */
UENUM()
enum class ETSBC_SolidityDataType : uint8
{
    // @formatter:off
    Address       UMETA(DisplayName = "address"),
    AddressArray  UMETA(DisplayName = "addressArray"),
    String        UMETA(DisplayName = "string"),
    StringArray   UMETA(DisplayName = "stringArray"),
    Bool          UMETA(DisplayName = "bool"),
    BoolArray     UMETA(DisplayName = "boolArray"),
    Uint          UMETA(DisplayName = "uint"),
    UintArray     UMETA(DisplayName = "uintArray"),
    Bytes         UMETA(DisplayName = "bytes"),
    BytesArray    UMETA(DisplayName = "bytesArray"),
    Tuple         UMETA(DisplayName = "tuple"),
    // @formatter:on
};

/**
 * Function Types inside Contract ABI.
 */
UENUM(BlueprintType, DisplayName="Contract ABI Function Types")
enum class ETSBC_ContractAbiFunctionType : uint8
{
    // @formatter:off
    Constructor  UMETA(DisplayName = "constructor"),
    Error        UMETA(DisplayName = "error"),
    Event        UMETA(DisplayName = "event"),
    Function     UMETA(DisplayName = "function"),
    Receive      UMETA(DisplayName = "receive"),
    Fallback     UMETA(DisplayName = "fallback"),
    // @formatter:on
};

/**
 * Struct that holds encoded data for an argument.
 */
USTRUCT()
struct FTSBC_EncodedArgumentData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 SegmentsCount = 0;

    UPROPERTY()
    bool bEncodedInPlace = false;

    UPROPERTY()
    FString HeaderData = "";

    FTSBC_EncodedArgumentData()
    {
    }

    FTSBC_EncodedArgumentData(const int32 InSegmentCount, const bool InEncodedInPlace, const FString InStaticData)
    {
        SegmentsCount = InSegmentCount;
        bEncodedInPlace = InEncodedInPlace;
        HeaderData = InStaticData;
    }
};

/**
 * List of values (as inputs or outputs) of a Solidity function.
 */
USTRUCT(BlueprintType, DisplayName="Value List")
struct FTSBC_SolidityValueList
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FString> Values;

    FTSBC_SolidityValueList()
    {
    }

    FTSBC_SolidityValueList(const TArray<FString> InValues)
    {
        Values = InValues;
    }
};

/**
 * Name and Type of a Solidity variable.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Input Component")
struct FTSBC_SolidityVariable
{
    GENERATED_BODY()

    /**
     * Solidity variable data type. See <code>ETSBC_SolidityDataType</code> for allowed values.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    /**
     * Solidity variable name.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";
};

/**
 * Contains the Solidity function signature.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Function Input")
struct FTSBC_SolidityFunctionSignature
{
    GENERATED_BODY()

    /**
     * Solidity function variable (or argument, depending how this is used).
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FTSBC_SolidityVariable Variable;

    /**
     * This array only contains elements if the "Variable" is of type "tuple".
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_SolidityVariable> TupleVariables;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    bool Indexed = false;
};

/**
 * Contract ABI "Function" Function.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Function")
struct FTSBC_ContractAbiFunction
{
    GENERATED_BODY()

    /**
     * The ABI function type can be: "constructor", "function", "receive", or "fallback".
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    /**
     * The ABI function name.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";

    /**
     * The ABI function arguments.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_SolidityFunctionSignature> Inputs;

    /**
     * Function return values.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_SolidityFunctionSignature> Outputs;

    /**
     * Function state mutability. Value can be: "pure", "view", "nonpayable", "payable".
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString StateMutability = "";
};

/**
 * Contract ABI "Event" Function.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Event")
struct FTSBC_ContractAbiEvent
{
    GENERATED_BODY()

    /**
     * The ABI function type is always "event".
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    /**
     * The ABI event (function) name.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";

    /**
     * The ABI function input arguments.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_SolidityFunctionSignature> Inputs;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    bool Anonymous = false;
};

/**
 * Contract ABI "Error" Function.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Error")
struct FTSBC_ContractAbiError
{
    GENERATED_BODY()

    /**
     * The ABI function type is always "error".
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    /**
     * The ABI error (function) name.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";

    /**
     * The ABI function input arguments.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_SolidityFunctionSignature> Inputs;
};

/**
 * Contract ABI.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI")
struct FTSBC_ContractAbi
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiFunction> ContractAbiFunctions;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiEvent> ContractAbiEvents;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiError> ContractAbiErrors;
};

struct FTSBC_ContractAbiEncodeContext
{
    TArray<FTSBC_SolidityFunctionSignature> FunctionParameters{};

    uint32 ParameterIndex{0};
    TArray<ETSBC_SolidityDataType> ArgumentsTypes{};
    TArray<FTSBC_SolidityValueList> Arguments{};

    bool bEncodingTuple{false};
    uint32 TupleParameterIndex{0};
    TArray<ETSBC_SolidityDataType> TupleArgumentsTypes{};
};