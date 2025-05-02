// #include <bitset>
// #include <vector>
// #include <random>
// #include <iostream>

// std::vector<std::bitset<128>> generate_keys(size_t n) {
//     std::vector<std::bitset<128>> keys;
//     keys.reserve(n);

//     std::random_device rd;
//     std::mt19937_64 gen(rd());
//     std::uniform_int_distribution<uint64_t> dist;

//     for (size_t i = 0; i < n; ++i) {
//         uint64_t high = dist(gen);
//         uint64_t low = dist(gen);

//         std::bitset<128> key = (std::bitset<128>(high) << 64) | std::bitset<128>(low);
//         keys.push_back(key);
//     }

//     return keys;
// }
