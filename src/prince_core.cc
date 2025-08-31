#include <iostream>
#include <bitset>
#include <array>

using u64 = std::bitset<64>;

u64 sbox(const u64& in);
u64 sbox_inv(const u64& in);
u64 linear_m(const u64& in);
u64 linear_m_inv(const u64& in);
u64 linear_m_prime(const u64& in);

const std::array<u64, 12> RCS = {
    u64(0x0000000000000000), u64(0x13198A2E03707344), u64(0xA4093822299F31D0),
    u64(0x082EFA98EC4E6C89), u64(0x452821E638D01377), u64(0xBE5466CF34E90C6C),
    u64(0x7EF84F78FD955CB1), u64(0x85840851F1AC43AA), u64(0xC882D32F25323C54),
    u64(0x64A51195E0E3610D), u64(0xD3B5A399CA0C2399), u64(0xC0AC29B7C97C50DD)
};

// u64 prince_core(const u64& data_in, const u64& key) {
//     std::array<u64, 12> ims;

//     // Round 0
//     ims[0] = data_in ^ key ^ RCS[0];

//     // Rounds 1 to 5
//     for (int i = 1; i <= 5; ++i) {
//         u64 sb_out = sbox(ims[i - 1]);
//         u64 m_out = linear_m(sb_out);
//         ims[i] = m_out ^ RCS[i] ^ key;
//     }

//     // Middle layer
//     u64 middle1 = sbox(ims[5]);
//     u64 middle2 = linear_m_prime(middle1);
//     ims[6] = sbox_inv(middle2);

//     // Rounds 6 to 10
//     for (int i = 6; i <= 10; ++i) {
//         u64 m_in = ims[i] ^ key ^ RCS[i];
//         u64 sb_in = linear_m_inv(m_in);
//         ims[i + 1] = sbox_inv(sb_in);
//     }

//     // Round 11
//     return ims[11] ^ RCS[11] ^ key;
// }

u64 prince_core(const u64& data_in, const u64& key) {
    std::array<u64, 6> ims;

    // Round 0
    ims[0] = data_in ^ key ^ RCS[0];

    // Rounds 1 to 2 (forward)
    for (int i = 1; i <= 2; ++i) {
        u64 sb_out = sbox(ims[i - 1]);
        u64 m_out = linear_m(sb_out);
        ims[i] = m_out ^ RCS[i] ^ key;
    }

    // Middle layer
    u64 middle1 = sbox(ims[2]);
    u64 middle2 = linear_m_prime(middle1);
    ims[3] = sbox_inv(middle2);

    // Rounds 3 to 4 (backward)
    for (int i = 3; i <= 4; ++i) {
        u64 m_in = ims[i] ^ key ^ RCS[i + 2]; // Use RCS[5] and RCS[6]
        u64 sb_in = linear_m_inv(m_in);
        ims[i + 1] = sbox_inv(sb_in);
    }

    // Final round output
    return ims[5] ^ RCS[7] ^ key; // Use RCS[7] for round 4 final
}

