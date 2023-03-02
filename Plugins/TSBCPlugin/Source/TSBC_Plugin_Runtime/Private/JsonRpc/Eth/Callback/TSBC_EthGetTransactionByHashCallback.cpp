// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthGetTransactionByHashCallback.h"

void UTSBC_EthGetTransactionByHashCallback::K2_EthGetTransactionByHash(
    FTSBC_K2_EthGetTransactionByHashCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Hash)
{
    CTSBC_EthGetTransactionByHash::FTSBC_EthGetTransactionByHash_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const bool bTransactionFound,
        const FTSBC_EthTransactionInfo& TransactionInfo)
        {
            if(ResponseDelegate.IsBound())
            {
                ResponseDelegate.Execute(bSuccess, Response, bTransactionFound, TransactionInfo);
            }
        });

    CTSBC_EthGetTransactionByHash::EthGetTransactionByHash(
        InternalDelegate,
        URL,
        ID,
        Hash);
}