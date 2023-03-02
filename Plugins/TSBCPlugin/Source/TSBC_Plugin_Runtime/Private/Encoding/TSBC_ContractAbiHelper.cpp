// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_ContractAbiHelper.h"

bool CTSBC_ContractAbiHelper::IsFunctionAvailable(
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    FTSBC_ContractAbiFunction& FunctionRef)
{
    TArray<FTSBC_ContractAbiFunction> FunctionsRef = ContractAbi.ContractAbiFunctions;

    for(const FTSBC_ContractAbiFunction& Function : FunctionsRef)
    {
        if(Function.Name == FunctionName)
        {
            FunctionRef = Function;
            return true;
        }
    }
    return false;
}


void CTSBC_ContractAbiHelper::GetSolidityDataTypes(
    const TArray<FTSBC_SolidityFunctionSignature>& FunctionSignatures,
    TArray<ETSBC_SolidityDataType>& SolidityDataTypes,
    bool& bContainsTuple)
{
    bContainsTuple = false;
    for(int32 i = 0; i < FunctionSignatures.Num(); i++)
    {
        const FString ArgumentType = FunctionSignatures[i].Variable.Type;

        if(ArgumentType.StartsWith(AbiType_Address))
        {
            if(ArgumentType.EndsWith("]"))
            {
                SolidityDataTypes.Add(ETSBC_SolidityDataType::AddressArray);
                continue;
            }

            SolidityDataTypes.Add(ETSBC_SolidityDataType::Address);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Uint))
        {
            if(ArgumentType.EndsWith("]"))
            {
                SolidityDataTypes.Add(ETSBC_SolidityDataType::UintArray);
                continue;
            }

            SolidityDataTypes.Add(ETSBC_SolidityDataType::Uint);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Bool))
        {
            if(ArgumentType.EndsWith("]"))
            {
                SolidityDataTypes.Add(ETSBC_SolidityDataType::BoolArray);
                continue;
            }
            SolidityDataTypes.Add(ETSBC_SolidityDataType::Bool);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_String))
        {
            if(ArgumentType.EndsWith("]"))
            {
                SolidityDataTypes.Add(ETSBC_SolidityDataType::StringArray);
                continue;
            }

            SolidityDataTypes.Add(ETSBC_SolidityDataType::String);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Bytes))
        {
            if(ArgumentType.EndsWith("]"))
            {
                SolidityDataTypes.Add(ETSBC_SolidityDataType::BytesArray);
                continue;
            }

            SolidityDataTypes.Add(ETSBC_SolidityDataType::Bytes);
            continue;
        }

        if(ArgumentType.StartsWith(AbiType_Tuple))
        {
            bContainsTuple = true;
            // If an argument is tuple array, skip since we can't handle this type.
            if(ArgumentType.EndsWith("]"))
            {
                continue;
            }
            SolidityDataTypes.Add(ETSBC_SolidityDataType::Tuple);
        }
    }
}

void CTSBC_ContractAbiHelper::GetSolidityDataTypesFromTuple(
    TArray<FTSBC_SolidityVariable> InSolidityTupleVariables,
    bool& bOutContainsDynamic,
    TArray<ETSBC_SolidityDataType>& OutSolidityDataTypes)
{
    bOutContainsDynamic = false;
    for(int32 i = 0; i < InSolidityTupleVariables.Num(); i++)
    {
        FString OutputType = InSolidityTupleVariables[i].Type;

        if(OutputType.StartsWith(AbiType_Address))
        {
            if(OutputType.EndsWith("]"))
            {
                bOutContainsDynamic = true;
                OutSolidityDataTypes.Add(ETSBC_SolidityDataType::AddressArray);
                continue;
            }

            OutSolidityDataTypes.Add(ETSBC_SolidityDataType::Address);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Uint))
        {
            if(OutputType.EndsWith("]"))
            {
                bOutContainsDynamic = true;
                OutSolidityDataTypes.Add(ETSBC_SolidityDataType::UintArray);
                continue;
            }

            OutSolidityDataTypes.Add(ETSBC_SolidityDataType::Uint);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Bool))
        {
            if(OutputType.EndsWith("]"))
            {
                bOutContainsDynamic = true;
                OutSolidityDataTypes.Add(ETSBC_SolidityDataType::BoolArray);
                continue;
            }

            OutSolidityDataTypes.Add(ETSBC_SolidityDataType::Bool);
            continue;
        }

        if(OutputType.StartsWith(AbiType_String))
        {
            bOutContainsDynamic = true;
            if(OutputType.EndsWith("]"))
            {
                OutSolidityDataTypes.Add(ETSBC_SolidityDataType::StringArray);
                continue;
            }

            OutSolidityDataTypes.Add(ETSBC_SolidityDataType::String);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Bytes))
        {
            bOutContainsDynamic = true;
            if(OutputType.EndsWith("]"))
            {
                OutSolidityDataTypes.Add(ETSBC_SolidityDataType::BytesArray);
                continue;
            }

            OutSolidityDataTypes.Add(ETSBC_SolidityDataType::Bytes);
            continue;
        }

        if(OutputType.StartsWith(AbiType_Tuple))
        {
            // If type is tuple in a tuple component we do nothing since we can't handle this type.
        }
    }
}


int32 CTSBC_ContractAbiHelper::GetStaticArrayLength(const FString& Type)
{
    int32 Bracket = 0;
    if(!Type.FindChar(TEXT('['), Bracket))
    {
        return 0;
    }
    FString ArrayLength = Type.Mid(Bracket, Type.Len());

    int32 FirstBracket = 0;
    if(!ArrayLength.FindChar(TEXT('['), FirstBracket))
    {
        return 0;
    }
    ArrayLength.RemoveAt(FirstBracket);

    int32 SecondBracket = 0;
    if(!ArrayLength.FindChar(TEXT(']'), SecondBracket))
    {
        return 0;
    }
    ArrayLength.RemoveAt(SecondBracket);

    const int32 Length = FCString::Atoi(*ArrayLength);
    return Length;
}