// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthGetTransactionReceiptCallback.h"

void UTSBC_EthGetTransactionReceiptCallback::K2_EthTransactionReceiptCallback(
    FTSBC_K2_EthGetTransactionCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Hash)
{
    CTSBC_EthGetTransactionReceipt::FTSBC_EthGetTransactionReceipt_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const bool bReceiptFound,
        const FTSBC_EthTransactionReceipt& TransactionReceipt)
        {
            if(ResponseDelegate.IsBound())
            {
                ResponseDelegate.Execute(bSuccess, Response, bReceiptFound, TransactionReceipt);
            }
        });

    CTSBC_EthGetTransactionReceipt::EthGetTransactionReceipt(
        InternalDelegate,
        URL,
        ID,
        Hash);
}