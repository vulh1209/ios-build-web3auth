// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/SignTransaction/Callback/TSBC_SignTransactionLowLevelCallback.h"

#include "Blockchain/SignTransaction/TSBC_SignTransaction.h"
#include "Data/TSBC_Types.h"

void UTSBC_SignTransactionLowLevelCallback::SignTransactionLowLevel(
    FTSBC_K2_SignTransactionLowLevelCallback_Delegate ResponseDelegate,
    const FString PrivateKey,
    const FTSBC_EthTransaction& Transaction,
    const bool bSignDeterministically)
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

    CTSBC_SignTransaction::SignTransactionLowLevelAsync(
        InternalDelegate,
        PrivateKey,
        Transaction,
        bSignDeterministically);
}