// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "DataAssets//TSBC_ContractAbiDataAsset.h"

#include "Encoding/TSBC_ContractAbiParsing.h"

void UTSBC_ContractAbiDataAsset::ParseJson()
{
    ContractAbi = {}; // Clear previous result

    bool bSuccess;
    FString Error;
    CTSBC_ContractAbiParsing::ParseAbiFromJson(ContractAbiJson, bSuccess, Error, ContractAbi);

    ContractAbiValidityStatus = bSuccess ? "OK - Contract ABI valid" : Error;
}