#include <bitset>
#include <cstdint>

// The fixed alpha constant used in PRINCE
const std::bitset<64> ALPHA(0xC0AC29B7C97C50DDULL);

std::bitset<64> prince_top(const std::bitset<64>& plaintext, const std::bitset<128>& key);

// Encrypt using modified key to simulate decryption
std::bitset<64> prince_decrypt(const std::bitset<64>& ciphertext, const std::bitset<128>& original_key) {
    // Extract k0_start and k1
    std::bitset<64> k0_start = std::bitset<64>((original_key >> 64).to_ullong());
    std::bitset<64> k1 = std::bitset<64>(original_key.to_ullong());

    // Compute k0_end (same logic as in prince_top)
    std::bitset<64> k0_end;
    k0_end[63] = original_key[64]; // MSB
    for (int i = 62; i > 0; --i)
        k0_end[i] = original_key[i + 64 + 1]; // key(127 downto 66)
    k0_end[0] = original_key[65] ^ original_key[127]; // LSB

    // Construct decryption key = k0_end || (k1 XOR ALPHA)
    std::bitset<64> k1_prime = k1 ^ ALPHA;

    std::bitset<128> decryption_key = (std::bitset<128>(k0_end.to_ullong()) << 64) | k1_prime.to_ullong();

    // Use encryption logic on ciphertext with tweaked key
    return prince_top(ciphertext, decryption_key);
}
