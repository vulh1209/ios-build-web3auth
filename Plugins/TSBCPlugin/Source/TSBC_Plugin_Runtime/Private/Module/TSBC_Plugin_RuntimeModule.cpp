// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Module/TSBC_Plugin_RuntimeModule.h"

// =============================================================================
// These includes need to be specified using full paths to avoid packaging errors.
#include "Developer/Settings/Public/ISettingsModule.h"
// =============================================================================

#include "Module/TSBC_PluginDefaultSettings.h"
#include "Module/TSBC_PluginUserSettings.h"

#define LOCTEXT_NAMESPACE "FTSBC_Plugin_RuntimeModule"

void FTSBC_Plugin_RuntimeModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact
    // timing is specified in the .uplugin file per-module

    // Register custom settings to appear in the project settings
    if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        // SettingsModule->RegisterSettings(
        //     "Project",
        //     "3Studio",
        //     "BlockchainDefaultSettings",
        //     LOCTEXT("TSBC_Name_DefaultSettings", "Blockchain [Default Settings]"),
        //     LOCTEXT("TSBC_Desc_DefaultSettings", "Configure project-wide settings of the 3Studio Blockchain plug-in."),
        //     GetMutableDefault<UTSBC_PluginDefaultSettings>()
        // );

        SettingsModule->RegisterSettings(
            "Project",
            "3Studio",
            "BlockchainUserSettings",
            LOCTEXT("TSBC_Name_UserSettings", "Blockchain [User Settings]"),
            LOCTEXT(
                "TSBC_Desc_UserSettings",
                "Configure user-specific settings of the 3Studio Blockchain plug-in. Usually, UserTSBC.ini should be ignored in your VCS."),
            GetMutableDefault<UTSBC_PluginUserSettings>()
        );
    }
}

void FTSBC_Plugin_RuntimeModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module. For
    // modules that support dynamic reloading, we call this function before
    // unloading the module.

    // Remove custom settings
    if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        // SettingsModule->UnregisterSettings("Project", "3Studio", "BlockchainDefaultSettings");

        SettingsModule->UnregisterSettings("Project", "3Studio", "BlockchainUserSettings");
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_GAME_MODULE(FTSBC_Plugin_RuntimeModule, TSBC_Plugin_Runtime)
