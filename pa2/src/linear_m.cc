#include <bitset>
#include <cstdint>
#include "../inc/bit_utils.h"

// Declare the linear_mprime function (you must implement this elsewhere)
std::bitset<64> linear_m_prime(const std::bitset<64>& data_in);



std::bitset<64> linear_m(const std::bitset<64>& data_in) {
    std::bitset<64> mprime_out = linear_m_prime(data_in);
    std::bitset<64> data_out;

    // Perform nibble-wise transposition (bit positions are high bits of nibbles)
    set_nibble(data_out, 63, get_nibble(mprime_out, 63));
    set_nibble(data_out, 59, get_nibble(mprime_out, 43));
    set_nibble(data_out, 55, get_nibble(mprime_out, 23));
    set_nibble(data_out, 51, get_nibble(mprime_out, 3));
    set_nibble(data_out, 47, get_nibble(mprime_out, 47));
    set_nibble(data_out, 43, get_nibble(mprime_out, 27));
    set_nibble(data_out, 39, get_nibble(mprime_out, 7));
    set_nibble(data_out, 35, get_nibble(mprime_out, 51));
    set_nibble(data_out, 31, get_nibble(mprime_out, 31));
    set_nibble(data_out, 27, get_nibble(mprime_out, 11));
    set_nibble(data_out, 23, get_nibble(mprime_out, 55));
    set_nibble(data_out, 19, get_nibble(mprime_out, 35));
    set_nibble(data_out, 15, get_nibble(mprime_out, 15));
    set_nibble(data_out, 11, get_nibble(mprime_out, 59));
    set_nibble(data_out, 7,  get_nibble(mprime_out, 39));
    set_nibble(data_out, 3,  get_nibble(mprime_out, 19));

    return data_out;
}
