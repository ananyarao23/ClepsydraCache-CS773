#include <bitset>
#include <cstdint>
#include "bit_utils.hpp"

// Declare the shared matrix transformation
std::bitset<64> linear_m_prime(const std::bitset<64>& data_in);



std::bitset<64> linear_m_inv(const std::bitset<64>& data_in) {
    std::bitset<64> mprime_in;

    // Reverse the transposition from linear_m
    set_nibble(mprime_in, 63, get_nibble(data_in, 63));
    set_nibble(mprime_in, 59, get_nibble(data_in, 11));
    set_nibble(mprime_in, 55, get_nibble(data_in, 23));
    set_nibble(mprime_in, 51, get_nibble(data_in, 35));
    set_nibble(mprime_in, 47, get_nibble(data_in, 47));
    set_nibble(mprime_in, 43, get_nibble(data_in, 59));
    set_nibble(mprime_in, 39, get_nibble(data_in, 7));
    set_nibble(mprime_in, 35, get_nibble(data_in, 19));
    set_nibble(mprime_in, 31, get_nibble(data_in, 31));
    set_nibble(mprime_in, 27, get_nibble(data_in, 43));
    set_nibble(mprime_in, 23, get_nibble(data_in, 55));
    set_nibble(mprime_in, 19, get_nibble(data_in, 3));
    set_nibble(mprime_in, 15, get_nibble(data_in, 15));
    set_nibble(mprime_in, 11, get_nibble(data_in, 27));
    set_nibble(mprime_in, 7,  get_nibble(data_in, 39));
    set_nibble(mprime_in, 3,  get_nibble(data_in, 51));

    // Pass through the involutive matrix transformation
    return linear_m_prime(mprime_in);
}
