#include "PayloadParamsUtils.h"
#include <string>

JSONVar emptyParamsArray;
JSONVar paramsArray;

uint32_t getEra() {
    return 0;
}

uint64_t getTip() {
    return 0;
}

std::string getGenesisBlockHash(BlockchainUtils *blockchainUtils) {
    // return "525639f713f397dcf839bd022cd821f367ebcf179de7b9253531f8adbe5436d6"; // Vara
    // return "631ccc82a078481584041656af292834e1ae6daab61d2875b4dd0c14bb9b17bc"; // Robonomics
    const char* res = getBlockHash(blockchainUtils, 0);
    std::string res_str(res);
    return res_str;
}

uint32_t getSpecVersion(JSONVar *runtimeInfo) {
    int specVersion_ = (int) (runtimeInfo->operator[]("specVersion"));
    return (uint32_t) specVersion_;
}

uint32_t getTransactionVersion(JSONVar *runtimeInfo) {
    int tx_version_ = (int)(runtimeInfo->operator[]("transactionVersion"));
    return (uint32_t) tx_version_;
}

// Get Nonce

uint64_t getNonce(BlockchainUtils *blockchainUtils, char *ss58Address) {
    paramsArray[0] = ss58Address;
    String message = blockchainUtils->createWebsocketMessage("system_accountNextIndex", paramsArray);
    JSONVar response = blockchainUtils->rpcRequest(message);
    int received_nonce = (int) (response["result"]);
    Serial.print("Nonce: ");
    Serial.println(received_nonce);
    return (uint64_t) (received_nonce);
}

// Get Block Hash

const char* getBlockHash(BlockchainUtils *blockchainUtils, int block_number) {
    paramsArray[0] = block_number;
    String message = blockchainUtils->createWebsocketMessage("chain_getBlockHash", paramsArray);
    JSONVar response = blockchainUtils->rpcRequest(message);
    Serial.print("Block 0 hash: ");
    Serial.println(response["result"]);
    return (const char*) response["result"];
}

// Get Runtime Info

JSONVar getRuntimeInfo(BlockchainUtils *blockchainUtils) {
    const char* chain_head_local = getChainHead(blockchainUtils);
    const char* parent_block_local = getParentBlockHash(chain_head_local, blockchainUtils);
    return getRuntimeInfo(parent_block_local, blockchainUtils);
}

JSONVar getRuntimeInfo(const char* parentBlockHash, BlockchainUtils *blockchainUtils) {
    paramsArray[0] = parentBlockHash;
    String message = blockchainUtils->createWebsocketMessage("state_getRuntimeVersion", paramsArray);
    JSONVar response = blockchainUtils->rpcRequest(message);
    return response["result"];
}

// Get Chain Head

const char* getChainHead(BlockchainUtils *blockchainUtils) {
    String message = blockchainUtils->createWebsocketMessage("chain_getHead", emptyParamsArray);
    JSONVar response = blockchainUtils->rpcRequest(message);
    const char* chain_head = (const char *) (response["result"]);
    Serial.print("Chain head: ");
    Serial.println(chain_head);
    return chain_head;
}

// Get Parent Block Hash

const char* getParentBlockHash(const char* chainHead, BlockchainUtils *blockchainUtils) {
    paramsArray[0] = chainHead;
    String message = blockchainUtils->createWebsocketMessage("chain_getHeader", paramsArray);
    JSONVar response = blockchainUtils->rpcRequest(message);
    const char* parent_block_hash = (const char *) (response["result"]["parentHash"]);
    Serial.print("Chain header: ");
    Serial.println(parent_block_hash);
    return parent_block_hash;
}
