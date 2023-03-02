// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Engine/DataAsset.h"
// =============================================================================
#include "CoreMinimal.h"
#include "Data/TSBC_ContractAbiTypes.h"

#include "TSBC_ContractAbiDataAsset.generated.h"

/**
 * Custom Data Asset that holds Contract ABI Data.
 */
UCLASS(Blueprintable)
class TSBC_PLUGIN_RUNTIME_API UTSBC_ContractAbiDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Contract ABI")
    FString ContractAbiJson;

    UPROPERTY(VisibleDefaultsOnly, Category = "Contract ABI")
    FString ContractAbiValidityStatus;

    UPROPERTY(EditAnywhere, Category = "Contract ABI")
    FTSBC_ContractAbi ContractAbi;

public:
    UFUNCTION(CallInEditor, Category = "Contract ABI")
    void ParseJson();
};