// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#include "TSBC_PluginUserSettings.generated.h"

UCLASS(Config="TSBC", ProjectUserConfig)
class TSBC_PLUGIN_RUNTIME_API UTSBC_PluginUserSettings : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(
        Config,
        EditAnywhere,
        Category="Debug",
        DisplayName="Enable Debug Logging: Signed Transactions",
        Meta=(ToolTip="This setting is only considered while running in-editor."))
    bool bDebugLoggingSignedTransactionsEnabled;

    UPROPERTY(
        Config,
        EditAnywhere,
        Category="Debug",
        DisplayName="Calculate decimal and hex values for each uint256 value [High performance impact]",
        Meta=(ToolTip="This setting is only considered while running in-editor."))
    bool bDebugUint256Values;

public:
    UTSBC_PluginUserSettings();

    static const UTSBC_PluginUserSettings* Get()
    {
        const auto* Settings = Cast<UTSBC_PluginUserSettings>(StaticClass()->ClassDefaultObject);
        return IsValid(Settings) ? Settings : nullptr;
    }
};