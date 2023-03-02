// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"

class SNameComboBox;

// Customized pin for function names, this will create a combo box for function selection.
class STSBC_FunctionNameConfigPin : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(STSBC_FunctionNameConfigPin)
        {
        }

    SLATE_END_ARGS()

private:
    TArray<TSharedPtr<FName>> AttributesList;
    TSharedPtr<SNameComboBox> NameComboBox;
    TSharedPtr<FName> CurrentlySelectedName;

public:
    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
    // This override is used to display slate widget used for customization.
    virtual TSharedRef<SWidget> GetDefaultValueWidget() override;

    void OnAttributeSelected(const TSharedPtr<FName> ItemSelected, ESelectInfo::Type SelectInfo);

private:
    void RefreshFunctionNameList();
};