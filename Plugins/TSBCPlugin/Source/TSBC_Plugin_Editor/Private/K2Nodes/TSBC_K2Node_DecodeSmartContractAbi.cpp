// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "K2Nodes/TSBC_K2Node_DecodeSmartContractAbi.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Data/TSBC_EditorTypes.h"
#include "DataAssets/TSBC_ContractAbiDataAsset.h"
#include "Encoding/TSBC_ContractAbiHelper.h"
#include "K2Nodes/TSBC_K2NodeWrapper.h"
#include "Math/TSBC_uint256FunctionLibrary.h"
#include "Util/TSBC_TypeUtils.h"

#define LOCTEXT_NAMESPACE "TSBC_K2Node_DecodeSmartContractAbi"

const FName UTSBC_K2Node_DecodeSmartContractAbi::InParamName_ContractAbiDataAsset(TEXT("ContractAbiDataAsset"));
const FName UTSBC_K2Node_DecodeSmartContractAbi::InParamName_FunctionName(TEXT("FunctionName"));
const FName UTSBC_K2Node_DecodeSmartContractAbi::InParamName_DataToDecode(TEXT("DataToDecode"));

const FName UTSBC_K2Node_DecodeSmartContractAbi::OutParamName_DecodedAbiValues(TEXT("DecodedAbiValues"));
const FName UTSBC_K2Node_DecodeSmartContractAbi::OutParamName_ErrorMessage(TEXT("ErrorMessage"));

UTSBC_K2Node_DecodeSmartContractAbi::UTSBC_K2Node_DecodeSmartContractAbi(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NodeTooltip = LOCTEXT(
        "NodeTooltip",
        "Decodes the resulting data returned from an Ethereum RPC-call");
}

void UTSBC_K2Node_DecodeSmartContractAbi::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();

    // In: Execution pin
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

    // Out: Success execution pin
    UEdGraphPin* ExecuteDecodeSuccessPin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_Exec,
        UEdGraphSchema_K2::PN_Then);
    ExecuteDecodeSuccessPin->PinFriendlyName = LOCTEXT("Successful Exec pin", "Successful");

    // Out: Failed execution pin
    UEdGraphPin* ExecuteDecodeFailedPin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_Exec,
        UEdGraphSchema_K2::PN_Else);
    ExecuteDecodeFailedPin->PinFriendlyName = LOCTEXT("Failed Exec pin", "Failed");

    // In: Contract ABI Data Asset pin
    UEdGraphPin* ContractAbiPin = CreatePin(
        EGPD_Input,
        UEdGraphSchema_K2::PC_Object,
        UTSBC_ContractAbiDataAsset::StaticClass(),
        InParamName_ContractAbiDataAsset);
    ContractAbiPin->bNotConnectable = true;
    ContractAbiPin->PinFriendlyName = LOCTEXT("Contract ABI Data Asset pin", "Contract ABI Data Asset");
    ContractAbiPin->PinToolTip = LOCTEXT(
        "ContractAbiPin_Tooltip",
        "The Contract ABI").ToString();

    // In: FunctionName pin
    UScriptStruct* FunctionNameScriptStruct =
        TSBC_TypeUtils::FindStructSafe<FTSBC_ContractAbiFunctionNamePin>(nullptr);
    UEdGraphPin* FunctionNamePin = CreatePin(
        EGPD_Input,
        UEdGraphSchema_K2::PC_Struct,
        FunctionNameScriptStruct,
        InParamName_FunctionName);
    FunctionNamePin->bNotConnectable = true;
    FunctionNamePin->PinToolTip = LOCTEXT(
        "FunctionNamePin_Tooltip",
        "The function name to hash").ToString();

    // In: Data to Decode pin
    UEdGraphPin* DataToDecodePin = CreatePin(
        EGPD_Input,
        UEdGraphSchema_K2::PC_String,
        InParamName_DataToDecode);
    DataToDecodePin->PinToolTip = LOCTEXT(
        "DataToDecodePin_Tooltip",
        "The data to decode").ToString();

    // Out: Error Message pin
    UEdGraphPin* ErrorMessagePin = CreatePin(
        EGPD_Output,
        UEdGraphSchema_K2::PC_String,
        OutParamName_ErrorMessage);
    ErrorMessagePin->PinToolTip = LOCTEXT(
        "ErrorMessagePin_Tooltip",
        "Contains an error message in case the operation fails. Otherwise, it will be empty").ToString();

    // Create output pins if we already have a spawned node with Contract ABI and function set.
    CreateOutputPins();
}

FText UTSBC_K2Node_DecodeSmartContractAbi::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("K2Node_DecodeABI_Title", "Decode Smart Contract ABI");
}

void UTSBC_K2Node_DecodeSmartContractAbi::PinDefaultValueChanged(UEdGraphPin* Pin)
{
    const UEdGraphPin* ContractAbiDataAssetPin = GetContractAbiDataAssetPin();
    UEdGraphPin* FunctionNamePin = GetFunctionNamePin();

    // When the default value of FunctionName pin was changed remove old pins and create new ones.
    if(Pin == FunctionNamePin && FunctionNamePin->LinkedTo.Num() == 0)
    {
        // Save the currently selected value of FunctionName pin for creation of its output pins.
        CurrentSerializedAbiFunctionName = FunctionNamePin->DefaultValue;
        if(FunctionNamePin->DefaultValue == Pin->DefaultValue)
        {
            CreateOutputPins();
            GetGraph()->NotifyGraphChanged();
        }
        return;
    }

    // Set the Contract ABI data from newly selected Contract ABI Data Asset.
    if(Pin == ContractAbiDataAssetPin && ContractAbiDataAssetPin->LinkedTo.Num() == 0)
    {
        ContractAbi = FTSBC_ContractAbi{};
        const UTSBC_ContractAbiDataAsset* ContractDataAsset = Cast<UTSBC_ContractAbiDataAsset>(
            ContractAbiDataAssetPin->DefaultObject);
        // Reset FunctionName pin to its default value and mark it as modified.
        FunctionNamePin->ResetDefaultValue();
        FunctionNamePin->Modify();

        RemoveOutputPins();
        if(IsValid(ContractDataAsset))
        {
            ContractAbi = ContractDataAsset->ContractAbi;
            CreateOutputPins();
        }
        GetGraph()->NotifyGraphChanged();
    }
}

FText UTSBC_K2Node_DecodeSmartContractAbi::GetTooltipText() const
{
    return NodeTooltip;
}

bool UTSBC_K2Node_DecodeSmartContractAbi::IsConnectionDisallowed(
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

void UTSBC_K2Node_DecodeSmartContractAbi::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);

    const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
    check(this && SourceGraph && Schema);

    const CThisK2Node ThisK2Node(Schema, CompilerContext, SourceGraph, this);

    const UTSBC_ContractAbiDataAsset* ContractAbiDataAsset =
        ThisK2Node.GetInParam_ContractAbiDataAsset() != nullptr
        ? Cast<UTSBC_ContractAbiDataAsset>(ThisK2Node.GetInParam_ContractAbiDataAsset()->DefaultObject)
        : nullptr;

    if(ThisK2Node.GetInParam_ContractAbiDataAsset() == nullptr
       || (ContractAbiDataAsset == nullptr && ThisK2Node.GetInParam_ContractAbiDataAsset()->LinkedTo.Num() == 0))
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT("DecodeABINoAsset_Error", "Decode ABI must have a DataAsset specified in @@").ToString(),
            this);
        // We break exec links so that this is the only error we get.
        BreakAllNodeLinks();
        return;
    }

    // Check if FunctionName pin has an actual value and is not empty or default parsed struct member.
    if(FTSBC_ContractAbiFunctionNamePin::IsValueEmptyOrDefault(*ThisK2Node.GetInParam_FunctionName()))
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT("DecodeABINoFunctionName_Error", "Function Name must must be selected in @@").ToString(),
            this);

        // We break exec links so that this is the only error we get.
        BreakAllNodeLinks();
        return;
    }

    // Check if FunctionName pin has an actual value and is not empty or default parsed struct member.
    if(ThisK2Node.GetInParam_DataToDecode()->DefaultValue == ""
       && ThisK2Node.GetInParam_DataToDecode()->LinkedTo.Num() == 0)
    {
        CompilerContext.MessageLog.Error(
            *LOCTEXT("DecodeABINoDataToDecode_Error", "No Data found to decode @@").ToString(),
            this);

        // We break exec links so that this is the only error we get.
        BreakAllNodeLinks();
        return;
    }

    TArray<ETSBC_SolidityDataType> OutputTypes;
    bool bContainsTuple;
    CTSBC_ContractAbiHelper::GetSolidityDataTypes(
        CurrentAbiFunction.Outputs,
        OutputTypes,
        bContainsTuple);

    if(bContainsTuple && (OutputTypes.Num() > 1 || OutputTypes.Num() == 0))
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
        OutputTypes.Empty();
        bool bDynamic;
        CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
            CurrentAbiFunction.Inputs[0].TupleVariables,
            bDynamic,
            OutputTypes);
    }

    // This is the node that does all the decoding work.
    const TSBC_K2NodeWrapper::DecodeSmartContractAbi DecodeAbi(
        TSBC_K2NODE_WRAPPER_DEFAULT_ARGS,
        ThisK2Node.GetInParam_ContractAbiDataAsset()->DefaultObject,
        FTSBC_ContractAbiFunctionNamePin::Deserialize(ThisK2Node.GetInParam_FunctionName()->DefaultValue));

    // Connect the input exec pin of this K2 node function's input exec pin.
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetInExec(), DecodeAbi.GetInExec());

    // Connect the "DataToDecode" pin of this K2 node function's data to decode pin.
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetInParam_DataToDecode(), DecodeAbi.GetInParam_DataToDecode());

    // Spawn a "Branch" node.
    const TSBC_K2NodeWrapper::Branch Branch(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
    TSBC_LINK_I2I(DecodeAbi.GetOutExec_Then(), Branch.GetInExec());
    TSBC_LINK_I2I(DecodeAbi.GetOutParam_bSuccess(), Branch.GetInParam_Condition());

    // Hook up output param pins
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutParam_ErrorMessage(), DecodeAbi.GetOutParam_ErrorMessage());

    // Hook up exec pins
    TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_Failed(), Branch.GetOutExec_False());

    /*
     * For each Output, we need to spawn a "Get Array Item" node then spawn and connect it to a "Break Struct" node,
     * which will be connected to the output pin directly if container type array, or spawn another "Get Array Item"
     * and get the first element from this array and connect it to the output pin.
     */
    for(int32 OutputIdx = 0; OutputIdx < OutputNamesPins.Num(); ++OutputIdx)
    {
        // Create a "Get Array Item" node to keep the decoded ABI value in this node to access elements
        const TSBC_K2NodeWrapper::GetArrayItem StructGetArrayItemNode(
            TSBC_K2NODE_WRAPPER_DEFAULT_ARGS,
            FString::FromInt(OutputIdx));

        // Connect the input pin of the "Get Array Item" to the function's "DecodedAbiValues" pin
        TSBC_LINK_I2I(StructGetArrayItemNode.GetInParam_Array(), DecodeAbi.GetOutParam_DecodedAbiValues());
        // Find the output pin from the get array item, it will be linked to the first element in the array by default
        StructGetArrayItemNode.GetOutParam_ReturnValue()->PinType.bIsReference = true;
        // This will set the "Get Array Item" node's type, only works if one pin is connected.
        TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(StructGetArrayItemNode.GetInParam_Array());

        // Spawn a "Break Struct" node to create the struct needed TSBC_SolidityValueList.
        const TSBC_K2NodeWrapper::BreakStruct_SolidityValueList SolidityValueList(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

        // Make link from the "Get Array Item" to "TSBC_SolidityValueList" struct
        TSBC_LINK_I2I(StructGetArrayItemNode.GetOutParam_ReturnValue(), SolidityValueList.GetInParam_Struct());

        // Move the connection of the output pin to the correct output value pin, and also set the correct
        // output pin type based on the pin that was hooked up.
        const ETSBC_SolidityDataType OutputType = OutputTypes[OutputIdx];
        UEdGraphPin* OutputPin = FindOutputPinByName(OutputNamesPins[OutputIdx]);
        switch(OutputPin->PinType.ContainerType)
        {
        case EPinContainerType::None:
            {
                LinkOutputStructToK2NodePins(
                    ThisK2Node,
                    OutputType,
                    OutputPin,
                    Branch,
                    SolidityValueList);
                break;
            }

        case EPinContainerType::Array:
            {
                LinkOutputStructToK2NodePinsArray(
                    ThisK2Node,
                    OutputType,
                    OutputPin,
                    Branch,
                    SolidityValueList);
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
    }

    if(bConnectedToLoop)
    {
        TSBC_LINK_I2I(Branch.GetOutExec_True(), FirstLoop_InExec);
        TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_Successful(), LastLoop_OutExec_Completed);
    }
    else
    {
        TSBC_LINK_S2I_MOVE(ThisK2Node.GetOutExec_Successful(), Branch.GetOutExec_True());
    }

    BreakAllNodeLinks();
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::FindOutputPinByName(const FName InPinName) const
{
    const UEdGraphPin* FunctionNamePin = GetFunctionNamePin();
    for(UEdGraphPin* Pin : Pins)
    {
        if(Pin != FunctionNamePin && Pin->Direction != EGPD_Input)
        {
            if(Pin->PinName.ToString().Equals(InPinName.ToString(), ESearchCase::CaseSensitive))
            {
                return Pin;
            }
        }
    }

    return nullptr;
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::FindOutputPin(const UEdGraphPin* InPin) const
{
    const UEdGraphPin* FunctionNamePin = GetFunctionNamePin();
    for(UEdGraphPin* Pin : Pins)
    {
        if(Pin == InPin && Pin != FunctionNamePin && Pin->Direction != EGPD_Input)
        {
            return Pin;
        }
    }

    return nullptr;
}

void UTSBC_K2Node_DecodeSmartContractAbi::CreateOutputPins()
{
    for(const FTSBC_ContractAbiFunction Function : ContractAbi.ContractAbiFunctions)
    {
        FString SelectedFunction = FTSBC_ContractAbiFunctionNamePin::Deserialize(CurrentSerializedAbiFunctionName);
        if(Function.Name != SelectedFunction)
        {
            continue;
        }

        CurrentAbiFunction = Function;

        // Clear the Output pin array before creating new pins.
        RemoveOutputPins();

        OutputCount = Function.Outputs.Num();
        TArray<ETSBC_SolidityDataType> OutputTypes;
        bool bContainsTuple;
        CTSBC_ContractAbiHelper::GetSolidityDataTypes(CurrentAbiFunction.Outputs, OutputTypes, bContainsTuple);
        if(bContainsTuple && OutputTypes.Num() > 1)
        {
            return;
        }

        for(int32 TypeIndex = 0; TypeIndex < OutputTypes.Num(); TypeIndex++)
        {
            if(bContainsTuple)
            {
                OutputTypes.Empty();
                bool bDynamic;
                CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
                    CurrentAbiFunction.Inputs[0].TupleVariables,
                    bDynamic,
                    OutputTypes);

                for(int32 TupleIndex = 0; TupleIndex < OutputTypes.Num(); TupleIndex++)
                {
                    const FTSBC_SolidityVariable TupleInput = Function.Outputs[TypeIndex].TupleVariables[TupleIndex];

                    UEdGraphPin* OutputPin = CreateOutputPin(
                        TupleInput.Name,
                        TupleInput.Type,
                        OutputTypes[TupleIndex]);
                    OutputPins.Add(OutputPin);
                    OutputNamesPins.Add(OutputPin->PinName);
                }
                return;
            }

            UEdGraphPin* OutputPin = CreateOutputPin(
                Function.Outputs[TypeIndex].Variable.Name,
                Function.Outputs[TypeIndex].Variable.Type,
                OutputTypes[TypeIndex]);

            OutputPins.Add(OutputPin);
            OutputNamesPins.Add(OutputPin->PinName);
        }
    }
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::CreateOutputPin(
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
            // Use our custom uint256 struct implementation for uint Solidity data type.
            PinCategory = UEdGraphSchema_K2::PC_Struct;
            UScriptStruct* UintScriptStruct = TSBC_TypeUtils::FindStructSafe<FTSBC_uint256>(nullptr);

            return CreatePin(
                EGPD_Output,
                PinCategory,
                UintScriptStruct,
                FName(PinName.IsEmpty() ? "Output" : PinName),
                PinParams);
        }

    default:
        {
            checkNoEntry();
            break;
        };
    }

    return CreatePin(
        EGPD_Output,
        PinCategory,
        FName(PinName.IsEmpty() ? "Output" : PinName),
        PinParams);
}

void UTSBC_K2Node_DecodeSmartContractAbi::RemoveOutputPins()
{
    // Clear all output pins
    for(UEdGraphPin* Output : OutputPins)
    {
        Output->MarkAsGarbage();
        Pins.Remove(Output);
    }
    OutputPins.Empty();
    OutputNamesPins.Empty();
}

void UTSBC_K2Node_DecodeSmartContractAbi::LinkOutputStructToK2NodePins(
    const CThisK2Node& ThisK2Node,
    const ETSBC_SolidityDataType OutputType,
    UEdGraphPin* OutputPin,
    const TSBC_K2NodeWrapper::Branch& Branch,
    const TSBC_K2NodeWrapper::BreakStruct_SolidityValueList& SolidityValueList)
{
    TSBC_DECLARE_LINK_VARS(ThisK2Node);

    // Create a "Get Array Item" node to get the first element from the array of the Argument struct
    const TSBC_K2NodeWrapper::GetArrayItem GetArrayItem_String(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

    // Connect the Input of the "Get Array Item" pin to the struct's "Values" pin
    TSBC_LINK_I2I(GetArrayItem_String.GetInParam_Array(), SolidityValueList.GetOutParam_Values());
    GetArrayItem_String.GetOutParam_ReturnValue()->PinType.bIsReference = true;
    // This will set the "Get Array Item" node's type, only works if one pin is connected.
    TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(GetArrayItem_String.GetInParam_Array());

    switch(OutputType)
    {
    case ETSBC_SolidityDataType::Bool:
        {
            // String to bool conversion
            const TSBC_K2NodeWrapper::Conv_StringToBool Conv_StringToBool(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Conv_StringToBool.GetInParam_String(), GetArrayItem_String.GetOutParam_ReturnValue());
            TSBC_LINK_S2I_MOVE(OutputPin, Conv_StringToBool.GetOutParam_ReturnValue());
            break;
        }

    case ETSBC_SolidityDataType::Uint:
        {
            // String to Uint256 conversion
            const TSBC_K2NodeWrapper::MakeLiteralUint256FromString StringToUint256(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(GetArrayItem_String.GetOutParam_ReturnValue(), StringToUint256.GetInParam_InValue());
            TSBC_LINK_S2I_MOVE(OutputPin, StringToUint256.GetOutParam_OutValue());
            break;
        }

    case ETSBC_SolidityDataType::Address:
    case ETSBC_SolidityDataType::String:
    case ETSBC_SolidityDataType::Bytes:
        {
            TSBC_LINK_S2I_MOVE(OutputPin, GetArrayItem_String.GetOutParam_ReturnValue());
            break;
        }

    default:
        {
            checkNoEntry();
            break;
        };
    }
}

void UTSBC_K2Node_DecodeSmartContractAbi::LinkOutputStructToK2NodePinsArray(
    const CThisK2Node& ThisK2Node,
    const ETSBC_SolidityDataType OutputType,
    UEdGraphPin* OutputPin,
    const TSBC_K2NodeWrapper::Branch& Branch,
    const TSBC_K2NodeWrapper::BreakStruct_SolidityValueList& SolidityValueList)
{
    TSBC_DECLARE_LINK_VARS(ThisK2Node);

    switch(OutputType)
    {
    case ETSBC_SolidityDataType::BoolArray:
        {
            bConnectedToLoop = true;
            // Temporary variable to store the array we want to convert.
            const TSBC_K2NodeWrapper::TemporaryVariable_BoolArray Temp_BoolArray(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            // Spawn "Array Add" node.
            const TSBC_K2NodeWrapper::Array_Add Array_Add(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            // Spawn "Array_Clear" node.
            const TSBC_K2NodeWrapper::Array_Clear Array_Clear(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_BoolArray.GetOutParam_Variable(), Array_Clear.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Clear.GetInParam_Array());

            TSBC_LINK_I2I(Temp_BoolArray.GetOutParam_Variable(), Array_Add.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Add.GetInParam_Array());

            // Spawn "For Each Element" node.
            const TSBC_K2NodeWrapper::ForEachElement ForEachElement(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            TSBC_LINK_I2I(Array_Clear.GetOutExec_Then(), ForEachElement.GetInExec());

            // Connect execution pins and the array we will iterate on
            TSBC_LINK_I2I(SolidityValueList.GetOutParam_Values(), ForEachElement.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(ForEachElement.GetInParam_Array());
            if(FirstLoop_InExec == nullptr)
            {
                FirstLoop_InExec = Array_Clear.GetInExec();
            }
            else
            {
                TSBC_LINK_I2I(LastLoop_OutExec_Completed, Array_Clear.GetInExec());
            }
            LastLoop_OutExec_Completed = ForEachElement.GetOutExec_Completed();
            // Array_add connection
            TSBC_LINK_I2I(ForEachElement.GetOutExec_LoopBody(), Array_Add.GetInExec());

            // String to bool conversion
            const TSBC_K2NodeWrapper::Conv_StringToBool Conv_StringToBool(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(ForEachElement.GetOutParam_ArrayElement(), Conv_StringToBool.GetInParam_String());
            TSBC_LINK_I2I(Conv_StringToBool.GetOutParam_ReturnValue(), Array_Add.GetInParam_NewItem());

            TSBC_LINK_S2I_MOVE(OutputPin, Temp_BoolArray.GetOutParam_Variable());

            break;
        }

    case ETSBC_SolidityDataType::UintArray:
        {
            bConnectedToLoop = true;
            // Temporary variable to store the array we want to convert.
            const TSBC_K2NodeWrapper::TemporaryVariable_Uint256Array
                Temp_Uint256Array(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            // Spawn "Array Add" node.
            const TSBC_K2NodeWrapper::Array_Add Array_Add(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_Uint256Array.GetOutParam_Variable(), Array_Add.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Add.GetInParam_Array());

            // Spawn "Array_Clear" node.
            const TSBC_K2NodeWrapper::Array_Clear Array_Clear(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(Temp_Uint256Array.GetOutParam_Variable(), Array_Clear.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Array_Clear.GetInParam_Array());

            // Spawn "For Each Element" node.
            const TSBC_K2NodeWrapper::ForEachElement ForEachElement(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);

            TSBC_LINK_I2I(Array_Clear.GetOutExec_Then(), ForEachElement.GetInExec());

            TSBC_LINK_I2I(SolidityValueList.GetOutParam_Values(), ForEachElement.GetInParam_Array());
            TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(ForEachElement.GetInParam_Array());

            if(FirstLoop_InExec == nullptr)
            {
                FirstLoop_InExec = Array_Clear.GetInExec();
            }
            else
            {
                // Connect execution pins and the array we will iterate on
                TSBC_LINK_I2I(LastLoop_OutExec_Completed, Array_Clear.GetInExec());
            }
            LastLoop_OutExec_Completed = ForEachElement.GetOutExec_Completed();
            // Array_add connection
            TSBC_LINK_I2I(ForEachElement.GetOutExec_LoopBody(), Array_Add.GetInExec());

            // String to Uint256 conversion
            const TSBC_K2NodeWrapper::MakeLiteralUint256FromString StringToUint256(TSBC_K2NODE_WRAPPER_DEFAULT_ARGS);
            TSBC_LINK_I2I(ForEachElement.GetOutParam_ArrayElement(), StringToUint256.GetInParam_InValue());
            TSBC_LINK_I2I(StringToUint256.GetOutParam_OutValue(), Array_Add.GetInParam_NewItem());

            TSBC_LINK_S2I_MOVE(OutputPin, Temp_Uint256Array.GetOutParam_Variable());

            break;
        }

    // Default string data array will be linked directly.
    case ETSBC_SolidityDataType::AddressArray:
    case ETSBC_SolidityDataType::StringArray:
    case ETSBC_SolidityDataType::BytesArray:
        {
            // Connect the output pin of this K2 node function's output pin.
            TSBC_LINK_S2I_MOVE(OutputPin, SolidityValueList.GetOutParam_Values());
            break;
        }

    default:
        {
            checkNoEntry();
            break;
        };
    }
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::GetFunctionNamePin() const
{
    UEdGraphPin* Pin = FindPinChecked(InParamName_FunctionName);
    check(Pin->Direction == EGPD_Input);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::GetDataToDecodePin() const
{
    UEdGraphPin* Pin = FindPinChecked(InParamName_DataToDecode);
    check(Pin->Direction == EGPD_Input);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::GetDecodingSuccessfulPin() const
{
    UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    check(Pin->Direction == EGPD_Output);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::GetDecodingFailedPin() const
{
    UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Else);
    check(Pin->Direction == EGPD_Output);
    return Pin;
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::FindInputStructPinChecked(UEdGraphNode* Node)
{
    check(Node != nullptr);
    UEdGraphPin* InputPin = nullptr;
    for(int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
    {
        UEdGraphPin* Pin = Node->Pins[PinIndex];
        if(Pin && (EGPD_Input == Pin->Direction) && Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec)
        {
            InputPin = Pin;
            break;
        }
    }
    check(InputPin != nullptr);
    return InputPin;
}

UEdGraphPin* UTSBC_K2Node_DecodeSmartContractAbi::GetContractAbiDataAssetPin() const
{
    UEdGraphPin* Pin = FindPinChecked(InParamName_ContractAbiDataAsset);
    check(Pin->Direction == EGPD_Input);
    return Pin;
}

void UTSBC_K2Node_DecodeSmartContractAbi::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

FText UTSBC_K2Node_DecodeSmartContractAbi::GetMenuCategory() const
{
    return LOCTEXT("MenuCategory_K2Node_DecodeABI", "3Studio|Blockchain|Ethereum|ABI");
}

#undef LOCTEXT_NAMESPACE