#include "../blake/blake2.h"
#include "address.h"

const char *const AddressUtils::ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";


// result must be declared: char result[len * 137 / 100];
int AddressUtils::EncodeBase58(const unsigned char *bytes, int len, unsigned char result[]) {
    unsigned char digits[512] = {0};
    int digitslen = 1;
    for (int i = 0; i < len; i++) {
        unsigned int carry = (unsigned int)bytes[i];
        for (int j = 0; j < digitslen; j++) {
            carry += (unsigned int)(digits[j]) << 8;
            digits[j] = (unsigned char)(carry % 58);
            carry /= 58;
        }
        while (carry > 0) {
            digits[digitslen++] = (unsigned char)(carry % 58);
            carry /= 58;
        }
    }
    int resultlen = 0;
    for (; resultlen < len && bytes[resultlen] == 0;)
        result[resultlen++] = '1';
    for (int i = 0; i < digitslen; i++)
        result[resultlen + i] = ALPHABET[digits[digitslen - 1 - i]];
    result[digitslen + resultlen] = 0;
    return digitslen + resultlen;
}


std::string AddressUtils::getAddrFromPublicKey(RobonomicsPublicKey &pubKey) {
    unsigned char plainAddr[SR25519_PUBLIC_SIZE + 3] = {ROBONOMICS_PREFIX};
    memcpy(plainAddr + 1, pubKey.bytes, SR25519_PUBLIC_SIZE);

    uint8_t ssPrefixed[SR25519_PUBLIC_SIZE + 8] = {0x53, 0x53, 0x35, 0x38, 0x50, 0x52, 0x45};
    memcpy(ssPrefixed + 7, plainAddr, SR25519_PUBLIC_SIZE + 1);

    unsigned char blake2bHashed[64] = {0};
    blake2(blake2bHashed, 64, ssPrefixed, SR25519_PUBLIC_SIZE + 8, NULL, 0);
    plainAddr[1 + PUBLIC_KEY_LENGTH] = blake2bHashed[0];
    plainAddr[2 + PUBLIC_KEY_LENGTH] = blake2bHashed[1];

    unsigned char addrCh[SR25519_PUBLIC_SIZE * 2] = {0};

    int encodedLen = EncodeBase58(plainAddr, SR25519_PUBLIC_SIZE + 3, addrCh);

    std::string result((char*)addrCh, encodedLen);
    return result;
}