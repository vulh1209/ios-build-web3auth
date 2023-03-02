// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#include "TSBC_EditorTypes.generated.h"

/**
 * Used to display custom combo box widget that holds the contract's ABI functions.
 */
USTRUCT(Blueprintable)
struct FTSBC_ContractAbiFunctionNamePin
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FName FunctionName = "";

public:
    static bool IsValueEmptyOrDefault(const UEdGraphPin& FunctionNamePin)
    {
        return FunctionNamePin.DefaultValue == "" || FunctionNamePin.DefaultValue == "(FunctionName=\"\")";
    }

    /**
     * Serializes this struct into a format UE can store.
     * 
     * NOTE: Currently this is only one field "FunctionName'.
     */
    static FString Serialize(const FString& FunctionName)
    {
        FString Content;
        if(!FunctionName.IsEmpty())
        {
            Content = FString::Printf(TEXT("FunctionName=\"%s\""), *FunctionName);
        }

        return FString::Printf(TEXT("(%s)"), *Content);
    }

    /**
     * Deserializes this struct's data from an input string.
     * 
     * NOTE: Currently this is only one field "FunctionName'.
     */
    static FString Deserialize(FString SerializedStruct)
    {
        if(!SerializedStruct.StartsWith(TEXT("(")) || !SerializedStruct.EndsWith(TEXT(")")))
        {
            return "";
        }

        SerializedStruct.MidInline(1, SerializedStruct.Len() - 2);
        SerializedStruct.Split("=", nullptr, &SerializedStruct);

        if(!SerializedStruct.StartsWith(TEXT("\"")) || !SerializedStruct.EndsWith(TEXT("\"")))
        {
            return "";
        }

        return SerializedStruct.Mid(1, SerializedStruct.Len() - 2);
    }
};