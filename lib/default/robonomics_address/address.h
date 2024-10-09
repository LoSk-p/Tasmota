#include <Arduino.h>
#include <string>

#define PUBLIC_KEY_LENGTH 32
#define ADDRESS_LENGTH 48
#define SR25519_PUBLIC_SIZE 32
#define ROBONOMICS_PREFIX 0x20

typedef struct RobonomicsPublicKey { unsigned char bytes[PUBLIC_KEY_LENGTH]; } RobonomicsPublicKey;

typedef struct Address { unsigned char symbols[ADDRESS_LENGTH]; } Address;

class AddressUtils {
private:
    static const char *const ALPHABET;
    static const int ALPHABET_MAP[128];
    static int EncodeBase58(const unsigned char *bytes, int len, unsigned char result[]);
    static int DecodeBase58(const unsigned char *str, int len, unsigned char *result);

public:
    static std::string getAddrFromPublicKey(RobonomicsPublicKey &pubKey);
};
