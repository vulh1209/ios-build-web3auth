// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "K2Node.h"
#include "EdGraph/EdGraphPin.h"

#include "TSBC_K2Node_ForEachElement.generated.h"

class FBlueprintActionDatabaseRegistrar;

/**
 * Native implementation of the "For Each Loop" Blueprint Macro.
 *
 * See the <Plugin>/Docs directory for a screenshot of how this K2Node is constructed in Blueprints.
 */
UCLASS()
class TSBC_PLUGIN_EDITOR_API UTSBC_K2Node_ForEachElement : public UK2Node
{
    GENERATED_BODY()

private:
    class CThisK2Node
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_Array = nullptr;

        UEdGraphPin* OutExec_LoopBody = nullptr;
        UEdGraphPin* OutExec_Completed = nullptr;

        UEdGraphPin* OutParam_ArrayElement = nullptr;
        UEdGraphPin* OutParam_ArrayIndex = nullptr;

    public:
        explicit CThisK2Node(const ThisClass* Node)
        {
            InExec = Node->GetExecPin();

            InParam_Array = Node->GetArrayPin();

            OutExec_LoopBody = Node->GetLoopBodyPin();
            OutExec_Completed = Node->GetCompletedPin();

            OutParam_ArrayElement = Node->GetArrayElementPin();
            OutParam_ArrayIndex = Node->GetArrayIndexPin();
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_Array() const { return InParam_Array; }

        UEdGraphPin* GetOutExec_LoopBody() const { return OutExec_LoopBody; }
        UEdGraphPin* GetOutExec_Completed() const { return OutExec_Completed; }

        UEdGraphPin* GetOutParam_ArrayElement() const { return OutParam_ArrayElement; }
        UEdGraphPin* GetOutParam_ArrayIndex() const { return OutParam_ArrayIndex; }
    };

private:
    static const FName InParamName_Array;

    static const FName OutExecName_Completed;

    static const FName OutParamName_ArrayElement;
    static const FName OutParamName_ArrayIndex;

    UPROPERTY()
    FEdGraphPinType OriginalWildcardType;

    UPROPERTY()
    FEdGraphPinType CurrentInputPinType;

public:
    //~ Begin UEdGraphNode Interface.
    virtual void AllocateDefaultPins() override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
    virtual FText GetTooltipText() const override;
    virtual void PostPasteNode() override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    //~ End UEdGraphNode Interface.

    //~ Begin UK2Node Interface.
    virtual bool IsNodePure() const override { return false; }
    virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
    virtual bool IsConnectionDisallowed(
        const UEdGraphPin* MyPin,
        const UEdGraphPin* OtherPin,
        FString& OutReason) const override;
    virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    virtual FText GetMenuCategory() const override;
    //~ End UK2Node Interface.

    UEdGraphPin* GetArrayPin() const;
    UEdGraphPin* GetLoopBodyPin() const;
    UEdGraphPin* GetArrayElementPin() const;
    UEdGraphPin* GetArrayIndexPin() const;
    UEdGraphPin* GetCompletedPin() const;
};