// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthSendRawTransactionCallback.h"

#include "JsonRpc/Eth/TSBC_EthSendRawTransaction.h"

void UTSBC_EthSendRawTransactionCallback::K2_EthSendRawTransactionCallback(
    FTSBC_K2_EthSendRawTransactionCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString SignedTransactionData)
{
    CTSBC_EthSendRawTransaction::FTSBC_EthSendRawTransaction_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FString& TransactionHash)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response, TransactionHash);
        });

    CTSBC_EthSendRawTransaction::EthSendRawTransaction(
        InternalDelegate,
        URL,
        ID,
        SignedTransactionData);
}