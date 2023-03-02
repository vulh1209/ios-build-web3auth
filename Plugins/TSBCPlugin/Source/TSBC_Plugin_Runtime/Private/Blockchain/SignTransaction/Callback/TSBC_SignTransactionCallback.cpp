// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/SignTransaction/Callback/TSBC_SignTransactionCallback.h"

#include "Blockchain/SignTransaction/TSBC_SignTransaction.h"

void UTSBC_SignTransactionCallback::K2_SignTransactionCallback(
    FTSBC_K2_SignTransactionCallback_Delegate ResponseDelegate,
    const TArray<uint8> PrivateKey,
    const int32 Nonce,
    const FTSBC_uint256 GasPrice,
    const FTSBC_uint256 GasLimit,
    const FString ToAddress,
    const FTSBC_uint256 Value,
    const FString Data,
    const int32 ChainId)
{
    CTSBC_SignTransaction::FTSBC_SignTransaction_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FString& ErrorMessage,
        const FString& SignedTransaction,
        const FString& MessageHash,
        const FString& TransactionHash)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(
                bSuccess,
                ErrorMessage,
                SignedTransaction,
                MessageHash,
                TransactionHash);
        });

    CTSBC_SignTransaction::SignTransactionAsync(
        InternalDelegate,
        PrivateKey,
        Nonce,
        GasPrice,
        GasLimit,
        ToAddress,
        Value,
        Data,
        ChainId);
}