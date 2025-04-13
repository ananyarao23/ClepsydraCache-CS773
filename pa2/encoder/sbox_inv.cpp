#include <cstdint>
#include <bitset>
#include <array>

// 4-bit inverse S-box for PRINCE
uint8_t prince_sbox_inv(uint8_t input) {
    static const std::array<uint8_t, 16> sbox_inv = {
        0xB, 0x7, 0x3, 0x2,
        0xF, 0xD, 0x8, 0x9,
        0xA, 0x6, 0x4, 0x0,
        0x5, 0xE, 0xC, 0x1
    };

    return sbox_inv[input & 0xF];
}

std::bitset<64> sbox_inv(const std::bitset<64>& in) {
    std::bitset<64> out;
    for (int i = 0; i < 16; ++i) {
        uint8_t nibble = (in >> (i * 4)).to_ulong() & 0xF;
        uint8_t sb = prince_sbox_inv(nibble);
        out |= (std::bitset<64>(sb) << (i * 4));
    }
    return out;
}
