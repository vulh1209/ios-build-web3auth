// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/TSBC_Layer2FunctionLibrary.h"

void UTSBC_Layer2FunctionLibrary::GetLayer2Config(
    const ETSBC_Layer2Network Layer2Network,
    FTSBC_Layer2Config& Layer2Config)
{
    switch(Layer2Network)
    {
    case ETSBC_Layer2Network::LoopringMainNet:
        {
            Layer2Config.RpcUrl = "https://api3.loopring.io";
            Layer2Config.NetworkName = "Loopring (MainNet)";
        }
        break;
    case ETSBC_Layer2Network::LoopringTestNet2:
        {
            Layer2Config.RpcUrl = "https://uat2.loopring.io";
            Layer2Config.NetworkName = "Loopring (TestNet2)";
        }
        break;
    case ETSBC_Layer2Network::LoopringTestNet3:
        {
            Layer2Config.RpcUrl = "http://uat3.loopring.io";
            Layer2Config.NetworkName = "Loopring (TestNet3)";
        }
        break;
    case ETSBC_Layer2Network::ImmutablexMainNet:
        {
            Layer2Config.RpcUrl = "https://api.x.immutable.com";
            Layer2Config.NetworkName = "ImmutableX (MainNet)";
        }
        break;
    case ETSBC_Layer2Network::ImmutablexSandboxTestNet:
        {
            Layer2Config.RpcUrl = "https://api.sandbox.x.immutable.com";
            Layer2Config.NetworkName = "ImmutableX Sandbox (TestNet)";
        }
        break;
    default:
        checkNoEntry();
        break;
    }
}