// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#include "TSBC_ContractAbiTypes.generated.h"

/**
 * Mostly used types in Contract ABI.
 */
UENUM()
enum class EContractAbiTypes : uint8
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
    // Int           UMETA(DisplayName = "int"),
    // IntArray      UMETA(DisplayName = "intArray"),
    Bytes         UMETA(DisplayName = "bytes"),
    BytesArray    UMETA(DisplayName = "bytesArray"),
    Tuple         UMETA(DisplayName = "tuple"),
    // TupleArray    UMETA(DisplayName = "tupleArray"),
    // @formatter:on
};

/**
 * Function Types inside Contract ABI.
 */
UENUM(BlueprintType, DisplayName="Contract ABI Function Types")
enum class EContractAbiFunctionTypes : uint8
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
 * Name and Type of an Input Component inside tuples.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Input Component")
struct FTSBC_ContractAbiInputComponent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";
};

/**
 * Contract ABI Function Input.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Function Input")
struct FTSBC_ContractAbiFunctionInput
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FTSBC_ContractAbiInputComponent Component;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiInputComponent> Components;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    bool Indexed = false;
};

/**
 * Contract ABI Function.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Function")
struct FTSBC_ContractAbiFunction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiFunctionInput> Inputs;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiFunctionInput> Outputs;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString StateMutability = "";
};

/**
 * Contract ABI Event.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Event")
struct FTSBC_ContractAbiEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiFunctionInput> Inputs;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    bool Anonymous = false;
};

/**
 * Contract ABI Error.
 */
USTRUCT(BlueprintType, DisplayName="Contract ABI Error")
struct FTSBC_ContractAbiError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Type = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    FString Name = "";

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|Blockchain|Ethereum|ABI")
    TArray<FTSBC_ContractAbiFunctionInput> Inputs;
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