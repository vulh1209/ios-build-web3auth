// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "Pins/TSBC_FunctionNameConfigPin.h"
#include "Data/TSBC_EditorTypes.h"

// Pin factory for ABI function name selection.
class FTSBC_FunctionNamePinFactory : public FGraphPanelPinFactory
{
    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InPin) const override
    {
        const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

        // Check if pin is struct, and then check if that pin is of struct type we want to customize
        if(InPin->PinType.PinCategory == K2Schema->PC_Struct &&
           InPin->PinType.PinSubCategoryObject == FTSBC_ContractAbiFunctionNamePin::StaticStruct())
        {
            // return our customized pin widget.
            return SNew(STSBC_FunctionNameConfigPin, InPin);
        }
        return nullptr;
    }
};