// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

/**
 * Signs a transaction either asynchronously or synchronously.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_SignTransaction
{
private:
    /**
     * Internal return value for lambdas.
     */
    struct FFutureRetval
    {
        /**
         * True if the operation is successful.
         */
        bool bSuccess;

        /**
         * Contains an error message in case the operation fails. Otherwise, it will be empty.
         */
        FString ErrorMessage;

        /**
         * The encoded and signed transaction data that can be passed to eth_sendRawTransaction.
         */
        FString SignedTransaction;

        /**
         * The message hash of the signed transaction.
         */
        FString MessageHash;

        /**
         * The hash of the signed transaction.
         */
        FString TransactionHash;
    };

public:
    DECLARE_DELEGATE_FiveParams(
        FTSBC_SignTransaction_Delegate,
        bool /* bSuccess */,
        FString /* ErrorMessage */,
        FString /* SignedTransaction */,
        FString /* MessageHash */,
        FString /* TransactionHash */);

    /**
     * Signs a transaction.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a signing the transaction fails.
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Nonce The nonce.
     * @param GasPrice The gas price used for each gas unit.
     * @param GasLimit Gas limit provided for the transaction to execute.
     * @param ToAddress The address the transaction is directed to.
     * @param Value The amount of Ether (in Wei) to send with this transaction.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param ChainId The Blockchain ID.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    static void SignTransactionAsync(
        FTSBC_SignTransaction_Delegate ResponseDelegate,
        const TArray<uint8> PrivateKey,
        const int32 Nonce,
        const FTSBC_uint256 GasPrice,
        const FTSBC_uint256 GasLimit,
        const FString ToAddress,
        const FTSBC_uint256 Value,
        const FString Data,
        const int32 ChainId,
        const bool bSignDeterministically = false);

    /**
     * Signs a transaction.
     *
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param SignedTransaction The encoded and signed transaction data that can be passed to eth_sendRawTransaction.
     * @param MessageHash The message hash of the signed transaction.
     * @param TransactionHash The hash of the signed transaction.
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Nonce The nonce.
     * @param GasPrice The gas price used for each gas unit.
     * @param GasLimit Gas limit provided for the transaction to execute.
     * @param ToAddress The address the transaction is directed to.
     * @param Value The amount of Ether (in Wei) to send with this transaction.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param ChainId The Blockchain ID.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    static void SignTransactionSync(
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
        const int32 ChainId,
        const bool bSignDeterministically = false);

    /**
     * Signs a transaction.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a signing the transaction fails.
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Transaction The transaction parameters.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    static void SignTransactionLowLevelAsync(
        FTSBC_SignTransaction_Delegate ResponseDelegate,
        const FString PrivateKey,
        const FTSBC_EthTransaction& Transaction,
        const bool bSignDeterministically = false);

    /**
     * Signs a transaction.
     *
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param SignedTransaction The encoded and signed transaction data that can be passed to eth_sendRawTransaction.
     * @param MessageHash The message hash of the signed transaction.
     * @param TransactionHash The hash of the signed transaction.
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Transaction The transaction parameters.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    static void SignTransactionLowLevelSync(
        bool& bSuccess,
        FString& ErrorMessage,
        FString& SignedTransaction,
        FString& MessageHash,
        FString& TransactionHash,
        const FString PrivateKey,
        const FTSBC_EthTransaction& Transaction,
        const bool bSignDeterministically = false);

private:
    /**
     * Normalizes transaction parameters by transforming or filling out missing information.
     *
     * @param InTransaction The transaction parameters.
     * @param OutTransactionNormalized The normalized transaction parameters.
     * @param OutErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @returns True if the operation is successful.
     */
    static bool NormalizeTransactionData(
        const FTSBC_EthTransaction& InTransaction,
        FTSBC_EthTransaction& OutTransactionNormalized,
        FString& OutErrorMessage);

    /**
     * Builds an array of strings from the transaction parameters to be used for RLP-encoding a message.
     *
     * @param InTransaction The transaction parameters.
     * @returns An array of strings from the transaction parameters.
     */
    static TArray<FString> BuildMessageTransactionParams(const FTSBC_EthTransaction& InTransaction);

    /**
     * Builds an array of strings from the transaction parameters to be used for signing the transaction.
     *
     * @param InTransaction The transaction parameters.
     * @param Signature The signature.
     * @param OutTransactionParams The transaction parameters as string array.
     * @param OutErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @returns An array of strings from the transaction parameters.
     */
    static bool BuildSignedTransactionParams(
        const FTSBC_EthTransaction& InTransaction,
        const TArray<uint8> Signature,
        TArray<FString>& OutTransactionParams,
        FString& OutErrorMessage);
};