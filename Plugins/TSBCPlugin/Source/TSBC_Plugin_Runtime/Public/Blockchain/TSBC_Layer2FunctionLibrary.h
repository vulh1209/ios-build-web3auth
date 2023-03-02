// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_Layer2FunctionLibrary.generated.h"

/**
 * Layer2 Blueprint Function Library.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_Layer2FunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Gets configuration data for a specific Layer2 network needed to send HTTP calls to.
     * 
     * @param Layer2Network The Layer2 network for which to get its configuration data.
     * @param Layer2Config Configuration data for a specific Layer2 network.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Get Layer2 configuration",
        Category="3Studio|Blockchain|Layer2")
    static void GetLayer2Config(
        const ETSBC_Layer2Network Layer2Network,
        FTSBC_Layer2Config& Layer2Config);
};