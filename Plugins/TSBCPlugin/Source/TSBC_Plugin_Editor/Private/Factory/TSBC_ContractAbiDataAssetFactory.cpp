// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Factory/TSBC_ContractAbiDataAssetFactory.h"

#include "Module/TSBC_Plugin_EditorModule.h"
#include "DataAssets/TSBC_ContractAbiDataAsset.h"

#define LOCTEXT_NAMESPACE "3StudioEditorFactories"

UTSBC_ContractAbiDataAssetFactory::UTSBC_ContractAbiDataAssetFactory()
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UDataAsset::StaticClass();
    DataAssetClass = UTSBC_ContractAbiDataAsset::StaticClass();
}

FText UTSBC_ContractAbiDataAssetFactory::GetDisplayName() const
{
    return LOCTEXT(
        "TSBC_ContractAbiDataAssetFactoryDescription",
        "Contract ABI Data Asset");
}

FName UTSBC_ContractAbiDataAssetFactory::GetNewAssetThumbnailOverride() const
{
    return TEXT("ClassThumbnail.DataAsset");
}

uint32 UTSBC_ContractAbiDataAssetFactory::GetMenuCategories() const
{
    return FModuleManager::GetModuleChecked<FTSBC_Plugin_EditorModule>("TSBC_Plugin_Editor").GetAssetCategoryBit();
}

FText UTSBC_ContractAbiDataAssetFactory::GetToolTip() const
{
    return LOCTEXT(
        "TSBC_ContractAbiDataAssetTooltip",
        "TSBC Contract ABI Data Asset");
}

UObject* UTSBC_ContractAbiDataAssetFactory::FactoryCreateNew(
    UClass* Class,
    UObject* InParent,
    const FName Name,
    EObjectFlags Flags,
    UObject* Context,
    FFeedbackContext* Warn)
{
    // If we have no data asset class, use the passed-in class instead.
    check(Class->IsChildOf(UDataAsset::StaticClass()));

    if(DataAssetClass != nullptr)
    {
        return NewObject<UDataAsset>(
            InParent,
            DataAssetClass,
            Name,
            Flags | RF_Transactional);
    }

    return NewObject<UTSBC_ContractAbiDataAsset>(
        InParent,
        Class,
        Name,
        Flags);
}

bool UTSBC_ContractAbiDataAssetFactory::ConfigureProperties()
{
    return true;
}

FString UTSBC_ContractAbiDataAssetFactory::GetDefaultNewAssetName() const
{
    return FString(TEXT("Contract ABI Data Asset"));
}

#undef LOCTEXT_NAMESPACE