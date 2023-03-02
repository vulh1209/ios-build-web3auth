// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/ImmutableX/TSBC_ImmutablexGetDetailsOfAsset.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Util/TSBC_HttpUtils.h"

void CTSBC_ImmutablexGetDetailsOfAsset::ImmutablexGetDetailsOfAsset(
    FTSBC_ImmutablexGetDetailsOfAsset_Delegate ResponseDelegate,
    const FString& URL,
    const FString& TokenAddress,
    const FString& TokenId,
    const bool bIncludeFees)
{
#if !UE_BUILD_SHIPPING
    if(URL.TrimStartAndEnd().IsEmpty())
    {
        TSBC_LOG(Error, TEXT("HTTP URL is unset: Verb<%s> Function<%s>"), "GET", "ImmutablexGetDetailsOfAsset");
    }
#endif

    const FString EndpointUrlWithPlaceholders = "/v1/assets/:token_address/:token_id";
    const FString EndpointURL = URL + TSBC_HttpUtils::ReplacePathVariables(
                                    EndpointUrlWithPlaceholders,
                                    {
                                        {"token_address", TokenAddress},
                                        {"token_id", TokenId},
                                    });

    TMap<FString, FString> Params = {};
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "include_fees", bIncludeFees ? "true" : "false");

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetHeader("Accept", "application/json");
    HttpRequest->SetURL(TSBC_HttpUtils::BuildUrlWithQueryParams(EndpointURL, Params));

    HttpRequest->OnProcessRequestComplete().BindLambda(
        [ResponseDelegate](const FHttpRequestPtr, const FHttpResponsePtr HttpResponsePtr, const bool bWasSuccessful)
        {
            FTSBC_HttpResponse HttpResponse;
            if(HttpResponsePtr)
            {
                const bool bStatusCodeOk = HttpResponsePtr->GetResponseCode() >= 200
                                           && HttpResponsePtr->GetResponseCode() <= 299;
                HttpResponse.bSuccess = bWasSuccessful && bStatusCodeOk;
                HttpResponse.StatusCode = HttpResponsePtr->GetResponseCode();
                HttpResponse.Headers = HttpResponsePtr->GetAllHeaders();
                HttpResponse.Body = HttpResponsePtr->GetContentAsString();
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(HttpResponse.bSuccess, HttpResponse);
        });

    HttpRequest->ProcessRequest();
}