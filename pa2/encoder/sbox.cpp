#include <cstdint>
#include <bitset>
#include <array>

// 4-bit S-box lookup for PRINCE
uint8_t prince_s_box(uint8_t input) {
    static const std::array<uint8_t, 16> sbox = {
        0xB, 0xF, 0x3, 0x2,
        0xA, 0xC, 0x9, 0x1,
        0x6, 0x7, 0x8, 0x0,
        0xE, 0x5, 0xD, 0x4
    };

    return sbox[input & 0xF]; // ensure only 4 LSB are used
}

std::bitset<64> sbox(const std::bitset<64>& in) {
    std::bitset<64> out;
    for (int i = 0; i < 16; ++i) {
        uint8_t nibble = (in >> (i * 4)).to_ulong() & 0xF;
        uint8_t sb = prince_s_box(nibble);
        out |= (std::bitset<64>(sb) << (i * 4));
    }
    return out;
}


