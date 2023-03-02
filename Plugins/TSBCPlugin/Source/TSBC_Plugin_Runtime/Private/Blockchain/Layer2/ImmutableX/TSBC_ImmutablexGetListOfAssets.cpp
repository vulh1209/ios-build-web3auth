// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/ImmutableX/TSBC_ImmutablexGetListOfAssets.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Util/TSBC_HttpUtils.h"

void CTSBC_ImmutablexGetListOfAssets::ImmutablexGetListOfAssets(
    FTSBC_ImmutablexGetListOfAssets_Delegate ResponseDelegate,
    const FString& URL,
    const FString& PageSize,
    const FString& Cursor,
    const FString& OrderBy,
    const FString& Direction,
    const FString& User,
    const FString& Status,
    const FString& Name,
    const FString& Metadata,
    const FString& SellOrders,
    const FString& BuyOrders,
    const bool bIncludeFees,
    const FString& Collection,
    const FString& UpdatedMinTimestamp,
    const FString& UpdatedMaxTimestamp,
    const FString& AuxillaryFeePercentages,
    const FString& AuxillaryFeeRecipients)
{
#if !UE_BUILD_SHIPPING
    if(URL.TrimStartAndEnd().IsEmpty())
    {
        TSBC_LOG(Error, TEXT("HTTP URL is unset: Verb<%s> Function<%s>"), "GET", "ImmutablexGetListOfAssets");
    }
#endif

    const FString EndpointURL = URL + "/v1/assets";

    TMap<FString, FString> Params = {};
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "page_size", PageSize);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "cursor", Cursor);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "order_by", OrderBy);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "direction", Direction);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "user", User);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "status", Status);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "name", Name);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "metadata", Metadata);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "sell_orders", SellOrders);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "buy_orders", BuyOrders);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "include_fees", bIncludeFees ? "true" : "false");
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "collection", Collection);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "updated_min_timestamp", UpdatedMinTimestamp);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "updated_max_timestamp", UpdatedMaxTimestamp);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "auxillary_fee_percentages", AuxillaryFeePercentages);
    TSBC_HttpUtils::AddParamIfValueNotEmpty(Params, "auxillary_fee_recipients", AuxillaryFeeRecipients);

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