// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "K2Node_AssignmentStatement.h"
#include "K2Node_BreakStruct.h"
#include "K2Node_CallArrayFunction.h"
#include "K2Node_ExecutionSequence.h"
#include "K2Node_GetArrayItem.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_MakeArray.h"
#include "K2Node_MakeStruct.h"
#include "K2Node_TemporaryVariable.h"
#include "KismetCompiler.h"
#include "Encoding/TSBC_EncodingFunctionLibrary.h"
#include "K2Nodes/TSBC_K2Node_ForEachElement.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Math/TSBC_uint256FunctionLibrary.h"
#include "Util/TSBC_StringUtilsFunctionLibrary.h"
#include "Util/TSBC_TypeUtils.h"

/**
 * Convenience macro to shorten the argument list when instantiating K2NodeWrapper classes.
 */
#define TSBC_K2NODE_WRAPPER_DEFAULT_ARGS CompilerContext, SourceGraph, this

/**
 * Use this macro to declare a local variables that are required for all TSBC_LINK_* macros.
 * Pass a subclassed <code>CBaseThisK2Node</code> class which implements a suitable interface.
 * 
 * The local scope variables will be named:
 * - Schema
 * - CompilerContext
 * - SourceGraph
 */
#define TSBC_DECLARE_LINK_VARS(ThisK2Node) \
    const UEdGraphSchema_K2* Schema = ThisK2Node.GetSchema(); \
    FKismetCompilerContext& CompilerContext = *ThisK2Node.GetCompilerContext(); \
    UEdGraph* SourceGraph = ThisK2Node.GetSourceGraph()

/**
 * Link intermediate pins of two nodes.
 */
#define TSBC_LINK_I2I(SourcePin, DestinationPin) \
    check(SourcePin && DestinationPin); \
    CompilerContext.GetSchema()->TryCreateConnection(SourcePin, DestinationPin)

/**
 * Link a pin to an intermediate pin by moving the link from the source pin to the destination pin.
 */
#define TSBC_LINK_S2I_MOVE(SourcePin, DestinationPin) \
    check(SourcePin && DestinationPin); \
    CompilerContext.MovePinLinksToIntermediate(*SourcePin, *DestinationPin)

/**
 * Link a pin to an intermediate pin by copying the link from the source pin to the destination pin.
 */
#define TSBC_LINK_S2I_COPY(SourcePin, DestinationPin) \
    check(SourcePin && DestinationPin); \
    CompilerContext.CopyPinLinksToIntermediate(*SourcePin, *DestinationPin)

/**
 * Copy the pin type of the source pin to the destination pin.
 * Afterwards, the owning node of the destination pin is notified of this change.
 */
#define TSBC_COPY_PIN_TYPE(SourcePin, DestinationPin) \
    check(SourcePin && DestinationPin); \
    DestinationPin->PinType = SourcePin->PinType; \
    DestinationPin->GetOwningNode()->PinConnectionListChanged(DestinationPin)

/**
 * Notify the owning node of the pin that its connection list changed. 
 */
#define TSBC_NOTIFY_OWNING_NODE_PIN_CONNECTION_CHANGED(Pin) \
    check(Pin); \
    Pin->GetOwningNode()->PinConnectionListChanged(Pin)

// ReSharper disable CppUE4CodingStandardNamingViolationWarning
namespace TSBC_K2NodeWrapper
{

    template <typename T>
    class CBaseThisK2Node
    {
    protected:
        const UEdGraphSchema_K2* Schema = nullptr;
        FKismetCompilerContext* CompilerContext = nullptr;
        UEdGraph* SourceGraph = nullptr;
        T* Node = nullptr;

    public:
        CBaseThisK2Node(
            const UEdGraphSchema_K2* InSchema,
            FKismetCompilerContext& InCompilerContext,
            UEdGraph* InSourceGraph,
            T* InNode)
        {
            Schema = InSchema;
            CompilerContext = &InCompilerContext;
            SourceGraph = InSourceGraph;
            Node = InNode;
        }

    public:
        const UEdGraphSchema_K2* GetSchema() const { return Schema; }
        FKismetCompilerContext* GetCompilerContext() const { return CompilerContext; }
        UEdGraph* GetSourceGraph() const { return SourceGraph; }
        T* GetNode() { return Node; }
    };

    template <typename T>
    class BaseK2NodeWrapper
    {
    protected:
        FKismetCompilerContext* CompilerContext = nullptr;
        UEdGraph* SourceGraph = nullptr;
        UK2Node* SourceNode = nullptr;
        T* Node = nullptr;

    public:
        BaseK2NodeWrapper(
            FKismetCompilerContext& InCompilerContext,
            UEdGraph* InSourceGraph,
            UK2Node* InSourceNode)
        {
            CompilerContext = &InCompilerContext;
            SourceGraph = InSourceGraph;
            SourceNode = InSourceNode;

            check(SourceNode && SourceGraph);
            Node = CompilerContext->SpawnIntermediateNode<T>(SourceNode, SourceGraph);
        }

        T* GetNode() const { return Node; }
    };

    /**
     * Creates a "Branch" (if-then-else) node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>bool Condition</code> (Default: <code>false</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - True
     * - False
     * 
     * <b>Output Param Pins</b><br>
     * <i>NONE</i>
     */
    class Branch : public BaseK2NodeWrapper<UK2Node_IfThenElse>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_Condition = nullptr;

        UEdGraphPin* OutExec_True = nullptr;
        UEdGraphPin* OutExec_False = nullptr;

    public:
        Branch(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const bool ArgInParamDefault_Condition = false)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_Condition = Node->GetConditionPin();

            // Output exec pins
            OutExec_True = Node->GetThenPin();
            OutExec_False = Node->GetElsePin();

            // Set pin defaults
            InParam_Condition->DefaultValue = ArgInParamDefault_Condition ? "true" : "false";
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_Condition() const { return InParam_Condition; }

        UEdGraphPin* GetOutExec_True() const { return OutExec_True; }
        UEdGraphPin* GetOutExec_False() const { return OutExec_False; }

    };

    /**
     * Creates a "Sequence" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * <i>NONE</i>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Then_1
     * - ...
     * - Then_n
     * 
     * <b>Output Param Pins</b><br>
     * <i>NONE</i>
     */
    class ExecutionSequence : public BaseK2NodeWrapper<UK2Node_ExecutionSequence>
    {
    private:
        UEdGraphPin* InExec = nullptr;

    public:
        ExecutionSequence(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetOutExec_Then(const int32 Index) const { return Node->GetThenPinGivenIndex(Index); }

    };

    /**
     * Creates an "Encode Smart Contract ABI" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>UTSBC_ContractAbiDataAsset ContractAbiDataAsset</code> (Default: <code>NULL</code>)
     * - <code>FString FunctionName</code> (Default: <code>""</code>)
     * - <code>TArray<FTSBC_SolidityValueList> FunctionArguments</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Then
     * 
     * <b>Output Param Pins</b><br>
     * - <code>bool bSuccess</code>
     * - <code>FString ErrorMessage</code>
     * - <code>FString FunctionHashAndEncodedArguments</code>
     */
    class EncodeSmartContractAbi : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_ContractAbiDataAsset = nullptr;
        UEdGraphPin* InParam_FunctionName = nullptr;
        UEdGraphPin* InParam_FunctionArguments = nullptr;

        UEdGraphPin* OutExec_Then = nullptr;

        UEdGraphPin* OutParam_bSuccess = nullptr;
        UEdGraphPin* OutParam_ErrorMessage = nullptr;
        UEdGraphPin* OutParam_FunctionHashAndEncodedArguments = nullptr;

    public:
        EncodeSmartContractAbi(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            UObject* ArgInParamDefault_ContractAbiDataAsset = nullptr,
            FString ArgInParamDefault_FunctionName = "")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UTSBC_EncodingFunctionLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UTSBC_EncodingFunctionLibrary, EncodeAbi));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_ContractAbiDataAsset = Node->FindPinChecked(TEXT("ContractAbiDataAsset"));
            InParam_FunctionName = Node->FindPinChecked(TEXT("FunctionName"));
            InParam_FunctionArguments = Node->FindPinChecked(TEXT("FunctionArguments"));

            // Output exec pins
            OutExec_Then = Node->GetThenPin();

            // Output param pins
            OutParam_bSuccess = Node->FindPinChecked(TEXT("bSuccess"));
            OutParam_ErrorMessage = Node->FindPinChecked(TEXT("ErrorMessage"));
            OutParam_FunctionHashAndEncodedArguments = Node->FindPinChecked(
                TEXT("FunctionHashAndEncodedArguments"));

            // Set pin defaults
            InParam_ContractAbiDataAsset->DefaultObject = ArgInParamDefault_ContractAbiDataAsset;
            InParam_FunctionName->DefaultValue = ArgInParamDefault_FunctionName;
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_ContractAbiDataAsset() const { return InParam_ContractAbiDataAsset; }
        UEdGraphPin* GetInParam_FunctionName() const { return InParam_FunctionName; }
        UEdGraphPin* GetInParam_FunctionArguments() const { return InParam_FunctionArguments; }

        UEdGraphPin* GetOutExec_Then() const { return OutExec_Then; }

        UEdGraphPin* GetOutParam_bSuccess() const { return OutParam_bSuccess; }
        UEdGraphPin* GetOutParam_ErrorMessage() const { return OutParam_ErrorMessage; }

        UEdGraphPin* GetOutParam_FunctionHashAndEncodedArguments() const
        {
            return OutParam_FunctionHashAndEncodedArguments;
        }
    };

    /**
     * Creates a "Decode Smart Contract ABI" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>UTSBC_ContractAbiDataAsset* ContractAbiDataAsset</code> (Default: <code>NULL</code>)
     * - <code>FString FunctionName</code> (Default: <code>""</code>)
     * - <code>FString DataToDecode</code> (Default: <code>""</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Then
     * 
     * <b>Output Param Pins</b><br>
     * - <code>bool bSuccess</code>
     * - <code>FString ErrorMessage</code>
     * - <code>TArray<FTSBC_SolidityValueList> DecodedAbiValues</code>
     */
    class DecodeSmartContractAbi : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_ContractAbiDataAsset = nullptr;
        UEdGraphPin* InParam_FunctionName = nullptr;
        UEdGraphPin* InParam_DataToDecode = nullptr;

        UEdGraphPin* OutExec_Then = nullptr;

        UEdGraphPin* OutParam_bSuccess = nullptr;
        UEdGraphPin* OutParam_ErrorMessage = nullptr;
        UEdGraphPin* OutParam_DecodedAbiValues = nullptr;

    public:
        DecodeSmartContractAbi(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            UObject* ArgInParamDefault_ContractAbiDataAsset = nullptr,
            FString ArgInParamDefault_FunctionName = "",
            FString ArgInParamDefault_DataToDecode = "")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UTSBC_EncodingFunctionLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UTSBC_EncodingFunctionLibrary, DecodeAbi));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_ContractAbiDataAsset = Node->FindPinChecked(TEXT("ContractAbiDataAsset"));
            InParam_FunctionName = Node->FindPinChecked(TEXT("FunctionName"));
            InParam_DataToDecode = Node->FindPinChecked(TEXT("DataToDecode"));

            // Output exec pins
            OutExec_Then = Node->GetThenPin();

            // Output param pins
            OutParam_bSuccess = Node->FindPinChecked(TEXT("bSuccess"));
            OutParam_ErrorMessage = Node->FindPinChecked(TEXT("ErrorMessage"));
            OutParam_DecodedAbiValues = Node->FindPinChecked(TEXT("DecodedABIValues"));

            // Set pin defaults
            InParam_ContractAbiDataAsset->DefaultObject = ArgInParamDefault_ContractAbiDataAsset;
            InParam_FunctionName->DefaultValue = ArgInParamDefault_FunctionName;
            InParam_DataToDecode->DefaultValue = ArgInParamDefault_DataToDecode;
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_ContractAbiDataAsset() const { return InParam_ContractAbiDataAsset; }
        UEdGraphPin* GetInParam_FunctionName() const { return InParam_FunctionName; }
        UEdGraphPin* GetInParam_DataToDecode() const { return InParam_DataToDecode; }

        UEdGraphPin* GetOutExec_Then() const { return OutExec_Then; }

        UEdGraphPin* GetOutParam_bSuccess() const { return OutParam_bSuccess; }
        UEdGraphPin* GetOutParam_ErrorMessage() const { return OutParam_ErrorMessage; }
        UEdGraphPin* GetOutParam_DecodedAbiValues() const { return OutParam_DecodedAbiValues; }
    };

    /**
     * Creates an "Assign" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>TemporaryVariable_Int32 Variable</code>
     * - <code>int32 Value</code> (Default: <code>"0"</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Then
     * 
     * <b>Output Param Pins</b><br>
     * <i>NONE</i>
     */
    class AssignmentStatement : public BaseK2NodeWrapper<UK2Node_AssignmentStatement>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_Variable = nullptr;
        UEdGraphPin* InParam_Value = nullptr;

        UEdGraphPin* OutExec_Then = nullptr;

    public:
        AssignmentStatement(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const FString& ArgInputParamDefault_Value = "0")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_Variable = Node->GetVariablePin();
            InParam_Value = Node->GetValuePin();

            // Output exec pins
            OutExec_Then = Node->GetThenPin();

            // Set pin defaults
            InParam_Value->DefaultValue = ArgInputParamDefault_Value;
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_Variable() const { return InParam_Variable; }
        UEdGraphPin* GetInParam_Value() const { return InParam_Value; }

        UEdGraphPin* GetOutExec_Then() const { return OutExec_Then; }
    };

    /**
     * Creates a Bool to String converter node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>bool InBool</code> (Default: <code>false</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     *
     * <b>Output Param Pins</b><br>
     * - <code>FString ReturnValue</code>
     */
    class Conv_BoolToString : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InParam_InBool = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Conv_BoolToString(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const bool ArgInParamDefault_InBool = false)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UKismetStringLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UKismetStringLibrary, Conv_BoolToString));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_InBool = Node->FindPinChecked(TEXT("InBool"));

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();

            // Set pin defaults
            InParam_InBool->DefaultValue = ArgInParamDefault_InBool ? "true" : "false";
        }

        UEdGraphPin* GetInParam_InBool() const { return InParam_InBool; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates a String to Bool converter node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>FString String</code> (Default: <code>""</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     *
     * <b>Output Param Pins</b><br>
     * - <code>bool ReturnValue</code>
     */
    class Conv_StringToBool : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InParam_String = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Conv_StringToBool(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const FString& ArgInParamDefault_String = "")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UTSBC_StringUtilsFunctionLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UTSBC_StringUtilsFunctionLibrary, StringToBool));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_String = Node->FindPinChecked(TEXT("String"));

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();

            // Set pin defaults
            InParam_String->DefaultValue = ArgInParamDefault_String;
        }

        UEdGraphPin* GetInParam_String() const { return InParam_String; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates an int < int node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>int32 A</code> (Default: <code>0</code>)
     * - <code>int32 B</code> (Default: <code>0</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     *
     * <b>Output Param Pins</b><br>
     * - <code>bool ReturnValue</code>
     */
    class Less_IntInt : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InParam_A = nullptr;
        UEdGraphPin* InParam_B = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Less_IntInt(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const FString& ArgInputParamDefault_A = "0",
            const FString& ArgInputParamDefault_B = "0")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UKismetMathLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UKismetMathLibrary, Less_IntInt));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_A = Node->FindPinChecked(TEXT("A"));
            InParam_B = Node->FindPinChecked(TEXT("B"));

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();

            // Set pin defaults
            InParam_A->DefaultValue = ArgInputParamDefault_A;
            InParam_B->DefaultValue = ArgInputParamDefault_B;
        }

        UEdGraphPin* GetInParam_A() const { return InParam_A; }
        UEdGraphPin* GetInParam_B() const { return InParam_B; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates an int32 + int32 node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>int32 A</code> (Default: <code>0</code>)
     * - <code>int32 B</code> (Default: <code>1</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>int32 ReturnValue</code>
     */
    class Add_IntInt : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InParam_A = nullptr;
        UEdGraphPin* InParam_B = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Add_IntInt(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const FString& ArgInputParamDefault_A = "0",
            const FString& ArgInputParamDefault_B = "1")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UKismetMathLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UKismetMathLibrary, Add_IntInt));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_A = Node->FindPinChecked(TEXT("A"));
            InParam_B = Node->FindPinChecked(TEXT("B"));

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();

            // Set pin defaults
            InParam_A->DefaultValue = ArgInputParamDefault_A;
            InParam_B->DefaultValue = ArgInputParamDefault_B;
        }

        UEdGraphPin* GetInParam_A() const { return InParam_A; }
        UEdGraphPin* GetInParam_B() const { return InParam_B; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates a "Make Literal uint256 from String" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>FString InValue</code> (Default: <code>"0"</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Then
     * 
     * <b>Output Param Pins</b><br>
     * - <code>FTSBC_uint256 OutValue</code>
     */
    class MakeLiteralUint256FromString : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InParam_InValue = nullptr;

        UEdGraphPin* OutParam_OutValue = nullptr;

    public:
        MakeLiteralUint256FromString(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const FString& ArgInParamDefault_InValue = "0")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UTSBC_uint256FunctionLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UTSBC_uint256FunctionLibrary, MakeLiteralUint256FromString));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_InValue = Node->FindPinChecked(TEXT("InValue"));

            // Output param pins
            OutParam_OutValue = Node->FindPinChecked(TEXT("OutValue"));

            // Set pin defaults
            InParam_InValue->DefaultValue = ArgInParamDefault_InValue;
        }

        UEdGraphPin* GetInParam_InValue() const { return InParam_InValue; }

        UEdGraphPin* GetOutParam_OutValue() const { return OutParam_OutValue; }
    };

    /**
     * Creates a "Make Array" node.
     * 
     * This node uses a wildcard data type <code>T</code>.
     * As soon as at least one wildcard pin is connected it will apply the
     * connected pin type to all other wildcard pins of this node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>T [0]</code> (Array Element <code>0</code>)
     * - ...
     * - <code>T [n]</code> (Array Element <code>n</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>TArray<T> Array</code>
     */
    class MakeArray : public BaseK2NodeWrapper<UK2Node_MakeArray>
    {
    private:
        // NOTE: The pointer for InParam_ArrayElement is fetched dynamically and not cached.

        UEdGraphPin* OutParam_Array = nullptr;

    public:
        MakeArray(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Output param pins
            OutParam_Array = Node->GetOutputPin();
        }

        UEdGraphPin* GetInParam_ArrayElement(const int32 ElementIndex) const
        {
            const FString PinName = FString::Printf(TEXT("[%d]"), ElementIndex);
            return Node->FindPinChecked(PinName);
        }

        UEdGraphPin* GetOutParam_Array() const { return OutParam_Array; }
    };

    /**
     * Creates a "Get" (Array Item) node.
     * 
     * This node uses a wildcard data type <code>T</code>.
     * As soon as at least one wildcard pin is connected it will apply the
     * connected pin type to all other wildcard pins of this node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>TArray<T> Array</code>
     * - <code>int32 Index</code> (Default: <code>"0"</code>)
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>T ReturnValue</code>
     */
    class GetArrayItem : public BaseK2NodeWrapper<UK2Node_GetArrayItem>
    {
    private:
        UEdGraphPin* InParam_Array = nullptr;
        UEdGraphPin* InParam_Index = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        GetArrayItem(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode,
            const FString& ArgInParamDefault_Index = "0")
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_Array = Node->GetTargetArrayPin();
            InParam_Index = Node->GetIndexPin();

            // Output param pins
            OutParam_ReturnValue = Node->GetResultPin();

            // Set pin defaults
            InParam_Index->DefaultValue = ArgInParamDefault_Index;
        }

        UEdGraphPin* GetInParam_Array() const { return InParam_Array; }
        UEdGraphPin* GetInParam_Index() const { return InParam_Index; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates a "Add" (Array Item) node.
     * 
     * This node uses a wildcard data type <code>T</code>.
     * As soon as at least one wildcard pin is connected it will apply the
     * connected pin type to all other wildcard pins of this node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>TArray<T> Array</code>
     * - <code>T Item</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Then
     * 
     * <b>Output Param Pins</b><br>
     * - <code>int32 ReturnValue</code>
     */
    class Array_Add : public BaseK2NodeWrapper<UK2Node_CallArrayFunction>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_Array = nullptr;
        UEdGraphPin* InParam_NewItem = nullptr;

        UEdGraphPin* OutExec_Then = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Array_Add(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UKismetArrayLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UKismetArrayLibrary, Array_Add));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_Array = Node->GetTargetArrayPin();
            InParam_NewItem = Node->FindPinChecked(TEXT("NewItem"));

            // Output exec pins
            OutExec_Then = Node->GetThenPin();

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_Array() const { return InParam_Array; }
        UEdGraphPin* GetInParam_NewItem() const { return InParam_NewItem; }

        UEdGraphPin* GetOutExec_Then() const { return OutExec_Then; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
    * Creates a "Clear" (Array) node.
    * 
    * This node uses a wildcard data type <code>T</code>.
    * As soon as at least one wildcard pin is connected it will apply the
    * connected pin type to all other wildcard pins of this node.
    *
    * <br>
    * <b>Input Exec Pins</b><br>
    * - Exec
    * 
    * <b>Input Param Pins</b><br>
    * - <code>TArray<T> Array</code>
    * 
    * <br>
    * <b>Output Exec Pins</b><br>
    * - Then
    */
    class Array_Clear : public BaseK2NodeWrapper<UK2Node_CallArrayFunction>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_Array = nullptr;

        UEdGraphPin* OutExec_Then = nullptr;

    public:
        Array_Clear(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UKismetArrayLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UKismetArrayLibrary, Array_Clear));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_Array = Node->GetTargetArrayPin();

            // Output exec pins
            OutExec_Then = Node->GetThenPin();
        }

        UEdGraphPin* GetInExec() const { return InExec; }

        UEdGraphPin* GetInParam_Array() const { return InParam_Array; }

        UEdGraphPin* GetOutExec_Then() const { return OutExec_Then; }

    };

    /**
     * Creates an (Array) "Length" node.
     * 
     * This node uses a wildcard data type <code>T</code>.
     * As soon as at least one wildcard pin is connected it will apply the
     * connected pin type to all other wildcard pins of this node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>TArray<T> Array</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>int32 ReturnValue</code>
     */
    class Array_Length : public BaseK2NodeWrapper<UK2Node_CallArrayFunction>
    {
    private:
        UEdGraphPin* InParam_Array = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Array_Length(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UKismetArrayLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UKismetArrayLibrary, Array_Length));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_Array = Node->GetTargetArrayPin();

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();
        }

        UEdGraphPin* GetInParam_Array() const { return InParam_Array; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates a "For Each Element" node.
     * 
     * NOTE: This is a C++ native implementation of UE's "For Each Loop" Blueprint Macro.
     * 
     * This node uses a wildcard data type <code>T</code>.
     * As soon as at least one wildcard pin is connected it will apply the
     * connected pin type to all other wildcard pins of this node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * - Exec
     * 
     * <b>Input Param Pins</b><br>
     * - <code>TArray<T> Array</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * - Loop Body
     * - Completed
     * 
     * <b>Output Param Pins</b><br>
     * - <code>T ArrayElement</code>
     * - <code>int32 ArrayIndex</code>
     */
    class ForEachElement : public BaseK2NodeWrapper<UTSBC_K2Node_ForEachElement>
    {
    private:
        UEdGraphPin* InExec = nullptr;

        UEdGraphPin* InParam_Array = nullptr;

        UEdGraphPin* OutExec_LoopBody = nullptr;
        UEdGraphPin* OutExec_Completed = nullptr;

        UEdGraphPin* OutParam_ArrayElement = nullptr;
        UEdGraphPin* OutParam_ArrayIndex = nullptr;

    public:
        ForEachElement(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input exec pins
            InExec = Node->GetExecPin();

            // Input param pins
            InParam_Array = Node->GetArrayPin();

            // Output exec pins
            OutExec_LoopBody = Node->GetLoopBodyPin();
            OutExec_Completed = Node->GetCompletedPin();

            // Output param pins
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

    /**
     * Creates a (uint256) "To Hex String" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>FTSBC_uint256 Value</code>
     * - <code>bool ZeroPadded</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b>
     * - <code>FString ReturnValue</code>
     */
    class Uint256ToHexString : public BaseK2NodeWrapper<UK2Node_CallFunction>
    {
    private:
        UEdGraphPin* InParam_Value = nullptr;
        UEdGraphPin* InParam_ZeroPadded = nullptr;

        UEdGraphPin* OutParam_ReturnValue = nullptr;

    public:
        Uint256ToHexString(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            const UFunction* Function = UTSBC_uint256FunctionLibrary::StaticClass()->FindFunctionByName(
                GET_FUNCTION_NAME_CHECKED(UTSBC_uint256FunctionLibrary, ToHexString));
            Node->SetFromFunction(Function);
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_Value = Node->FindPinChecked(TEXT("Value"));
            InParam_ZeroPadded = Node->FindPinChecked(TEXT("bZeroPadded"));

            // Output param pins
            OutParam_ReturnValue = Node->GetReturnValuePin();
        }

        UEdGraphPin* GetInParam_Value() const { return InParam_Value; }
        UEdGraphPin* GetInParam_ZeroPadded() const { return InParam_ZeroPadded; }

        UEdGraphPin* GetOutParam_ReturnValue() const { return OutParam_ReturnValue; }
    };

    /**
     * Creates a "Make TSBC_SolidityValueList" (struct) node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>TArray<FString> Values</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b>
     * - <code>FTSBC_SolidityValueList Struct</code>
     */
    class MakeStruct_SolidityValueList : public BaseK2NodeWrapper<UK2Node_MakeStruct>
    {
    private:
        UEdGraphPin* InParam_Values = nullptr;

        UEdGraphPin* OutParam_Struct = nullptr;

    public:
        MakeStruct_SolidityValueList(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            static UScriptStruct* SolidityValueListStruct =
                TSBC_TypeUtils::FindStructSafe<FTSBC_SolidityValueList>(nullptr);

            Node->StructType = SolidityValueListStruct;
            Node->bMadeAfterOverridePinRemoval = true;
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_Values = Node->FindPinChecked(TEXT("Values"));

            // Output param pins
            OutParam_Struct = Node->FindPinChecked(Node->StructType->GetFName());
        }

        UEdGraphPin* GetInParam_Values() const { return InParam_Values; }

        UEdGraphPin* GetOutParam_Struct() const { return OutParam_Struct; }
    };

    /**
     * Creates a "Break TSBC_SolidityValueList" (struct) node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * - <code>FTSBC_SolidityValueList InputValue</code>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b>
     * - <code>TArray<FString> Values</code> 
     */
    class BreakStruct_SolidityValueList : public BaseK2NodeWrapper<UK2Node_BreakStruct>
    {
    private:
        UEdGraphPin* InParam_Struct = nullptr;
        UEdGraphPin* OutParam_Values = nullptr;

    public:
        BreakStruct_SolidityValueList(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            static UScriptStruct* SolidityValueListStruct =
                TSBC_TypeUtils::FindStructSafe<FTSBC_SolidityValueList>(nullptr);

            Node->StructType = SolidityValueListStruct;
            Node->bMadeAfterOverridePinRemoval = true;
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Input param pins
            InParam_Struct = Node->FindPinChecked(Node->StructType->GetFName());

            // Output param pins
            OutParam_Values = Node->FindPinChecked(TEXT("Values"));
        }

        UEdGraphPin* GetInParam_Struct() const { return InParam_Struct; }
        UEdGraphPin* GetOutParam_Values() const { return OutParam_Values; }
    };

    /**
     * Creates a "Local Integer" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * <i>NONE</i>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>int32 Variable</code>
     */
    class TemporaryVariable_Int : public BaseK2NodeWrapper<UK2Node_TemporaryVariable>
    {
    private:
        UEdGraphPin* OutParam_Variable = nullptr;

    public:
        TemporaryVariable_Int(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->VariableType.PinCategory = UEdGraphSchema_K2::PC_Int;
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Output param pins
            OutParam_Variable = Node->GetVariablePin();
        }

        UEdGraphPin* GetOutParam_Variable() const { return OutParam_Variable; }
    };

    /**
     * Creates a "Local Array of Booleans" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * <i>NONE</i>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>TArray<bool> Variable</code>
     */
    class TemporaryVariable_BoolArray : public BaseK2NodeWrapper<UK2Node_TemporaryVariable>
    {
    private:
        UEdGraphPin* OutParam_Variable = nullptr;

    public:
        TemporaryVariable_BoolArray(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->VariableType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
            Node->VariableType.ContainerType = EPinContainerType::Array;
            Node->VariableType.bIsConst = true;
            Node->VariableType.bIsReference = true;
            Node->bIsPersistent = true;
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Output param pins
            OutParam_Variable = Node->GetVariablePin();
        }

        UEdGraphPin* GetOutParam_Variable() const { return OutParam_Variable; }
    };

    /**
     * Creates a "Local Array of Strings" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * <i>NONE</i>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>TArray<FString> Variable</code>
     */
    class TemporaryVariable_StringArray : public BaseK2NodeWrapper<UK2Node_TemporaryVariable>
    {
    private:
        UEdGraphPin* OutParam_Variable = nullptr;

    public:
        TemporaryVariable_StringArray(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            Node->VariableType.PinCategory = UEdGraphSchema_K2::PC_String;
            Node->VariableType.ContainerType = EPinContainerType::Array;
            Node->VariableType.bIsConst = true;
            Node->VariableType.bIsReference = true;
            Node->bIsPersistent = true;
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Output param pins
            OutParam_Variable = Node->GetVariablePin();
        }

        UEdGraphPin* GetOutParam_Variable() const { return OutParam_Variable; }
    };

    /**
     * Creates a "Local Array of uint256" node.
     *
     * <br>
     * <b>Input Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Input Param Pins</b><br>
     * <i>NONE</i>
     * 
     * <br>
     * <b>Output Exec Pins</b><br>
     * <i>NONE</i>
     * 
     * <b>Output Param Pins</b><br>
     * - <code>TArray<FTSBC_uint256> Variable</code>
     */
    class TemporaryVariable_Uint256Array : public BaseK2NodeWrapper<UK2Node_TemporaryVariable>
    {
    private:
        UEdGraphPin* OutParam_Variable = nullptr;

    public:
        TemporaryVariable_Uint256Array(
            FKismetCompilerContext& CompilerContext,
            UEdGraph* SourceGraph,
            UK2Node* SourceNode)
            : BaseK2NodeWrapper(CompilerContext, SourceGraph, SourceNode)
        {
            static UScriptStruct* Uint256Struct = TSBC_TypeUtils::FindStructSafe<FTSBC_uint256>(nullptr);

            Node->VariableType.PinCategory = UEdGraphSchema_K2::PC_Struct;
            Node->VariableType.PinSubCategoryObject = Uint256Struct;
            Node->VariableType.ContainerType = EPinContainerType::Array;
            Node->VariableType.bIsConst = true;
            Node->VariableType.bIsReference = true;
            Node->bIsPersistent = true;
            Node->AllocateDefaultPins();
            CompilerContext.MessageLog.NotifyIntermediateObjectCreation(Node, SourceNode);

            // Output param pins
            OutParam_Variable = Node->GetVariablePin();
        }

        UEdGraphPin* GetOutParam_Variable() const { return OutParam_Variable; }
    };

}