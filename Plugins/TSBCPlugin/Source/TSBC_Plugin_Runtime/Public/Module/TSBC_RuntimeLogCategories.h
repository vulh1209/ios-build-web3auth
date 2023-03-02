// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

TSBC_PLUGIN_RUNTIME_API DECLARE_LOG_CATEGORY_EXTERN(LogTSBC_Runtime, Log, All);

#define TSBC_LOG(Verbosity, Format, ...) UE_LOG(LogTSBC_Runtime, Verbosity, Format, ##__VA_ARGS__);
#define TSBC_LOG_COND(Condition, Verbosity, Format, ...) if(Condition) { UE_LOG(LogTSBC_Runtime, Verbosity, Format, ##__VA_ARGS__); }