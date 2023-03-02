// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "K2Nodes/TSBC_K2Node_EncodeSmartContractAbi.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Data/TSBC_EditorTypes.h"
#include "DataAssets/TSBC_ContractAbiDataAsset.h"
#include "Encoding/TSBC_ContractAbiHelper.h"
#include "Math/TSBC_uint256FunctionLibrary.h"

#define LOCTEXT_NAMESPACE "TSBC_K2Node_EncodeSmartContractAbi"

const FName UTSBC_K2Node_EncodeSmartContractAbi::InputParamPinName_ContractAbiDataAsset(TEXT("ContractAbiDataAsset"));
const FName UTSBC_K2Node_EncodeSmartContractAbi::InputParamPinName_FunctionName(TEXT("FunctionName"));

const FName UTSBC_K2Node_EncodeSmartContractAbi::OutputParamPinName_ErrorMessage(TEXT("ErrorMessage"));
const FName UTSBC_K2Node_EncodeSmartContractAbi::OutputParamPinName_FunctionHashAndEncodedArguments(
    TEXT("FunctionHashAndEncodedArguments"));

UTSBC_K2Node_EncodeSmartContractAbi::UTSBC_K2Node_EncodeSmartContractAbi(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NodeTooltip = LOCTEXT(
        "NodeTooltip",
        "Generates a \"Function Selector\" with argument encoding for a specific function of an Ethereum Smart Contract.");
}

void UTSBC_K2Node_EncodeSmartContractAbi::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();

    // In: Execution pin
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

    // Out: Success execution pin 
    UEdGraphPin* ExecuteEncodeSuccessPin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_Exec,
        UEdGraphSchema_K2::PN_Then);
    ExecuteEncodeSuccessPin->PinFriendlyName = LOCTEXT("Successful Exec pin", "Successful");

    // Out: Failed execution pin 
    UEdGraphPin* ExecuteEncodeFailedPin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_Exec,
        UEdGraphSchema_K2::PN_Else);
    ExecuteEncodeFailedPin->PinFriendlyName = LOCTEXT("Failed Exec pin", "Failed");

    // In: Contract ABI Data Asset pin
    UEdGraphPin* ContractAbiPin = CreatePin(
        EGPD_Input,
        UEdGraphSchema_K2::PC_Object,
        UTSBC_ContractAbiDataAsset::StaticClass(),
        InputParamPinName_ContractAbiDataAsset);
    ContractAbiPin->bNotConnectable = true;
    ContractAbiPin->PinFriendlyName = LOCTEXT("Contract ABI Data Asset pin", "Contract ABI Data Asset");
    ContractAbiPin->PinToolTip = LOCTEXT("ContractAbiDataAssetPin_ToolTip", "The Contract ABI Data Asset").ToString();

    // In: Function Name pin 
    UScriptStruct* FunctionNameScriptStruct =
        TSBC_TypeUtils::FindStructSafe<FTSBC_ContractAbiFunctionNamePin>(nullptr);
    UEdGraphPin* FunctionNamePin = CreatePin(
        EGPD_Input,
        UEdGraphSchema_K2::PC_Struct,
        FunctionNameScriptStruct,
        InputParamPinName_FunctionName);
    FunctionNamePin->bNotConnectable = true;
    FunctionNamePin->PinToolTip = LOCTEXT("FunctionNamePin_ToolTip", "The function name").ToString();

    // Out: Error Message pin
    UEdGraphPin* ErrorMessagePin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_String,
        OutputParamPinName_ErrorMessage);
    ErrorMessagePin->PinToolTip = LOCTEXT(
        "ErrorMessagePin_ToolTip",
        "Contains an error message in case the operation fails. Otherwise, it will be empty").ToString();

    // Out: Function Hash pin
    UEdGraphPin* FunctionHashPin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_String,
        OutputParamPinName_FunctionHashAndEncodedArguments);
    FunctionHashPin->PinToolTip = LOCTEXT(
        "FunctionHashPin_ToolTip",
        "The \"Function Selector\" with encoded arguments").ToString();

    // Create argument pins if we already have a spawned node with Contract ABI and function set.
    CreateArgumentPins();
}

FText UTSBC_K2Node_EncodeSmartContractAbi::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("K2Node_EncodeABI_Title", "Encode Smart Contract ABI");
}

void UTSBC_K2Node_EncodeSmartContractAbi::PinDefaultValueChanged(UEdGraphPin* Pin)
{
    const UEdGraphPin* ContractAbiDataAssetPin = GetContractAbiDataAssetPin();
    UEdGraphPin* FunctionNamePin = GetFunctionNamePin();

    // When the default value of FunctionName pin was changed remove old pins and create new ones.
    if(Pin == FunctionNamePin && FunctionNamePin->LinkedTo.Num() == 0)
    {
        // Save the currently selected value of FunctionName pin for creation of its argument pins.
        CurrentSerializedAbiFunctionName = FunctionNamePin->DefaultValue;
        if(FunctionNamePin->DefaultValue == Pin->DefaultValue)
        {
            CreateArgumentPins();
            GetGraph()->NotifyGraphChanged();
        }
        return;
    }

    // Set the Contract ABI data from newly selected Contract ABI Data Asset.
    if(Pin == ContractAbiDataAssetPin && ContractAbiDataAssetPin->LinkedTo.Num() == 0)
    {
        ContractAbi = {};
        const UTSBC_ContractAbiDataAsset* ContractDataAsset =
            Cast<UTSBC_ContractAbiDataAsset>(ContractAbiDataAssetPin->DefaultObject);
        // Reset FunctionName pin to its default value and mark it as modified.
        FunctionNamePin->ResetDefaultValue();
        FunctionNamePin->Modify();

        RemoveArgumentPins();
        if(IsValid(ContractDataAsset))
        {
            ContractAbi = ContractDataAsset->ContractAbi;
            CreateArgumentPins();
        }
        GetGraph()->NotifyGraphChanged();
    }
}

FText UTSBC_K2Node_EncodeSmartContractAbi::GetTooltipText() const
{
    return NodeTooltip;
}

bool UTSBC_K2Node_EncodeSmartContractAbi::IsConnectionDisallowed(
    const UEdGraphPin* MyPin,
    const UEdGraphPin* OtherPin,
    FString& OutReason) const
{
    const UEdGraphPin* CachedContractAbiDataAssetPin = GetContractAbiDataAssetPin();
    if(MyPin == CachedContractAbiDataAssetPin && MyPin->Direction == EGPD_Input)
    {
        OutReason = LOCTEXT(
            "Error_InvalidConnection",
            "Contract ABI Pin should be used as default value only.").ToString();
        return true;
    }

    return Super::IsConnectionDisallowed(MyPin, OtherPin, OutReason);
}

void UTSBC_K2Node_EncodeSmartContractAbi::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);

    const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
    check(this && SourceGraph && Schema);

    // Cache pins of this K2 node
    const CThisK2Node ThisK2Node(Schema, CompilerContext, SourceGraph, this);

    const UTSBC_ContractAbiDataAsset* ContractAbiDataAsset =
        ThisK2Node.GetInParam_ContractAbiDataAsset() != nullptr
        ? Cast<UTSBC_ContractAbiDataAsset>(ThisK2Node.GetInParam_ContractAbiDataAsset()->DefaultObject)
        : nullptr;

    if(ThisK2Node.GetInParam_ContractAbiDataAsset() == nullptr
       || (ContractAbiDataAsset == nullptr && ThisK2Node.GetInParam_ContractAbiDataAsset()->LinkedTo.Num() == 0))
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT("EncodeABINoAsset_Error", "Encode ABI must have a DataAsset specified in @@").ToString(),
            this);

        // We break exec links so that this is the only error we get.
        BreakAllNodeLinks();
        return;
    }

    // Check if FunctionName pin has an actual value and is not empty or default parsed struct member.
    if(FTSBC_ContractAbiFunctionNamePin::IsValueEmptyOrDefault(*ThisK2Node.GetInParam_FunctionName()))
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT("EncodeABINoFunctionName_Error", "Function Name must must be selected in @@").ToString(),
            this);

        // We break exec links so that this is the only error we get.
        BreakAllNodeLinks();
        return;
    }

    TArray<ETSBC_SolidityDataType> ArgumentsTypes;
    bool bContainsTuple;
    CTSBC_ContractAbiHelper::GetSolidityDataTypes(
        CurrentAbiFunction.Inputs,
        ArgumentsTypes,
        bContainsTuple);

    // Check if the function have tuple array argument
    if(bContainsTuple && (ArgumentsTypes.Num() > 1 || ArgumentsTypes.Num() == 0))
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT(
                "EncodeABI_Error",
                "Encoding function @@ is not possible due to blueprints limitations.").ToString(),
            this
        );

        // We break exec links so that this is the only error we get.
        BreakAllNodeLinks();
        return;
    }

    if(bContainsTuple)
    {
        ArgumentsTypes.Empty();
        bool bDynamic;
        CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
            CurrentAbiFunction.Inputs[0].TupleVariables,
            bDynamic,
            ArgumentsTypes);
    }

    // Create a "Make Array" node to compile the list of arguments into an array for the Encode ABI function being called
    const TSBC_K2NodeWrapper::MakeArray FunctionArguments(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

    // This is the node that does all the encoding work.
    const TSBC_K2NodeWrapper::EncodeSmartContractAbi EncodeAbi(
        TSBC_K2NODE_WRAPPER_DEFAULT_ARGS,
        ThisK2Node.GetInParam_ContractAbiDataAsset()->DefaultObject,
        FTSBC_ContractAbiFunctionNamePin::Deserialize(ThisK2Node.GetInParam_FunctionName()->DefaultValue));
    TSBC_LINK_I2I(FunctionArguments.GetOutParam_Array(), EncodeAbi.GetInParam_FunctionArguments());

    // This will set the "Make Array" node's type, only works if one pin is connected.
    FunctionArguments.GetNode()->PinConnectionListChanged(FunctionArguments.GetOutParam_Array());

    // If there's no parameters, empty pins on the "Make array" node and link to EncodeAbi
    // This fixes the issue where when there is no Input arguments the node would not execute at all
    if(InputArgumentPinNames.Num() == 0)
    {
        FunctionArguments.GetNode()->RemoveInputPin(FunctionArguments.GetInParam_ArrayElement(0));
        TSBC_LINK_S2I_MOVE(ThisK2Node.GetInExec(), EncodeAbi.GetInExec());
    }
    else
    {
        SpawnAndLinkArgumentStructs(
            CompilerContext,
            SourceGraph,
            ThisK2Node,
            ArgumentsTypes,
            FunctionArguments,
            EncodeAbi);
    }

    // Create a "Branch" node
    const TSBC_K2NodeWrapper::Branch Branch(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(EncodeAbi.GetOutExec_Then(), Branch.GetInExec());
    TSBC_LINK_I2I(EncodeAbi.GetOutParam_bSuccess(), Branch.GetInParam_Condition());

    // Hook up outputs param pins
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutParam_ErrorMessage(), EncodeAbi.GetOutParam_ErrorMessage());
    TSBC_LINK_S2I_MOVE(
        ThisK2Node.GetOutParam_FunctionHashAndEncodedArguments(),
        EncodeAbi.GetOutParam_FunctionHashAndEncodedArguments());

    // Hook up outputs exec pins
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_Successful(), Branch.GetOutExec_True());
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_Failed(), Branch.GetOutExec_False());

    BreakAllNodeLinks();
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::FindInputArgumentPinByName(const FName InPinName) const
{
    const UEdGraphPin* FunctionNamePin = GetFunctionNamePin();
    for(UEdGraphPin* Pin : Pins)
    {
        if(Pin != FunctionNamePin && Pin->Direction != EGPD_Output)
        {
            if(Pin->PinName.ToString().Equals(InPinName.ToString(), ESearchCase::CaseSensitive))
            {
                return Pin;
            }
        }
    }

    return nullptr;
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::FindArgumentPin(const UEdGraphPin* InPin) const
{
    const UEdGraphPin* FunctionNamePin = GetFunctionNamePin();
    for(UEdGraphPin* Pin : Pins)
    {
        if(Pin == InPin && Pin != FunctionNamePin && Pin->Direction != EGPD_Output)
        {
            return Pin;
        }
    }

    return nullptr;
}

void UTSBC_K2Node_EncodeSmartContractAbi::CreateArgumentPins()
{
    for(const FTSBC_ContractAbiFunction Function : ContractAbi.ContractAbiFunctions)
    {
        FString SelectedFunction = FTSBC_ContractAbiFunctionNamePin::Deserialize(CurrentSerializedAbiFunctionName);
        if(Function.Name != SelectedFunction)
        {
            continue;
        }

        CurrentAbiFunction = Function;

        // Clear the argument pin array before creating new pins.
        RemoveArgumentPins();

        ArgumentCount = Function.Inputs.Num();

        TArray<ETSBC_SolidityDataType> ArgumentsTypes;
        bool bContainsTuple;
        CTSBC_ContractAbiHelper::GetSolidityDataTypes(CurrentAbiFunction.Inputs, ArgumentsTypes, bContainsTuple);
        if(bContainsTuple && ArgumentsTypes.Num() > 1)
        {
            return;
        }

        for(int32 TypeIndex = 0; TypeIndex < ArgumentsTypes.Num(); TypeIndex++)
        {
            if(bContainsTuple)
            {
                ArgumentsTypes.Empty();
                bool bDynamic;
                CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
                    CurrentAbiFunction.Inputs[0].TupleVariables,
                    bDynamic,
                    ArgumentsTypes);
                for(int32 TupleIndex = 0; TupleIndex < ArgumentsTypes.Num(); TupleIndex++)
                {
                    const FTSBC_SolidityVariable TupleInput = Function.Inputs[TypeIndex].TupleVariables[TupleIndex];

                    UEdGraphPin* ArgumentPin = CreateArgumentPin(
                        TupleInput.Name,
                        TupleInput.Type,
                        ArgumentsTypes[TupleIndex]);
                    ArgumentPins.Add(ArgumentPin);
                    InputArgumentPinNames.Add(ArgumentPin->PinName);
                }
                return;
            }

            UEdGraphPin* ArgumentPin = CreateArgumentPin(
                Function.Inputs[TypeIndex].Variable.Name,
                Function.Inputs[TypeIndex].Variable.Type,
                ArgumentsTypes[TypeIndex]);
            ArgumentPins.Add(ArgumentPin);
            InputArgumentPinNames.Add(ArgumentPin->PinName);
        }
    }
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::CreateArgumentPin(
    const FString& PinName,
    const FString& PinType,
    const ETSBC_SolidityDataType& PinDataType)
{
    FCreatePinParams PinParams;
    PinParams.ContainerType = EPinContainerType::None;
    PinParams.bIsConst = true;

    // We need to set additional parameters if this is an array.
    if(PinType.EndsWith("]"))
    {
        PinParams.ContainerType = EPinContainerType::Array;
        PinParams.bIsConst = true;
        PinParams.bIsReference = true;
    }

    FName PinCategory = UEdGraphSchema_K2::PC_String;
    switch(PinDataType)
    {
    case ETSBC_SolidityDataType::Address:
    case ETSBC_SolidityDataType::AddressArray:
    case ETSBC_SolidityDataType::String:
    case ETSBC_SolidityDataType::StringArray:
    case ETSBC_SolidityDataType::Bytes:
    case ETSBC_SolidityDataType::BytesArray:
    case ETSBC_SolidityDataType::Tuple:
        {
            // "PinCategory" stays unchanged.
            break;
        }
    case ETSBC_SolidityDataType::Bool:
    case ETSBC_SolidityDataType::BoolArray:
        {
            PinCategory = UEdGraphSchema_K2::PC_Boolean;
            break;
        }
    case ETSBC_SolidityDataType::Uint:
    case ETSBC_SolidityDataType::UintArray:
        {
            PinParams.bIsReference = true;
            // Use our custom uint256 struct implementation for uint Solidity data type.
            PinCategory = UEdGraphSchema_K2::PC_Struct;
            UScriptStruct* UintScriptStruct = TSBC_TypeUtils::FindStructSafe<FTSBC_uint256>(nullptr);
            return CreatePin(
                EGPD_Input,
                PinCategory,
                UintScriptStruct,
                FName(PinName),
                PinParams);
        }

    default:
        {
            checkNoEntry();
            break;
        };
    }

    return CreatePin(
        EGPD_Input,
        PinCategory,
        FName(PinName),
        PinParams);
}

void UTSBC_K2Node_EncodeSmartContractAbi::RemoveArgumentPins()
{
    // Clear all arguments pins
    for(UEdGraphPin* Argument : ArgumentPins)
    {
        Argument->MarkAsGarbage();
        Pins.Remove(Argument);
    }
    ArgumentPins.Empty();
    InputArgumentPinNames.Empty();
}


void UTSBC_K2Node_EncodeSmartContractAbi::SpawnAndLinkArgumentStructs(
    FKismetCompilerContext& CompilerContext,
    UEdGraph* SourceGraph,
    const CThisK2Node& ThisK2Node,
    TArray<ETSBC_SolidityDataType>& ArgumentsTypes,
    const TSBC_K2NodeWrapper::MakeArray& FunctionArguments,
    const TSBC_K2NodeWrapper::EncodeSmartContractAbi& EncodeAbi)
{
    // For each argument, we will need to spawn a "Make Struct" node and "Make Array" node if container type is not array.
    for(int32 ArgIdx = 0; ArgIdx < InputArgumentPinNames.Num(); ++ArgIdx)
    {
        UEdGraphPin* InputArgumentPin = FindInputArgumentPinByName(InputArgumentPinNames[ArgIdx]);

        // Spawn a "Make Struct" node of type TSBC_SolidityValueList.
        TSBC_K2NodeWrapper::MakeStruct_SolidityValueList SolidityValueList(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

        // Move the connection of the argument pin to the correct argument value pin and also set the correct
        // argument type based on the pin that was hooked up.
        const ETSBC_SolidityDataType ArgumentType = ArgumentsTypes[ArgIdx];
        switch(InputArgumentPin->PinType.ContainerType)
        {
        case EPinContainerType::None:
            {
                LinkArgumentStructToK2NodePins(
                    ThisK2Node,
                    ArgumentType,
                    SolidityValueList,
                    InputArgumentPin);
                break;
            }

        case EPinContainerType::Array:
            {
                LinkArgumentStructToK2NodePinsArray(
                    ThisK2Node,
                    ArgumentType,
                    SolidityValueList,
                    InputArgumentPin);
                break;
            }

        case EPinContainerType::Set:
        case EPinContainerType::Map:
        default:
            {
                checkNoEntry();
                break;
            }
        }

        // The "Make Array" node already has one pin available, so don't create one for ArgIdx == 0
        if(ArgIdx > 0)
        {
            FunctionArguments.GetNode()->AddInputPin();
        }
        // Find the input pin on the "Make Array" node by index.
        UEdGraphPin* InputPin = FunctionArguments.GetInParam_ArrayElement(ArgIdx);

        // Find the output for the pin's "Make Struct" node and link it to the corresponding pin on the "Make Array" node.
        TSBC_LINK_I2I(FindOutputStructPinChecked(SolidityValueList.GetNode()), InputPin);
    }

    if(bConnectedToLoop)
    {
        TSBC_LINK_S2I_MOVE(ThisK2Node.GetInExec(), FirstLoop_InExec);
        TSBC_LINK_I2I(LastLoop_OutExec_Completed, EncodeAbi.GetInExec());
    }
    else
    {
        TSBC_LINK_S2I_MOVE(ThisK2Node.GetInExec(), EncodeAbi.GetInExec());
    }
}

void UTSBC_K2Node_EncodeSmartContractAbi::LinkArgumentStructToK2NodePins(
    const CThisK2Node& ThisK2Node,
    const ETSBC_SolidityDataType& ArgumentType,
    const TSBC_K2NodeWrapper::MakeStruct_SolidityValueList& SolidityValueList,
    UEdGraphPin* ArgumentPin)
{
    TSBC_DECLARE_LINK_VARS(ThisK2Node);

    // Create a "Make Array" node to compile the list of arguments into an array for the Argument struct
    const TSBC_K2NodeWrapper::MakeArray Arguments(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    // Connect the output of the "Make Array" pin to the struct's "Values" pin
    TSBC_LINK_I2I(SolidityValueList.GetInParam_Values(), Arguments.GetOutParam_Array());
    // This will set the "Make Array" node's type, only works if one pin is connected.
    TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Arguments.GetOutParam_Array());

    // Find the first input pin on the "Make Array" node by index.
    UEdGraphPin* FirstElementPin = Arguments.GetInParam_ArrayElement(0);

    switch(ArgumentType)
    {
    case ETSBC_SolidityDataType::Bool:
        {
            // Spawn "BoolToString" node to convert before adding to argument data
            const TSBC_K2NodeWrapper::Conv_BoolToString Conv_BoolToString(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            // Is pin connected?
            if(ArgumentPin->LinkedTo.Num() > 0)
            {
                // Pin connected: Link the argument pin to InBool argument to convert the value to string.
                TSBC_LINK_S2I_MOVE(ArgumentPin, Conv_BoolToString.GetInParam_InBool());
            }
            else
            {
                // Pin disconnected: Copy literal default value.
                Conv_BoolToString.GetInParam_InBool()->DefaultValue = ArgumentPin->DefaultValue;
            }

            // Connect the return value of the "BoolToString" node to the first element in the array.
            TSBC_LINK_I2I(Conv_BoolToString.GetOutParam_ReturnValue(), FirstElementPin);

            break;
        }

    case ETSBC_SolidityDataType::Uint:
        {
            // Spawn "ToHexString" node to convert before adding to argument data
            const TSBC_K2NodeWrapper::Uint256ToHexString ToHexString(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            // Link the argument pin to Value argument to convert the value to string.
            TSBC_LINK_S2I_MOVE(ArgumentPin, ToHexString.GetInParam_Value());
            // Connect the ToHexString return value to the first element of the array.
            TSBC_LINK_I2I(ToHexString.GetOutParam_ReturnValue(), FirstElementPin);

            break;
        }

    case ETSBC_SolidityDataType::Address:
    case ETSBC_SolidityDataType::String:
    case ETSBC_SolidityDataType::Bytes:
        {
            // Is pin connected?
            if(ArgumentPin->LinkedTo.Num() > 0)
            {
                // Pin connected: Link the argument pin to the first element in the array.
                TSBC_LINK_S2I_MOVE(ArgumentPin, FirstElementPin);
            }
            else
            {
                // Pin disconnected: Copy literal default value.
                FirstElementPin->DefaultValue = ArgumentPin->DefaultValue;
            }

            break;
        }

    default:
        {
            checkNoEntry();
            break;
        };
    }
}

void UTSBC_K2Node_EncodeSmartContractAbi::LinkArgumentStructToK2NodePinsArray(
    const CThisK2Node& ThisK2Node,
    const ETSBC_SolidityDataType& ArgumentType,
    const TSBC_K2NodeWrapper::MakeStruct_SolidityValueList& SolidityValueList,
    UEdGraphPin* ArgumentPin)
{
    TSBC_DECLARE_LINK_VARS(ThisK2Node);

    switch(ArgumentType)
    {
    case ETSBC_SolidityDataType::BoolArray:
        {
            bConnectedToLoop = true;
            // Spawn "TemporaryVariable" node to store the array we want to convert.
            const TSBC_K2NodeWrapper::TemporaryVariable_StringArray Temp_StringArray(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_StringArray.GetOutParam_Variable(), SolidityValueList.GetInParam_Values());

            // Spawn "Array_Clear" node.
            const TSBC_K2NodeWrapper::Array_Clear Array_Clear(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_StringArray.GetOutParam_Variable(), Array_Clear.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Clear.GetInParam_Array());

            // Spawn "Array_Add" node.
            const TSBC_K2NodeWrapper::Array_Add Array_Add_String(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            // Spawn "For Each Element" node.
            const TSBC_K2NodeWrapper::ForEachElement ForEachElement(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            TSBC_LINK_I2I(Array_Clear.GetOutExec_Then(), ForEachElement.GetInExec());

            if(LastLoop_OutExec_Completed == nullptr)
            {
                FirstLoop_InExec = Array_Clear.GetInExec();
            }
            else
            {
                // Connect execution pins and the array we will iterate on.
                TSBC_LINK_I2I(LastLoop_OutExec_Completed, Array_Clear.GetInExec());
            }
            LastLoop_OutExec_Completed = ForEachElement.GetOutExec_Completed();

            TSBC_LINK_S2I_MOVE(ArgumentPin, ForEachElement.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(ForEachElement.GetInParam_Array());

            // Connect "Array_Add" pins
            TSBC_LINK_I2I(ForEachElement.GetOutExec_LoopBody(), Array_Add_String.GetInExec());
            TSBC_LINK_I2I(Temp_StringArray.GetOutParam_Variable(), Array_Add_String.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Add_String.GetInParam_Array());

            // Spawn "BoolToString" node.
            const TSBC_K2NodeWrapper::Conv_BoolToString Conv_BoolToString(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(ForEachElement.GetOutParam_ArrayElement(), Conv_BoolToString.GetInParam_InBool());
            TSBC_LINK_I2I(Conv_BoolToString.GetOutParam_ReturnValue(), Array_Add_String.GetInParam_NewItem());

            break;
        }

    case ETSBC_SolidityDataType::UintArray:
        {
            bConnectedToLoop = true;
            // Spawn "TemporaryVariable" node to store the array we want to convert.
            const TSBC_K2NodeWrapper::TemporaryVariable_StringArray Temp_StringArray(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_StringArray.GetOutParam_Variable(), SolidityValueList.GetInParam_Values());

            // Spawn "Array_Clear" node.
            const TSBC_K2NodeWrapper::Array_Clear Array_Clear(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_StringArray.GetOutParam_Variable(), Array_Clear.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Clear.GetInParam_Array());

            // Spawn "For Each Element" node.
            const TSBC_K2NodeWrapper::ForEachElement ForEachElement(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            TSBC_LINK_I2I(Array_Clear.GetOutExec_Then(), ForEachElement.GetInExec());

            if(LastLoop_OutExec_Completed == nullptr)
            {
                FirstLoop_InExec = Array_Clear.GetInExec();
            }
            else
            {
                // Connect execution pins and the array we will iterate on.
                TSBC_LINK_I2I(LastLoop_OutExec_Completed, Array_Clear.GetInExec());
            }
            LastLoop_OutExec_Completed = ForEachElement.GetOutExec_Completed();

            TSBC_LINK_S2I_MOVE(ArgumentPin, ForEachElement.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(ForEachElement.GetInParam_Array());

            // Spawn "Array_Add" node.
            const TSBC_K2NodeWrapper::Array_Add Array_Add_String(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            // Connect "Array_Add" pins.
            TSBC_LINK_I2I(ForEachElement.GetOutExec_LoopBody(), Array_Add_String.GetInExec());
            TSBC_LINK_I2I(Temp_StringArray.GetOutParam_Variable(), Array_Add_String.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Add_String.GetInParam_Array());

            // Spawn "uint256 to Hex String" node.
            const TSBC_K2NodeWrapper::Uint256ToHexString ToHexString(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(ForEachElement.GetOutParam_ArrayElement(), ToHexString.GetInParam_Value());
            TSBC_LINK_I2I(ToHexString.GetOutParam_ReturnValue(), Array_Add_String.GetInParam_NewItem());

            break;
        }

    // Default string data array will be linked directly.
    case ETSBC_SolidityDataType::AddressArray:
    case ETSBC_SolidityDataType::StringArray:
    case ETSBC_SolidityDataType::BytesArray:
        {
            TSBC_LINK_S2I_MOVE(ArgumentPin, SolidityValueList.GetInParam_Values());
            break;
        }

    default:
        {
            checkNoEntry();
            break;
        };
    }
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::GetFunctionNamePin() const
{
    UEdGraphPin* Pin = FindPinChecked(InputParamPinName_FunctionName);
    check(Pin->Direction == EGPD_Input);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::GetEncodingSuccessfulPin() const
{
    UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    check(Pin->Direction == EGPD_Output);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::GetEncodingFailedPin() const
{
    UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Else);
    check(Pin->Direction == EGPD_Output);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::FindOutputStructPinChecked(UEdGraphNode* Node)
{
    check(Node != nullptr);
    UEdGraphPin* OutputPin = nullptr;
    for(int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
    {
        UEdGraphPin* Pin = Node->Pins[PinIndex];
        if(Pin && (EGPD_Output == Pin->Direction))
        {
            OutputPin = Pin;
            break;
        }
    }
    check(OutputPin != nullptr);
    return OutputPin;
}

UEdGraphPin* UTSBC_K2Node_EncodeSmartContractAbi::GetContractAbiDataAssetPin() const
{
    UEdGraphPin* Pin = FindPinChecked(InputParamPinName_ContractAbiDataAsset);
    check(Pin->Direction == EGPD_Input);
    return Pin;
}

void UTSBC_K2Node_EncodeSmartContractAbi::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
    // actions get registered under specific object-keys; the idea is that 
    // actions might have to be updated (or deleted) if their object-key is  
    // mutated (or removed)... here we use the node's class (so if the node 
    // type disappears, then the action should go with it)
    const UClass* ActionKey = GetClass();
    // to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
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

FText UTSBC_K2Node_EncodeSmartContractAbi::GetMenuCategory() const
{
    return LOCTEXT("MenuCategory_K2Node_EncodeABI", "3Studio|Blockchain|Ethereum|ABI");
}

#undef LOCTEXT_NAMESPACE