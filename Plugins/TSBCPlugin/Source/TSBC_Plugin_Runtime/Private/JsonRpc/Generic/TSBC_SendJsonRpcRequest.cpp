// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

void CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
    FTSBC_JsonRpcResponse_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Method,
    const FString& Params)
{
    const FString Body = FString::Printf(
        TEXT("{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":[%s],\"id\":\"%s\"}"),
        *Method,
        *Params,
        *ID);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetURL(URL);
    HttpRequest->SetContentAsString(Body);

    HttpRequest->OnProcessRequestComplete().BindLambda(
        [ResponseDelegate](const FHttpRequestPtr, const FHttpResponsePtr HttpResponse, const bool bWasSuccessful)
        {
            FTSBC_JsonRpcResponse JsonRpcResponse;
            if(HttpResponse)
            {
                const bool bStatusCodeOk = HttpResponse->GetResponseCode() >= 200
                                           && HttpResponse->GetResponseCode() <= 299;
                JsonRpcResponse.bSuccess = bWasSuccessful && bStatusCodeOk;
                JsonRpcResponse.StatusCode = HttpResponse->GetResponseCode();
                JsonRpcResponse.Headers = HttpResponse->GetAllHeaders();
                JsonRpcResponse.Body = HttpResponse->GetContentAsString();
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(JsonRpcResponse);
        });

    HttpRequest->ProcessRequest();
}