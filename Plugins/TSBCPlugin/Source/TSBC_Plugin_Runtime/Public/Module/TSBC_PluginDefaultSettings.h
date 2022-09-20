// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#include "TSBC_PluginDefaultSettings.generated.h"

UCLASS(Config="TSBC", DefaultConfig)
class TSBC_PLUGIN_RUNTIME_API UTSBC_PluginDefaultSettings : public UObject
{
    GENERATED_BODY()

public:
    UTSBC_PluginDefaultSettings();

    static const UTSBC_PluginDefaultSettings* Get()
    {
        const auto* Settings = Cast<UTSBC_PluginDefaultSettings>(StaticClass()->ClassDefaultObject);
        return IsValid(Settings) ? Settings : nullptr;
    }
};