#include <bitset>
#include <iostream>
#include <iomanip>
#include <cstdint>

// Placeholder for the actual PRINCE core logic
std::bitset<64> prince_core(const std::bitset<64>& data_in, const std::bitset<64>& key);

std::bitset<64> prince_top(const std::bitset<64>& plaintext, const std::bitset<128>& key) {
    // Split keys
    std::bitset<64> k0_start;
    std::bitset<64> k0_end;
    std::bitset<64> k1;

    // Extract k0_start (upper 64 bits of key)
    for (int i = 0; i < 64; ++i) {
        k0_start[i] = key[i + 64];
    }

    // Extract k1 (lower 64 bits of key)
    for (int i = 0; i < 64; ++i) {
        k1[i] = key[i];
    }

    // Compute k0_end using the rule:
    // k0_end = key(64) & key(127 downto 66) & (key(65) xor key(127))
    k0_end[63] = key[64]; // MSB
    for (int i = 62; i > 0; --i) {
        k0_end[i] = key[i + 64 + 1]; // key(127 downto 66)
    }
    k0_end[0] = key[65] ^ key[127]; // LSB

    // Input whitening
    std::bitset<64> core_in = plaintext ^ k0_start;

    // Core encryption
    std::bitset<64> core_out = prince_core(core_in, k1);

    // Output whitening
    std::bitset<64> ciphertext = core_out ^ k0_end;

    return ciphertext;
}

uint64_t extract_tag(uint64_t address, int n) {
    return address >> (64 - n);
}

uint64_t extract_index(uint64_t address, int n, int k) {
    return (address >> (64 - n - k)) & ((1ULL << k) - 1);
}

uint32_t encode_addr(std::bitset<128> key, uint32_t text)
{
    std::bitset<64> plaintext(text);

    std::bitset<64> ciphertext = prince_top(plaintext,key);

    return ciphertext.to_ulong();
}


