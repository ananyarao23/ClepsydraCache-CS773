#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <bitset>
#include <vector>

inline uint8_t get_nibble(const std::bitset<64>& bits, int high_bit) {
    return static_cast<uint8_t>((bits >> (high_bit - 3)).to_ullong() & 0xF);
}

inline void set_nibble(std::bitset<64>& bits, int high_bit, uint8_t nibble) {
    bits &= ~(std::bitset<64>(0xF) << (high_bit - 3)); // clear
    bits |= (std::bitset<64>(nibble & 0xF) << (high_bit - 3)); // set
}

uint64_t extract_tag(uint64_t address, int n);
uint64_t extract_index(uint64_t address, int n, int k);
uint32_t encode_addr(std::bitset<128> key, uint64_t text);
uint64_t decode_addr(std::bitset<128> key, uint32_t cipher);
std::vector<std::bitset<128>> generate_keys(size_t n);

#endif // BIT_UTILS_HPP
