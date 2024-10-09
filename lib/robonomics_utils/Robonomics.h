#pragma once

#include "Call.h"
#include "Extrinsic.h"
#include "BlockchainUtils.h"

#include "Arduino.h"
#include "Secret.h"
#include "PayloadParamsUtils.h"

class Robonomics {
private:
    BlockchainUtils blockchainUtils;
    uint8_t publicKey_[KEYS_SIZE];
    uint8_t privateKey_[KEYS_SIZE];
    String extrinsicResult;
    bool got_extrinsic_result = false;
    Data createCall();
    Data createPayload(Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block);
    Data createSignature(Data data, uint8_t privateKey[32], uint8_t publicKey[32]);
    Data createSignedExtrinsic(Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call);
    void sendExtrinsic(Data extrinsicData, int requestId);
    void getExstrinsicResult();
    void getExstrinsicResultCallback(uint8_t *payload);
public:
    void setup();
    void sendDatalogRecord(std::string data);
};