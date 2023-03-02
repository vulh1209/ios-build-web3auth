// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/SignTransaction/TSBC_SignTransaction.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Async/Async.h"
// =============================================================================

#include "Crypto/Encryption/TSBC_EcdsaSecp256k1.h"
#include "Crypto/Hash/TSBC_Keccak256.h"
#include "Data/TSBC_Types.h"
#include "Encoding/TSBC_RLP.h"
#include "Module/TSBC_RuntimeLogCategories.h"
#include "Module/TSBC_PluginUserSettings.h"
#include "Util/TSBC_StringUtils.h"

void CTSBC_SignTransaction::SignTransactionAsync(
    FTSBC_SignTransaction_Delegate ResponseDelegate,
    const TArray<uint8> PrivateKey,
    const int32 Nonce,
    const FTSBC_uint256 GasPrice,
    const FTSBC_uint256 GasLimit,
    const FString ToAddress,
    const FTSBC_uint256 Value,
    const FString Data,
    const int32 ChainId)
{
    Async(
        EAsyncExecution::Thread,
        [
            PrivateKey,
            Nonce,
            GasPrice,
            GasLimit,
            ToAddress,
            Value,
            Data,
            ChainId
        ]() -> FFutureRetval
        {
            const FString PrivateKeyAsString = TSBC_StringUtils::BytesToHex(PrivateKey, false);

            const FTSBC_EthTransaction Transaction{
                FString::FromInt(Nonce),
                GasPrice.ToHexString(),
                GasLimit.ToHexString(),
                ToAddress,
                Value.ToHexString(),
                Data,
                FString::FromInt(ChainId),
            };

            FFutureRetval Retval;
            SignTransactionLowLevelSync(
                Retval.bSuccess,
                Retval.ErrorMessage,
                Retval.SignedTransaction,
                Retval.MessageHash,
                Retval.TransactionHash,
                PrivateKeyAsString,
                Transaction);

            return Retval;
        }).Then(
        [ResponseDelegate](auto RetvalPtr)
        {
            const FFutureRetval Retval = RetvalPtr.Get();

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(
                Retval.bSuccess,
                Retval.ErrorMessage,
                Retval.SignedTransaction,
                Retval.MessageHash,
                Retval.TransactionHash);
        });
}

void CTSBC_SignTransaction::SignTransactionSync(
    bool& bSuccess,
    FString& ErrorMessage,
    FString& SignedTransaction,
    FString& MessageHash,
    FString& TransactionHash,
    const TArray<uint8> PrivateKey,
    const int32 Nonce,
    const FTSBC_uint256 GasPrice,
    const FTSBC_uint256 GasLimit,
    const FString ToAddress,
    const FTSBC_uint256 Value,
    const FString Data,
    const int32 ChainId)
{
    const FString PrivateKeyAsString = TSBC_StringUtils::BytesToHex(PrivateKey, false);

    const FTSBC_EthTransaction Transaction{
        FString::FromInt(Nonce),
        GasPrice.ToHexString(),
        GasLimit.ToHexString(),
        ToAddress,
        Value.ToHexString(),
        Data,
        FString::FromInt(ChainId),
    };

    SignTransactionLowLevelSync(
        bSuccess,
        ErrorMessage,
        SignedTransaction,
        MessageHash,
        TransactionHash,
        PrivateKeyAsString,
        Transaction);
}

void CTSBC_SignTransaction::SignTransactionLowLevelAsync(
    FTSBC_SignTransaction_Delegate ResponseDelegate,
    const FString PrivateKey,
    const FTSBC_EthTransaction& Transaction)
{
    Async(
        EAsyncExecution::Thread,
        [
            PrivateKey,
            Transaction
        ]() -> FFutureRetval
        {
            FFutureRetval Retval;
            SignTransactionLowLevelSync(
                Retval.bSuccess,
                Retval.ErrorMessage,
                Retval.SignedTransaction,
                Retval.MessageHash,
                Retval.TransactionHash,
                PrivateKey,
                Transaction);

            return Retval;
        }).Then(
        [ResponseDelegate](auto RetvalPtr)
        {
            const FFutureRetval Retval = RetvalPtr.Get();

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(
                Retval.bSuccess,
                Retval.ErrorMessage,
                Retval.SignedTransaction,
                Retval.MessageHash,
                Retval.TransactionHash);
        });
}

void CTSBC_SignTransaction::SignTransactionLowLevelSync(
    bool& bSuccess,
    FString& ErrorMessage,
    FString& SignedTransaction,
    FString& MessageHash,
    FString& TransactionHash,
    const FString PrivateKey,
    const FTSBC_EthTransaction& Transaction)
{
    bool bDebugLoggingSignedTransactionsEnabled = false;
#if UE_EDITOR
    if(const auto* Settings = UTSBC_PluginUserSettings::Get())
    {
        bDebugLoggingSignedTransactionsEnabled = Settings->bDebugLoggingSignedTransactionsEnabled;
    }
#endif

    bSuccess = false;
    ErrorMessage = "";

    // Validate private key to be used for signing
    const TArray<uint8> PrivateKeyAsBytes = TSBC_StringUtils::HexToBytes(PrivateKey);
    if(PrivateKeyAsBytes.Num() != 32)
    {
        ErrorMessage = "Private Key must be 32 bytes long";
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    // NOTE: Uncomment this code block if you want to also debug log the private key used when signing transactions.
    // NOTE: However, be aware that this MAY LEAD TO ACCIDENTAL DISCLOSURE OF YOUR PRIVATE KEY if not handled carefully!
    // #ifdef UE_EDITOR
    //     TSBC_LOG_COND(
    //         bDebugLoggingSignedTransactionsEnabled,
    //         Warning,
    //         TEXT("Private Key used for signing Hex: %s"),
    //         *TSBC_StringUtils::BytesToHex(PrivateKeyAsBytes));
    // #endif

    // Normalize transaction data (all data in hex notation)
    FTSBC_EthTransaction TransactionNormalized;
    if(!NormalizeTransactionData(Transaction, TransactionNormalized, ErrorMessage))
    {
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    // RLP-encode transaction parameters.
    const TArray<FString> TransactionParams = BuildMessageTransactionParams(TransactionNormalized);
    const TArray<uint8> RlpEncodedTransactionParamsAsBytes = UTSBC_RLP::Encode(TransactionParams);
    const FString MessageAsHex = TSBC_StringUtils::BytesToHex(RlpEncodedTransactionParamsAsBytes);
    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT("Message = RLP(Message Transaction Params): %s"),
        *MessageAsHex);

    // Hash message using Keccak-256
    CTSBC_Keccak256 Hasher = CTSBC_Keccak256();
    const TArray<uint8> MessageAsBytes = TSBC_StringUtils::HexToBytes(MessageAsHex);
    MessageHash = Hasher.KeccakFromBytes(MessageAsBytes);
    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT("Message Hash = KEC(Message): %s"),
        *MessageHash);

    // Sign Message Hash
    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT("Signature = SECP256k1.Sign(Private Key, Message Hash) [Non-Deterministic]"));
    const TArray<uint8> HashAsBytes = TSBC_StringUtils::HexToBytes(MessageHash);
    TArray<uint8> Signature;
    const bool bSignatureCalculated = CTSBC_EcdsaSecp256k1::Secp256k1_CreateSignature(
        PrivateKeyAsBytes,
        HashAsBytes,
        Signature);
    if(!bSignatureCalculated)
    {
        ErrorMessage = "Failed to calculate signature";
        TSBC_LOG(Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    // RLP-encode signed transaction parameters. This can be sent as "params" using the "eth_sendRawTransaction" method.
    TArray<FString> SignedTransactionParams;
    if(!BuildSignedTransactionParams(
        TransactionNormalized,
        Signature,
        SignedTransactionParams,
        ErrorMessage))
    {
        return;
    }

    const TArray<uint8> SignedTransactionAsBytes = UTSBC_RLP::Encode(SignedTransactionParams);
    SignedTransaction = TSBC_StringUtils::BytesToHex(SignedTransactionAsBytes);
    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT("Signed Transaction = RLP(Signed Transaction Params): %s\n\n"),
        *SignedTransaction);

    TransactionHash = Hasher.KeccakFromBytes(SignedTransactionAsBytes);
    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT("\n\nTransaction Hash = KEC(Signed Transaction): %s\n\n"),
        *TransactionHash);

    bSuccess = true;
}

bool CTSBC_SignTransaction::NormalizeTransactionData(
    const FTSBC_EthTransaction& InTransaction,
    FTSBC_EthTransaction& OutTransactionNormalized,
    FString& OutErrorMessage)
{
    FString NonceAsString = InTransaction.Nonce.TrimStartAndEnd();
    NonceAsString = NonceAsString.IsEmpty() || NonceAsString == "0"
                    ? "0x"
                    : InTransaction.Nonce;
    FTSBC_uint256 Nonce;
    if(!Nonce.ParseFromString(NonceAsString))
    {
        OutErrorMessage = "Could not parse Nonce as uint256";
        return false;
    }

    FTSBC_uint256 GasPrice;
    if(!GasPrice.ParseFromString(InTransaction.GasPrice))
    {
        OutErrorMessage = "Could not parse GasPrice as uint256";
        return false;
    }

    FTSBC_uint256 GasLimit;
    if(!GasLimit.ParseFromString(InTransaction.GasLimit.IsEmpty() ? InTransaction.GasPrice : InTransaction.GasLimit))
    {
        OutErrorMessage = "Could not parse GasLimit as uint256";
        return false;
    }

    FString ToAddress;
    if(!InTransaction.ToAddress.IsEmpty())
    {
        ToAddress = InTransaction.ToAddress.StartsWith("0x") ? InTransaction.ToAddress : "0x" + InTransaction.ToAddress;
    }

    FString Data;
    if(!InTransaction.Data.IsEmpty())
    {
        Data = InTransaction.Data.StartsWith("0x") ? InTransaction.Data : "0x" + InTransaction.Data;
    }

    FTSBC_uint256 Value;
    if(!Value.ParseFromString(InTransaction.Value))
    {
        OutErrorMessage = "Could not parse Value as uint256";
        return false;
    }
    FTSBC_uint256 ChainId;
    if(!ChainId.ParseFromString(InTransaction.ChainId))
    {
        OutErrorMessage = "Could not parse ChainId as uint256";
        return false;
    }

    OutTransactionNormalized = FTSBC_EthTransaction{
        Nonce == 0 ? "" : Nonce.ToHexString(),
        GasPrice.ToHexString(),
        GasLimit.ToHexString(),
        ToAddress,
        Value == 0 ? "" : Value.ToHexString(),
        Data,
        ChainId.ToHexString(),
    };

    return true;
}

TArray<FString> CTSBC_SignTransaction::BuildMessageTransactionParams(const FTSBC_EthTransaction& InTransaction)
{
    const TArray<FString> TransactionParams = {
        InTransaction.Nonce,
        InTransaction.GasPrice,
        InTransaction.GasLimit,
        InTransaction.ToAddress,
        InTransaction.Value,
        InTransaction.Data,
        InTransaction.ChainId,
        "",
        "",
    };

    bool bDebugLoggingSignedTransactionsEnabled = false;
#if UE_EDITOR
    if(const auto* Settings = UTSBC_PluginUserSettings::Get())
    {
        bDebugLoggingSignedTransactionsEnabled = Settings->bDebugLoggingSignedTransactionsEnabled;
    }
#endif

    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT(
            "Message Transaction Params: Nonce<%s> GasPrice<%s> GasLimit<%s> ToAddress<%s> Value<%s> Data<%s> ChainId<%s>"
        ),
        *InTransaction.Nonce,
        *InTransaction.GasPrice,
        *InTransaction.GasLimit,
        *InTransaction.ToAddress,
        *InTransaction.Value,
        *InTransaction.Data,
        *InTransaction.ChainId);

    return TransactionParams;
}

bool CTSBC_SignTransaction::BuildSignedTransactionParams(
    const FTSBC_EthTransaction& InTransaction,
    const TArray<uint8> Signature,
    TArray<FString>& OutTransactionParams,
    FString& OutErrorMessage)
{
    const int32 ChainId = TSBC_StringUtils::HexStringToInt32(InTransaction.ChainId);
    if(ChainId == -1)
    {
        OutErrorMessage = "Invalid Chain ID in transaction parameters.";
        return false;
    }

    FTSBC_uint256 V;
    if(!V.ParseFromString(CTSBC_EcdsaSecp256k1::GetFromSignatureValueV(Signature, ChainId)))
    {
        OutErrorMessage = "Could not parse V as uint256";
        return false;
    }

    FTSBC_uint256 R;
    if(!R.ParseFromString(CTSBC_EcdsaSecp256k1::GetFromSignatureValueR(Signature)))
    {
        OutErrorMessage = "Could not parse R as uint256";
        return false;
    }

    FTSBC_uint256 S;
    if(!S.ParseFromString(CTSBC_EcdsaSecp256k1::GetFromSignatureValueS(Signature)))
    {
        OutErrorMessage = "Could not parse S as uint256";
        return false;
    }

    OutTransactionParams = TArray<FString>{
        InTransaction.Nonce,
        InTransaction.GasPrice,
        InTransaction.GasLimit,
        InTransaction.ToAddress,
        InTransaction.Value,
        InTransaction.Data,
        V.ToHexString(),
        R.ToHexString(),
        S.ToHexString(),
    };

    bool bDebugLoggingSignedTransactionsEnabled = false;
#if UE_EDITOR
    if(const auto* Settings = UTSBC_PluginUserSettings::Get())
    {
        bDebugLoggingSignedTransactionsEnabled = Settings->bDebugLoggingSignedTransactionsEnabled;
    }
#endif

    TSBC_LOG_COND(
        bDebugLoggingSignedTransactionsEnabled,
        Warning,
        TEXT(
            "Signed Transaction Params: Nonce<%s> GasPrice<%s> GasLimit<%s> ToAddress<%s> Value<%s> Data<%s> Sig.V<%s> Sig.R<%s> Sig.S<%s>"
        ),
        *OutTransactionParams[0],
        *OutTransactionParams[1],
        *OutTransactionParams[2],
        *OutTransactionParams[3],
        *OutTransactionParams[4],
        *OutTransactionParams[5],
        *OutTransactionParams[6],
        *OutTransactionParams[7],
        *OutTransactionParams[8]);

    return true;
}