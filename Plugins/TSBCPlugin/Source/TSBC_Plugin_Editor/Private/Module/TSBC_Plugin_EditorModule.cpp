// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Module/TSBC_Plugin_EditorModule.h"
#include "AssetToolsModule.h"
#include "Pins/Factory/TSBC_FunctionNamePinFactory.h"

#define LOCTEXT_NAMESPACE "FTSBC_Plugin_EditorModule"

void FTSBC_Plugin_EditorModule::StartupModule()
{
    // Create factory and shared pointer to it and then register it.
    const TSharedPtr<FTSBC_FunctionNamePinFactory> Factory = MakeShareable(new FTSBC_FunctionNamePinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(Factory);

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTypeCategory = AssetTools.RegisterAdvancedAssetCategory(
        FName(TEXT("3Studio")),
        LOCTEXT("3StudioAssetCategory", "3Studio"));
}

void FTSBC_Plugin_EditorModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_GAME_MODULE(FTSBC_Plugin_EditorModule, TSBC_Plugin_Editor)