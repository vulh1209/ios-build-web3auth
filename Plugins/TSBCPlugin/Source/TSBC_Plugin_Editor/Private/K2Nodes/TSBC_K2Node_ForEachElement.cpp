// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "K2Nodes/TSBC_K2Node_ForEachElement.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_AssignmentStatement.h"
#include "KismetCompiler.h"
#include "K2Nodes/TSBC_K2NodeWrapper.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "TSBC_K2Node_ForEachElement"

const FName UTSBC_K2Node_ForEachElement::InParamName_Array(TEXT("ArrayPin"));

const FName UTSBC_K2Node_ForEachElement::OutParamName_ArrayElement(TEXT("ArrayElementPin"));
const FName UTSBC_K2Node_ForEachElement::OutParamName_ArrayIndex(TEXT("ArrayIndexPin"));

const FName UTSBC_K2Node_ForEachElement::OutExecName_Completed(TEXT("CompletedPin"));

void UTSBC_K2Node_ForEachElement::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();

    // InExec
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

    // InParam: Array
    UEdGraphPin* ArrayPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, InParamName_Array);
    ArrayPin->PinType.ContainerType = EPinContainerType::Array;
    ArrayPin->PinType.bIsConst = true;
    ArrayPin->PinType.bIsReference = true;
    ArrayPin->PinFriendlyName = LOCTEXT("Array Pin", "Array");

    OriginalWildcardType = ArrayPin->PinType;

    // OutExec: Loop Body
    UEdGraphPin* LoopBodyPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    LoopBodyPin->PinFriendlyName = LOCTEXT("For Each Element Then Pin", "Loop Body");

    // OutParam: Array Element
    UEdGraphPin* ArrayElementPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, OutParamName_ArrayElement);
    ArrayElementPin->PinType.bIsConst = true;
    ArrayElementPin->PinType.bIsReference = true;
    ArrayElementPin->PinFriendlyName = LOCTEXT("Array Element Pin", "Array Element");

    // OutParam: Array Index
    UEdGraphPin* ArrayIndexPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, OutParamName_ArrayIndex);
    ArrayIndexPin->PinFriendlyName = LOCTEXT("Index Pin", "Array Index");
    ArrayIndexPin->PinToolTip = LOCTEXT("IndexPin_Tooltip", "Index of the Element inside the Array").ToString();

    // OutExec: Completed
    UEdGraphPin* CompletedPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, OutExecName_Completed);
    CompletedPin->PinFriendlyName = LOCTEXT("Completed Pin", "Completed");
    CompletedPin->PinToolTip = LOCTEXT(
        "CompletedPin_Tooltip",
        "Execute once all array elements have been iterated on").ToString();

    if(CurrentInputPinType.PinCategory == NAME_None)
    {
        CurrentInputPinType = OriginalWildcardType;
    }
    else if(CurrentInputPinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
    {
        ArrayPin->PinType = CurrentInputPinType;
        ArrayElementPin->PinType = CurrentInputPinType;
        ArrayElementPin->PinType.ContainerType = EPinContainerType::None;
        // Let the graph know to refresh
        GetGraph()->NotifyGraphChanged();

        UBlueprint* Blueprint = GetBlueprint();
        if(!Blueprint->bBeingCompiled)
        {
            FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
            Blueprint->BroadcastChanged();
        }
    }

    AdvancedPinDisplay = ENodeAdvancedPins::NoPins;
}

FText UTSBC_K2Node_ForEachElement::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("K2Node_ForEachElement_Title", "For Each Element");
}

void UTSBC_K2Node_ForEachElement::PinConnectionListChanged(UEdGraphPin* Pin)
{
    Super::PinConnectionListChanged(Pin);
    if(Pin == nullptr || Pin->PinName != InParamName_Array)
    {
        return;
    }

    CurrentInputPinType = Pin->LinkedTo.Num() > 0 ? Pin->LinkedTo[0]->PinType : OriginalWildcardType;
    Pin->PinType = CurrentInputPinType;
    Pin->PinType.bIsConst = true;
    Pin->PinType.bIsReference = true;

    UEdGraphPin* ElementPin = GetArrayElementPin();
    ElementPin->PinType = CurrentInputPinType;
    ElementPin->PinType.ContainerType = EPinContainerType::None;
    ElementPin->PinType.bIsConst = true;
    ElementPin->PinType.bIsReference = true;

    // Let the graph know to refresh
    GetGraph()->NotifyGraphChanged();

    UBlueprint* Blueprint = GetBlueprint();
    if(!Blueprint->bBeingCompiled)
    {
        FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
        Blueprint->BroadcastChanged();
    }
}

FText UTSBC_K2Node_ForEachElement::GetTooltipText() const
{
    return LOCTEXT("NodeToolTip", "Loop over each element of an array");
}

void UTSBC_K2Node_ForEachElement::PostPasteNode()
{
    Super::PostPasteNode();

    CurrentInputPinType.PinCategory = NAME_None;
    if(UEdGraphPin* ArrayPin = GetArrayPin())
    {
        if((CurrentInputPinType.PinCategory == NAME_None) && ArrayPin->LinkedTo.Num())
        {
            PinConnectionListChanged(ArrayPin);
        }
    }
}

FSlateIcon UTSBC_K2Node_ForEachElement::GetIconAndTint(FLinearColor& OutColor) const
{
    return FSlateIcon("EditorStyle", "GraphEditor.Macro.ForEach_16x");
}

bool UTSBC_K2Node_ForEachElement::IsConnectionDisallowed(
    const UEdGraphPin* MyPin,
    const UEdGraphPin* OtherPin,
    FString& OutReason) const
{
    if(MyPin == GetArrayPin()
       && OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard
       && OtherPin->PinType.ContainerType != EPinContainerType::Array)
    {
        OutReason = LOCTEXT(
            "Error_InvalidConnection",
            "Array pin can only accept array containers").ToString();
        return true;
    }

    return Super::IsConnectionDisallowed(MyPin, OtherPin, OutReason);
}

void UTSBC_K2Node_ForEachElement::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);

    if(GetArrayPin()->LinkedTo.Num() == 0)
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT("MissingArray_Error", "For Each Element node @@ must have an array to iterate on.").ToString(),
            this);
        BreakAllNodeLinks();
        return;
    }

    const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
    check(this && SourceGraph && Schema);

    // Cache pins of this K2 node
    const CThisK2Node ThisK2Node(this);

    // Create int Loop Counter
    const TSBC_K2NodeWrapper::TemporaryVariable_Int LoopCounter(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

    // Initialize loop counter
    const TSBC_K2NodeWrapper::AssignmentStatement Assign1_LoopCounter(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetInExec(), Assign1_LoopCounter.GetInExec());
    TSBC_LINK_I2I(LoopCounter.GetOutParam_Variable(), Assign1_LoopCounter.GetInParam_Variable());

    // Create int Array Index
    const TSBC_K2NodeWrapper::TemporaryVariable_Int ArrayIndex(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

    // Initialize array index
    const TSBC_K2NodeWrapper::AssignmentStatement Assign1_ArrayIndex(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(Assign1_LoopCounter.GetOutExec_Then(), Assign1_ArrayIndex.GetInExec());
    TSBC_LINK_I2I(ArrayIndex.GetOutParam_Variable(), Assign1_ArrayIndex.GetInParam_Variable());

    // Array length
    const TSBC_K2NodeWrapper::Array_Length Array_Length(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_S2I_COPY(ThisK2Node.GetInParam_Array(), Array_Length.GetInParam_Array());
    TSBC_COPY_PIN_TYPE(ThisK2Node.GetInParam_Array(), Array_Length.GetInParam_Array());

    // Do loop condition
    const TSBC_K2NodeWrapper::Less_IntInt Condition(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(LoopCounter.GetOutParam_Variable(), Condition.GetInParam_A());
    TSBC_LINK_I2I(Array_Length.GetOutParam_ReturnValue(), Condition.GetInParam_B());

    // Array Index assigned
    const TSBC_K2NodeWrapper::AssignmentStatement Assign2_ArrayIndex(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(ArrayIndex.GetOutParam_Variable(), Assign2_ArrayIndex.GetInParam_Variable());
    TSBC_LINK_I2I(LoopCounter.GetOutParam_Variable(), Assign2_ArrayIndex.GetInParam_Value());

    // Do loop branch
    const TSBC_K2NodeWrapper::Branch Branch(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(Assign1_ArrayIndex.GetOutExec_Then(), Branch.GetInExec());
    TSBC_LINK_I2I(Condition.GetOutParam_ReturnValue(), Branch.GetInParam_Condition());
    TSBC_LINK_I2I(Branch.GetOutExec_True(), Assign2_ArrayIndex.GetInExec());
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_Completed(), Branch.GetOutExec_False());

    // Body sequence
    const TSBC_K2NodeWrapper::ExecutionSequence Sequence(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(Assign2_ArrayIndex.GetOutExec_Then(), Sequence.GetInExec());

    // Loop Counter increment
    const TSBC_K2NodeWrapper::Add_IntInt Increment(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(LoopCounter.GetOutParam_Variable(), Increment.GetInParam_A());

    // Loop Counter assigned
    const TSBC_K2NodeWrapper::AssignmentStatement Assign2_LoopCounter(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(Sequence.GetOutExec_Then(1), Assign2_LoopCounter.GetInExec());
    TSBC_LINK_I2I(LoopCounter.GetOutParam_Variable(), Assign2_LoopCounter.GetInParam_Variable());
    TSBC_LINK_I2I(Increment.GetOutParam_ReturnValue(), Assign2_LoopCounter.GetInParam_Value());
    TSBC_LINK_I2I(Branch.GetInExec(), Assign2_LoopCounter.GetOutExec_Then());

    // Array element retrieval
    const TSBC_K2NodeWrapper::GetArrayItem Array_GetElement(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(ArrayIndex.GetOutParam_Variable(), Array_GetElement.GetInParam_Index());
    TSBC_LINK_S2I_COPY(ThisK2Node.GetInParam_Array(), Array_GetElement.GetInParam_Array());
    TSBC_COPY_PIN_TYPE(ThisK2Node.GetInParam_Array(), Array_GetElement.GetInParam_Array());
    TSBC_COPY_PIN_TYPE(ThisK2Node.GetInParam_Array(), Array_GetElement.GetOutParam_ReturnValue());
    Array_GetElement.GetOutParam_ReturnValue()->PinType.ContainerType = EPinContainerType::None;

    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_LoopBody(), Sequence.GetOutExec_Then(0));
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutParam_ArrayIndex(), ArrayIndex.GetOutParam_Variable());

    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutParam_ArrayElement(), Array_GetElement.GetOutParam_ReturnValue());

    BreakAllNodeLinks();
}

void UTSBC_K2Node_ForEachElement::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
    // Actions get registered under specific object-keys; The idea is that 
    // actions might have to be updated (or deleted) if their object-key is  
    // mutated (or removed)... here we use the node's class (so if the node 
    // type disappears, then the action should go with it)
    const UClass* ActionKey = GetClass();
    // To keep from needlessly instantiating a UBlueprintNodeSpawner, first   
    // check to make sure that the registrar is looking for actions of this type
    // (could be regenerating actions for a specific asset, and therefore the 
    // registrar would only accept actions corresponding to that asset)
    if(ActionRegistrar.IsOpenForRegistration(ActionKey))
    {
        UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
        check(NodeSpawner != nullptr);

        ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
    }
}

FText UTSBC_K2Node_ForEachElement::GetMenuCategory() const
{
    return LOCTEXT("MenuCategory_K2Node_ForEachElement", "3Studio|Utilities|Array");
}

UEdGraphPin* UTSBC_K2Node_ForEachElement::GetArrayPin() const
{
    return FindPinChecked(InParamName_Array);
}

UEdGraphPin* UTSBC_K2Node_ForEachElement::GetLoopBodyPin() const
{
    return FindPinChecked(UEdGraphSchema_K2::PN_Then);
}

UEdGraphPin* UTSBC_K2Node_ForEachElement::GetArrayElementPin() const
{
    return FindPinChecked(OutParamName_ArrayElement);
}

UEdGraphPin* UTSBC_K2Node_ForEachElement::GetArrayIndexPin() const
{
    return FindPinChecked(OutParamName_ArrayIndex);
}

UEdGraphPin* UTSBC_K2Node_ForEachElement::GetCompletedPin() const
{
    return FindPinChecked(OutExecName_Completed);
}

#undef LOCTEXT_NAMESPACE