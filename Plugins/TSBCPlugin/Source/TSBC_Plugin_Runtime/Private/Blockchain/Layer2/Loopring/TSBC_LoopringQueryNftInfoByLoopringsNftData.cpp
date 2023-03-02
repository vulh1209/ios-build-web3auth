// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/TSBC_LoopringQueryNftInfoByLoopringsNftData.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Util/TSBC_HttpUtils.h"

void CTSBC_LoopringQueryNftInfoByLoopringsNftData::LoopringQueryNftInfoByLoopringsNftData(
    FTSBC_LoopringQueryNftInfoByLoopringsNftData_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ApiKey,
    const FString& NftDatas)
{
#if !UE_BUILD_SHIPPING
    if(URL.TrimStartAndEnd().IsEmpty())
    {
        TSBC_LOG(
            Error,
            TEXT("HTTP URL is unset: Verb<%s> Function<%s>"),
            "GET",
            "LoopringQueryNftInfoByLoopringsNftData");
    }
#endif

    const FString EndpointURL = URL + "/api/v3/nft/info/nfts";

    TMap<FString, FString> Params = {};
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "nftDatas", NftDatas);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetHeader("X-API-KEY", ApiKey);
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