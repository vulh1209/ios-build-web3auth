// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "K2Node.h"
#include "Data/TSBC_ContractAbiTypes.h"
#include "EdGraph/EdGraphPin.h"
#include "K2Nodes/TSBC_K2NodeWrapper.h"

#include "TSBC_K2Node_EncodeSmartContractAbi.generated.h"

class FBlueprintActionDatabaseRegistrar;

/**
 * Generates a "Function Selector" with argument encoding for a specific function of an Ethereum Smart Contract.
 */
UCLASS()
class TSBC_PLUGIN_EDITOR_API UTSBC_K2Node_EncodeSmartContractAbi : public UK2Node
{
    GENERATED_BODY()

private:
    class CThisK2Node : public TSBC_K2NodeWrapper::CBaseThisK2Node<UTSBC_K2Node_EncodeSmartContractAbi>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_ContractAbiDataAsset = nullptr;
        UEdGraphPin* InParam_FunctionName = nullptr;

        UEdGraphPin* OutExec_Successful = nullptr;
        UEdGraphPin* OutExec_Failed = nullptr;

        UEdGraphPin* OutParam_ErrorMessage = nullptr;
        UEdGraphPin* OutParam_FunctionHashAndEncodedArguments = nullptr;

    public:
        CThisK2Node(
            const UEdGraphSchema_K2* InSchema,
            FKismetCompilerContext& InCompilerContext,
            UEdGraph* InSourceGraph,
            UTSBC_K2Node_EncodeSmartContractAbi* InNode)
            : CBaseThisK2Node(InSchema, InCompilerContext, InSourceGraph, InNode)
        {
            InExec = Node->GetExecPin();

            InParam_ContractAbiDataAsset = Node->GetContractAbiDataAssetPin();
            InParam_FunctionName = Node->GetFunctionNamePin();

            OutExec_Successful = Node->GetEncodingSuccessfulPin();
            OutExec_Failed = Node->GetEncodingFailedPin();

            OutParam_ErrorMessage = Node->FindPinChecked(OutputParamPinName_ErrorMessage);
            OutParam_FunctionHashAndEncodedArguments = Node->FindPinChecked(
                OutputParamPinName_FunctionHashAndEncodedArguments);
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_ContractAbiDataAsset() const { return InParam_ContractAbiDataAsset; }
        UEdGraphPin* GetInParam_FunctionName() const { return InParam_FunctionName; }

        UEdGraphPin* GetOutExec_Successful() const { return OutExec_Successful; }
        UEdGraphPin* GetOutExec_Failed() const { return OutExec_Failed; }

        UEdGraphPin* GetOutParam_ErrorMessage() const { return OutParam_ErrorMessage; }

        UEdGraphPin* GetOutParam_FunctionHashAndEncodedArguments() const
        {
            return OutParam_FunctionHashAndEncodedArguments;
        }
    };

public:
    // This is public to let the function selector pin to access the function names from the contract abi data asset.
    UPROPERTY()
    FTSBC_ContractAbi ContractAbi;

private:
    static const FName InputParamPinName_ContractAbiDataAsset;
    static const FName InputParamPinName_FunctionName;

    static const FName OutputParamPinName_ErrorMessage;
    static const FName OutputParamPinName_FunctionHashAndEncodedArguments;

    // Serialized data of the currently selected Contract ABI function.
    UPROPERTY()
    FString CurrentSerializedAbiFunctionName;

    // Currently selected Contract ABI function.
    UPROPERTY()
    FTSBC_ContractAbiFunction CurrentAbiFunction;

    // The input argument pin names of this K2Node.
    UPROPERTY()
    TArray<FName> InputArgumentPinNames;

    // The argument count of the currently selected Contract ABI function.
    UPROPERTY()
    int32 ArgumentCount = 0;

    // Check if the ThisK2Node.GetInExec() is connected to a For Each Element node.
    UPROPERTY()
    bool bConnectedToLoop = false;

    // The argument pins of the currently selected Contract ABI function. 
    TArray<UEdGraphPin*> ArgumentPins;

    // Localized tooltip for this K2Node.
    FText NodeTooltip;

    // Stores the exec pin of the first For Each Element conversion node. 
    UEdGraphPin* FirstLoop_InExec;

    // Stores the completed pin of the last For Each Element conversion node. 
    UEdGraphPin* LastLoop_OutExec_Completed;

public:
    UTSBC_K2Node_EncodeSmartContractAbi(const FObjectInitializer& ObjectInitializer);

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
    virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    virtual FText GetMenuCategory() const override;
    //~ End UK2Node Interface.

private:
    // Finds the argument pin of this K2Node by name which should not be the function pin or an output pin.
    UEdGraphPin* FindInputArgumentPinByName(const FName InPinName) const;

    // Finds the argument pin of this K2Node by object which should not be the function pin or an output pin.
    UEdGraphPin* FindArgumentPin(const UEdGraphPin* InPin) const;

    // Gets the Contract ABI Data Asset pin.
    UEdGraphPin* GetContractAbiDataAssetPin() const;

    // Gets the Function Name pin.
    UEdGraphPin* GetFunctionNamePin() const;

    // Gets the Then pin.
    UEdGraphPin* GetEncodingSuccessfulPin() const;

    // Gets the Encoding Failed Pin.
    UEdGraphPin* GetEncodingFailedPin() const;

    // Finds the output pin for the "Make Struct" node
    static UEdGraphPin* FindOutputStructPinChecked(UEdGraphNode* Node);

    // Creates pins for each argument of the selected function
    void CreateArgumentPins();

    // Creates a function argument pin
    UEdGraphPin* CreateArgumentPin(
        const FString& PinName,
        const FString& PinType,
        const ETSBC_SolidityDataType& PinDataType);

    // Removes all function argument pins.
    void RemoveArgumentPins();

    void SpawnAndLinkArgumentStructs(
        FKismetCompilerContext& CompilerContext,
        UEdGraph* SourceGraph,
        const CThisK2Node& ThisK2Node,
        TArray<ETSBC_SolidityDataType>& ArgumentsTypes,
        const TSBC_K2NodeWrapper::MakeArray& FunctionArguments,
        const TSBC_K2NodeWrapper::EncodeSmartContractAbi& EncodeAbi);

    void LinkArgumentStructToK2NodePins(
        const CThisK2Node& ThisK2Node,
        const ETSBC_SolidityDataType& ArgumentType,
        const TSBC_K2NodeWrapper::MakeStruct_SolidityValueList& SolidityValueList,
        UEdGraphPin* ArgumentPin);

    void LinkArgumentStructToK2NodePinsArray(
        const CThisK2Node& ThisK2Node,
        const ETSBC_SolidityDataType& ArgumentType,
        const TSBC_K2NodeWrapper::MakeStruct_SolidityValueList& SolidityValueList,
        UEdGraphPin* ArgumentPin);
};