#include "Robonomics.h"

void Robonomics::setup() {
    blockchainUtils.setup();
}

void Robonomics::sendDatalogRecord(std::string data) {
    // Data call = createCall();
    Data head_dr_  = Data{0x33,0};
    Data call = callDatalogRecord(head_dr_, data);
    std::vector<uint8_t> vk = hex2bytes(PRIV_KEY);
    std::copy(vk.begin(), vk.end(), privateKey_);

    Ed25519::derivePublicKey(publicKey_, privateKey_);

    uint64_t payloadNonce = getNonce(& blockchainUtils);
    std::string payloadBlockHash = getGenesisBlockHash();
    uint32_t payloadEra = getEra();
    uint64_t payloadTip = getTip();
    JSONVar runtimeInfo = getRuntimeInfo(& blockchainUtils);
    uint32_t payloadSpecVersion = getSpecVersion(runtimeInfo);
    uint32_t payloadTransactionVersion = getTransactionVersion(runtimeInfo);
    // Serial.printf("Spec version: %u, tx version: %u, nonce: %u, era: %u, tip: %u\n", payloadSpecVersion, payloadTransactionVersion, payloadNonce, payloadEra, payloadTip);

    Data data_ = createPayload(call, payloadEra, payloadNonce, payloadTip, payloadSpecVersion, payloadTransactionVersion, payloadBlockHash, payloadBlockHash);
    Data signature_ = createSignature(data_, privateKey_, publicKey_);
    std::vector<std::uint8_t> pubKey( reinterpret_cast<std::uint8_t*>(std::begin(publicKey_)), reinterpret_cast<std::uint8_t*>(std::end(publicKey_)));
    Data edata_ = createSignedExtrinsic(signature_, pubKey, payloadEra, payloadNonce, payloadTip, call);
    int requestId = blockchainUtils.getRequestId();
    sendExtrinsic(edata_, requestId);
}

Data Robonomics::createCall() {
    Data call;
    std::vector<uint8_t> callStr = hex2bytes(CALL_ENCODED);
    append(call, callStr);
    // Serial.printf("Call size %d : \n", call.size());
    // for (int k = 0; k < call.size(); k++) 
    //     Serial.printf("%02x", call[k]);
    // Serial.println("");
    return call;
}

Data Robonomics::createPayload(Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block) {
    Data data_ = doPayload (call, era, nonce, tip, sv, tv, gen, block);
    // Serial.printf("\nPayload size %d : \n", data_.size());
    // for (int k = 0; k < data_.size(); k++) 
    //     Serial.printf("%02x", data_[k]);
    // Serial.println("");
    return data_;
}

Data Robonomics::createSignature(Data data, uint8_t privateKey[32], uint8_t publicKey[32]) {
    Data signature_ = doSign (data, privateKey, publicKey);
    // Serial.printf("\nSignatured size %d : \n", signature_.size());
    // for (int k = 0; k < signature_.size(); k++) 
    //     Serial.printf("%02x", signature_[k]);
    // Serial.println("");
    return signature_;
}

Data Robonomics::createSignedExtrinsic(Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call) {
    Data edata_ = doEncode (signature, pubKey, era, nonce, tip, call);
    // Serial.printf("Extrinsic %s: size %d\n", "Datalog", edata_.size());
    // for (int k = 0; k < edata_.size(); k++) 
    //     Serial.printf("%02x", edata_[k]);
    // Serial.println("");
    return edata_;
}

void Robonomics::sendExtrinsic(Data extrinsicData, int requestId) {
    String extrinsicMessage = fillParamsJs(extrinsicData, requestId);
    // Serial.println("After to string: ");
    // Serial.print(extrinsicMessage);
    blockchainUtils.rpcRequest(extrinsicMessage);
    getExstrinsicResult();
}

void Robonomics::getExstrinsicResult() {
    blockchainUtils.setOnTextCallback([this](uint8_t *payload) {getExstrinsicResultCallback(payload);});
    while (!got_extrinsic_result) {
        blockchainUtils.websocketLoop();
    }
    // Serial.print("Extrinsic result: ");
    // Serial.println(extrinsicResult);
}

void Robonomics::getExstrinsicResultCallback(uint8_t *payload) {
    JSONVar received_message = JSON.parse((char *)payload);
    extrinsicResult = JSON.stringify(received_message["result"]);
    got_extrinsic_result = true;
}