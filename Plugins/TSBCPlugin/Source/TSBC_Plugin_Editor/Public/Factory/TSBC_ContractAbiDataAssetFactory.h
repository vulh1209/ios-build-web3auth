// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Factories/DataAssetFactory.h"

#include "TSBC_ContractAbiDataAssetFactory.generated.h"

/**
 * Allows the user to create "TSBC_ContractAbiDataAsset" to save the contract's ABI data.
 */
UCLASS(Blueprintable)
class TSBC_PLUGIN_EDITOR_API UTSBC_ContractAbiDataAssetFactory : public UDataAssetFactory
{
    GENERATED_BODY()

public:
    UTSBC_ContractAbiDataAssetFactory();

    virtual FText GetDisplayName() const override;
    virtual FName GetNewAssetThumbnailOverride() const override;
    virtual uint32 GetMenuCategories() const override;
    virtual FText GetToolTip() const override;
    virtual bool ConfigureProperties() override;
    virtual FString GetDefaultNewAssetName() const override;

    virtual UObject* FactoryCreateNew(
        UClass* Class,
        UObject* InParent,
        const FName Name,
        EObjectFlags Flags,
        UObject* Context,
        FFeedbackContext* Warn) override;

};