// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class FTSBC_Plugin_EditorModule : public FDefaultGameModuleImpl
{
    EAssetTypeCategories::Type AssetTypeCategory = EAssetTypeCategories::None;

public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    EAssetTypeCategories::Type GetAssetCategoryBit() const
    {
        return AssetTypeCategory;
    }
};