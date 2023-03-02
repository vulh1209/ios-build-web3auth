// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "GenericPlatform/GenericPlatformHttp.h"

namespace TSBC_HttpUtils
{
    static FString BuildUrlWithQueryParams(
        const FString& URL,
        const TMap<FString, FString> Params
    )
    {
        // No parameters given
        if(Params.Num() == 0)
        {
            return URL;
        }

        FString UrlWithQueryParams = URL;

        uint32 ParamIndex = 0;
        for(TTuple<FString, FString> Param : Params)
        {
            const FString Delimiter = ParamIndex == 0 ? "?" : "&";
            const FString Key = Param.Key;
            const FString Value = FGenericPlatformHttp::UrlEncode(Param.Value);
            UrlWithQueryParams += FString::Printf(TEXT("%s%s=%s"), *Delimiter, *Key, *Value);

            ParamIndex++;
        }

        return UrlWithQueryParams;
    }

    static void AddParamIfValueNotEmpty(TMap<FString, FString>& Params, const FString& Key, const FString& Value)
    {
        if(Value.TrimStartAndEnd().IsEmpty())
        {
            return;
        }

        Params.Add(Key, Value);
    }

    static FString ReplacePathVariables(
        const FString& URL,
        const TMap<FString, FString> PathVariablePlaceholderToValueMap)
    {
        if(PathVariablePlaceholderToValueMap.Num() == 0)
        {
            return URL;
        }

        FString UrlWithReplacedPathVariables = URL;
        for(auto Element : PathVariablePlaceholderToValueMap)
        {
            const FString PathVariablePlaceholder = FString::Printf(TEXT(":%s"), *Element.Key);
            const FString PathVariableValue = FGenericPlatformHttp::UrlEncode(Element.Value);
            UrlWithReplacedPathVariables.ReplaceInline(
                *PathVariablePlaceholder,
                *PathVariableValue,
                ESearchCase::CaseSensitive);
        }

        return UrlWithReplacedPathVariables;
    }
}