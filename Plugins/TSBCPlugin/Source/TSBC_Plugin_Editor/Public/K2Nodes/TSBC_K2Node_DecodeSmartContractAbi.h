// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "K2Node.h"
#include "TSBC_K2NodeWrapper.h"
#include "Data/TSBC_ContractAbiTypes.h"
#include "EdGraph/EdGraphPin.h"

#include "TSBC_K2Node_DecodeSmartContractAbi.generated.h"

class FBlueprintActionDatabaseRegistrar;

/**
 * Decodes the resulting data returned from an Ethereum RPC-call.
 */
UCLASS()
class TSBC_PLUGIN_EDITOR_API UTSBC_K2Node_DecodeSmartContractAbi : public UK2Node
{
    GENERATED_BODY()

private:
    class CThisK2Node : public TSBC_K2NodeWrapper::CBaseThisK2Node<UTSBC_K2Node_DecodeSmartContractAbi>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_ContractAbiDataAsset = nullptr;
        UEdGraphPin* InParam_FunctionName = nullptr;
        UEdGraphPin* InParam_DataToDecode = nullptr;

        UEdGraphPin* OutExec_Successful = nullptr;
        UEdGraphPin* OutExec_Failed = nullptr;

        UEdGraphPin* OutParam_ErrorMessage = nullptr;

    public:
        CThisK2Node(
            const UEdGraphSchema_K2* InSchema,
            FKismetCompilerContext& InCompilerContext,
            UEdGraph* InSourceGraph,
            UTSBC_K2Node_DecodeSmartContractAbi* InNode)
            : CBaseThisK2Node(InSchema, InCompilerContext, InSourceGraph, InNode)
        {
            InExec = Node->GetExecPin();

            InParam_ContractAbiDataAsset = Node->GetContractAbiDataAssetPin();
            InParam_FunctionName = Node->GetFunctionNamePin();
            InParam_DataToDecode = Node->GetDataToDecodePin();

            OutExec_Successful = Node->GetDecodingSuccessfulPin();
            OutExec_Failed = Node->GetDecodingFailedPin();

            OutParam_ErrorMessage = Node->FindPinChecked(OutParamName_ErrorMessage);
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_ContractAbiDataAsset() const { return InParam_ContractAbiDataAsset; }
        UEdGraphPin* GetInParam_FunctionName() const { return InParam_FunctionName; }
        UEdGraphPin* GetInParam_DataToDecode() const { return InParam_DataToDecode; }

        UEdGraphPin* GetOutExec_Successful() const { return OutExec_Successful; }
        UEdGraphPin* GetOutExec_Failed() const { return OutExec_Failed; }

        UEdGraphPin* GetOutParam_ErrorMessage() const { return OutParam_ErrorMessage; }
    };

public:
    // This is public to let the function selector pin to access the function names from the contract abi data asset.
    UPROPERTY()
    FTSBC_ContractAbi ContractAbi;

private:
    static const FName InParamName_ContractAbiDataAsset;
    static const FName InParamName_FunctionName;
    static const FName InParamName_DataToDecode;

    static const FName OutParamName_DecodedAbiValues;
    static const FName OutParamName_ErrorMessage;

    // Serialized data of the currently selected Contract ABI function.
    UPROPERTY()
    FString CurrentSerializedAbiFunctionName;

    // Currently selected Contract ABI function.
    UPROPERTY()
    FTSBC_ContractAbiFunction CurrentAbiFunction;

    // The output pin names of this K2Node.
    UPROPERTY()
    TArray<FName> OutputNamesPins;

    // The output pin count of the currently selected Contract ABI function.
    UPROPERTY()
    int32 OutputCount = 0;

    // Check if the ThisK2Node.GetInExec() is connected to a For Each Element node.
    UPROPERTY()
    bool bConnectedToLoop = false;

    TArray<UEdGraphPin*> OutputPins;

    FText NodeTooltip;

    // Stores the exec pin of the first For Each Element conversion node. 
    UEdGraphPin* FirstLoop_InExec;

    // Stores the completed pin of the last For Each Element conversion node. 
    UEdGraphPin* LastLoop_OutExec_Completed;

public:
    UTSBC_K2Node_DecodeSmartContractAbi(const FObjectInitializer& ObjectInitializer);

    //~ Begin UEdGraphNode Interface.
    virtual void AllocateDefaultPins() override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
    virtual FText GetTooltipText() const override;
    //~ End UEdGraphNode Interface.

    //~ Begin UK2Node Interface.
    virtual bool IsNodePure() const override { return false; }
    virtual bool IsConnectionDisallowed(
        const UEdGraphPin* MyPin,
        const UEdGraphPin* OtherPin,
        FString& OutReason) const override;
    virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
    virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    virtual FText GetMenuCategory() const override;
    //~ End UK2Node Interface.

private:
    // Find argument pin by pin name which should not be the format pin or an output pin.
    UEdGraphPin* FindOutputPinByName(const FName InPinName) const;

    // Find argument pin by object pin which should not be the format pin or an output pin.
    UEdGraphPin* FindOutputPin(const UEdGraphPin* InPin) const;

    // Get contract ABI pin.
    UEdGraphPin* GetContractAbiDataAssetPin() const;

    // Get Function Name pin.
    UEdGraphPin* GetFunctionNamePin() const;

    // Get data to decode Name pin.
    UEdGraphPin* GetDataToDecodePin() const;

    // Get Then pin.
    UEdGraphPin* GetDecodingSuccessfulPin() const;

    // Get Decoding Failed Pin.
    UEdGraphPin* GetDecodingFailedPin() const;

    // Find the output pin for the "Make Struct" node
    static UEdGraphPin* FindInputStructPinChecked(UEdGraphNode* Node);

    // Create Arguments pins needed
    void CreateOutputPins();

    // Create single Argument pin
    UEdGraphPin* CreateOutputPin(
        const FString& PinName,
        const FString& PinType,
        const ETSBC_SolidityDataType& PinDataType);

    // Remove all arguments pins.
    void RemoveOutputPins();

    void LinkOutputStructToK2NodePins(
        const CThisK2Node& ThisK2Node,
        const ETSBC_SolidityDataType OutputType,
        UEdGraphPin* OutputPin,
        const TSBC_K2NodeWrapper::Branch& Branch,
        const TSBC_K2NodeWrapper::BreakStruct_SolidityValueList& SolidityValueList);

    void LinkOutputStructToK2NodePinsArray(
        const CThisK2Node& ThisK2Node,
        const ETSBC_SolidityDataType OutputType,
        UEdGraphPin* OutputPin,
        const TSBC_K2NodeWrapper::Branch& Branch,
        const TSBC_K2NodeWrapper::BreakStruct_SolidityValueList& SolidityValueList);
};