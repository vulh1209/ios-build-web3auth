// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Pins/TSBC_FunctionNameConfigPin.h"

#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "SNameComboBox.h"
#include "SGraphPanel.h"
#include "Data/TSBC_EditorTypes.h"
#include "K2Nodes/TSBC_K2Node_DecodeSmartContractAbi.h"
#include "K2Nodes/TSBC_K2Node_EncodeSmartContractAbi.h"

void STSBC_FunctionNameConfigPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    AttributesList.Empty();
    SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget> STSBC_FunctionNameConfigPin::GetDefaultValueWidget()
{
    RefreshFunctionNameList();
    if(GraphPinObj)
    {
        FName FunctionName;
        // Preserve previous selection, or set to first in list
        const FName PreviousSelection = FName(*GraphPinObj->GetDefaultAsString());
        FString PinString = PreviousSelection.ToString();
        if(PinString.StartsWith(TEXT("(")) && PinString.EndsWith(TEXT(")")))
        {
            PinString.MidInline(1, PinString.Len() - 2);
            PinString.Split("=", nullptr, &PinString);

            if(PinString.StartsWith(TEXT("\"")) && PinString.EndsWith(TEXT("\"")))
            {
                PinString.MidInline(1, PinString.Len() - 2);
            }
            FunctionName = FName(*PinString);
        }
        for(TSharedPtr<FName> ListNamePtr : AttributesList)
        {
            if(FunctionName == *ListNamePtr.Get())
            {
                CurrentlySelectedName = ListNamePtr;
                break;
            }
        }
    }

    return SAssignNew(NameComboBox, SNameComboBox)
        .OptionsSource(&AttributesList)
        .InitiallySelectedItem(CurrentlySelectedName)
        .OnSelectionChanged(this, &STSBC_FunctionNameConfigPin::OnAttributeSelected);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void STSBC_FunctionNameConfigPin::OnAttributeSelected(
    const TSharedPtr<FName> ItemSelected,
    ESelectInfo::Type SelectInfo)
{
    FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();
    const FString SelectedAttribute = *ItemSelected->ToString();
    if(CurrentDefaultValue.IsEmpty())
    {
        CurrentDefaultValue = FString(TEXT("()"));
    }

    const FString AttributeString = FTSBC_ContractAbiFunctionNamePin::Serialize(SelectedAttribute);

    // Here we set our value to parameter if it is different than last one.
    if(!CurrentDefaultValue.Equals(AttributeString))
    {
        GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, AttributeString);
    }
}

void STSBC_FunctionNameConfigPin::RefreshFunctionNameList()
{
    UEdGraphNode* OwnerGraphNode = GraphPinObj->GetOwningNode();
    check(GraphPinObj);

    UTSBC_K2Node_EncodeSmartContractAbi* const K2Node_EncodeAbi = Cast<UTSBC_K2Node_EncodeSmartContractAbi>(
        OwnerGraphNode);
    if(IsValid(K2Node_EncodeAbi))
    {
        for(const FTSBC_ContractAbiFunction Function : K2Node_EncodeAbi->ContractAbi.ContractAbiFunctions)
        {
            AttributesList.Add(MakeShareable(new FName(Function.Name)));
        }

        return;
    }

    UTSBC_K2Node_DecodeSmartContractAbi* const K2Node_DecodeAbi = Cast<UTSBC_K2Node_DecodeSmartContractAbi>(
        OwnerGraphNode);
    if(IsValid(K2Node_DecodeAbi))
    {
        for(const FTSBC_ContractAbiFunction Function : K2Node_DecodeAbi->ContractAbi.ContractAbiFunctions)
        {
            AttributesList.Add(MakeShareable(new FName(Function.Name)));
        }
    }
}