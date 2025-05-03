#include <bitset>
#include <iostream>
#include <iomanip>
#include <cstdint>

// Placeholder for the actual PRINCE core logic
const std::bitset<64> ALPHA(0xC0AC29B7C97C50DDULL);

// Stub for PRINCE core (you'll replace this with your actual logic)
std::bitset<64> prince_core(const std::bitset<64>& data_in, const std::bitset<64>& key);

std::bitset<64> prince_top(const std::bitset<64>& plaintext, const std::bitset<128>& key) {
    std::bitset<64> k0_start, k0_end, k1;

    for (int i = 0; i < 64; ++i) {
        k0_start[i] = key[i + 64];
        k1[i] = key[i];
    }
    k0_end[0] = k0_start[0];
    for(int i = 63; i > 1; i--)
    {
        k0_end[64-i] = k0_start[i];
    }
    k0_end[63] = k0_start[1] ^ k0_start[63];

    std::bitset<64> core_in = plaintext ^ k0_start;
    std::bitset<64> core_out = prince_core(core_in, k1);
    std::bitset<64> ciphertext = core_out ^ k0_end;

    return ciphertext;
}

std::bitset<64> prince_decrypt(const std::bitset<64>& ciphertext, const std::bitset<128>& original_key) {
    std::bitset<64> k0_start, k1;
    for (int i = 0; i < 64; ++i) {
        k0_start[i] = original_key[i + 64];
        k1[i] = original_key[i];
    }

    std::bitset<64> k0_end;
    k0_end[0] = k0_start[0];
    for(int i = 63; i > 1; i--)
    {
        k0_end[64-i] = k0_start[i];
    }
    k0_end[63] = k0_start[1] ^ k0_start[63];

    std::bitset<64> k1_prime = k1 ^ ALPHA;
    std::bitset<64> core_in = ciphertext ^ k0_end;
    std::bitset<64> core_out = prince_core(core_in,k1_prime);
    std::bitset<64> decrypted_text = core_out ^ k0_start;
    return decrypted_text;
}

uint64_t extract_tag(uint64_t address, int n) {
    return address >> (64 - n);
}

uint64_t extract_index(uint64_t address, int n, int k) {
    return (address >> (64 - n - k)) & ((1ULL << k) - 1);
}

uint32_t encode_addr(std::bitset<128> key, uint64_t text) {
    std::bitset<64> plaintext(text);
    std::bitset<64> ciphertext = prince_top(plaintext, key);
    return static_cast<uint32_t>((ciphertext >> 32).to_ulong()); // top 32 bits
}

uint64_t decode_addr(std::bitset<128> key, uint32_t cipher)
{
    std::bitset<64> ciphertext(cipher);
    std::bitset<64> plaintext = prince_decrypt(ciphertext,key);
    return static_cast<uint32_t>((plaintext >> 64).to_ullong());
}

// Safely print bitset<64> in hex
void print_hex64(const std::string& label, const std::bitset<64>& bits) {
    uint64_t value = 0;
    for (int i = 0; i < 64; ++i) {
        if (bits[i]) {
            value |= (1ULL << i);
        }
    }
    std::cout << label << "0x" << std::hex << std::uppercase << value << "\n";
}