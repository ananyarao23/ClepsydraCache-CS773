#include "cache.h"
#include "../inc/bit_utils.h"

#define DEBUG_START() std::cout << "\033[1;33m[// DEBUG]\033[0m Entering: " << __func__ << std::endl;
#define DEBUG_END() std::cout << "\033[1;32m[// DEBUG]\033[0m Exitin: " << __func__ << std::endl;

// std::vector<std::bitset<128>> keys;

// initialize replacement state
void CACHE::base_initialize_replacement()
{
    // DEBUG_START();
    cout << NAME << " has LRU replacement policy" << endl;
    // DEBUG_END();
}

// find replacement victim
uint32_t CACHE::base_find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU
    // DEBUG_START();
    return lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type);
}

// called on every cache hit and cache fill
void CACHE::base_update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    // DEBUG_START();
    if ((type == WRITEBACK) && ip)
        assert(0);

    // uncomment this line to see the LLC accesses
    // cout << "CPU: " << cpu << "  LLC " << setw(9) << TYPE_NAME << " set: " << setw(5) << set << " way: " << setw(2) << way;
    // cout << hex << " paddr: " << setw(12) << paddr << " ip: " << setw(8) << ip << " victim_addr: " << victim_addr << dec << endl;

    // baseline LRU
    if (hit && (type == WRITEBACK)) // writeback hit does not update LRU state
        return;
        // DEBUG_END();
    return lru_update(set, way);
}

void CACHE::base_replacement_final_stats()
{
}

uint32_t CACHE::lru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // DEBUG_START();
    uint32_t way = 0;

    // fill invalid line first
    for (way = 0; way < NUM_WAY; way++)
    {
        if (block[set][way].valid == false)
        {

            DP(if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    // LRU victim
    if (way == NUM_WAY)
    {
        for (way = 0; way < NUM_WAY; way++)
        {
            if (block[set][way].lru == NUM_WAY - 1)
            {

                DP(if (warmup_complete[cpu]) {
                cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                cout << dec << " lru: " << block[set][way].lru << endl; });

                break;
            }
        }
    }

    if (way == NUM_WAY)
    {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }
    // DEBUG_END();
    return way;
}

vector<uint32_t> CACHE::get_dynamic_set(uint64_t addr)
{
    // DEBUG_START();
    uint64_t index;
    int tag_bits = 64-log2(BLOCK_SIZE)-log2(NUM_SET);
    index = extract_index(addr,tag_bits,log2(NUM_SET));
    vector<uint32_t> ciphers;
    for(int i = 0; i < NUM_WAY; i++)
    {
        // cout << "call 1" << endl;
        uint64_t cipher = encode_addr(keys[i],index);
        ciphers.push_back(cipher);
    }
    // DEBUG_END();
    return ciphers;
}

vector<uint32_t> CACHE::get_tags(uint64_t addr)
{
    // DEBUG_START();
    uint64_t tag;
    int tag_bits = 64-log2(BLOCK_SIZE)-log2(NUM_SET);
    tag = extract_tag(addr,tag_bits);
    vector<uint32_t> ciphers;
    for(int i = 0; i < NUM_WAY; i++)
    {
        // cout << "call 2" << endl;
        uint64_t cipher = encode_addr(keys[i],tag);
        ciphers.push_back(cipher);
    }
    // DEBUG_END();
    return ciphers;
}

uint32_t CACHE::ttl_victim(uint32_t cpu, uint64_t instr_id, uint32_t &set, uint64_t ip, uint64_t full_addr, uint32_t type)
{

    // todododo
    // cout << " in ttl_victim" << endl;
    // DEBUG_START();
    vector<uint32_t> dynamic_set = get_dynamic_set(full_addr);

    uint32_t new_set;
    // looking for entries whose TTLs have expired
    for(int i = 0; i < NUM_WAY; i++)
    {
        new_set = dynamic_set[i] % NUM_SET;
        if (block[new_set][i].ttl == 0)
        {
            DP(if (warmup_complete[cpu]) {
                cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << new_set << " way: " << i;
                cout << hex << " address: " << (full_addr >> LOG2_BLOCK_SIZE) << " victim address: " << block[new_set][i].address << " data: " << block[new_set][i].data;
            });
            update_rttl(0);
            set = new_set;
            return i;
        }
    }
    

    // no expired entry, randomly evict one
    int way = std::rand() % dynamic_set.size();
    new_set = dynamic_set[way] % NUM_SET;
    int tag_bits = 64-log2(BLOCK_SIZE)-log2(NUM_SET);
    uint64_t index = extract_index(full_addr,tag_bits,log2(NUM_SET));
    evicted_indices[index][decode_addr(keys[way],new_set)]++;

    DP(if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << new_set << " way: " << way;
        cout << hex << " address: " << (full_addr >> LOG2_BLOCK_SIZE) << " victim address: " << block[new_set][way].address << " data: " << block[new_set][way].data;
    });

    update_rttl(1);
    set = new_set;
    // DEBUG_END();
    return way;
}

void CACHE::lru_update(uint32_t set, uint32_t way)
{
    // update lru replacement state
    // DEBUG_START();
    for (uint32_t i = 0; i < NUM_WAY; i++)
    {
        if (block[set][i].lru < block[set][way].lru)
        {
            block[set][i].lru++;
        }
    }
    block[set][way].lru = 0; // promote to the MRU position
    // DEBUG_END();
}

void CACHE::ttl_update(uint32_t set, uint32_t way, uint64_t full_addr)
{
    // update lru replacement state
    // DEBUG_START();
    // cout << "[RTTL] " << rTTL << endl;
    for (uint32_t i = 0; i < NUM_WAY; i++)
    {
        // cout <<"this" << endl;
        if(block[set][i].ttl > rTTL)
        {
            block[set][i].ttl -= rTTL;
        }
        else block[set][i].ttl = 0;
        // cout << "that" << endl;
        // cout << block[set][i].ttl <<" ";
        if (block[set][i].ttl == 0)
        {
            block[set][i].valid = 0;
        }
    }
    // cout<<endl;

    std::random_device rd;                              // Seed source
    std::mt19937 gen(rd());                             // Mersenne Twister PRNG
    std::uniform_int_distribution<int> dis(lTTL, uTTL); // Uniform distribution
    int randomValue = dis(gen);

    block[set][way].ttl = randomValue; // promote to the MRU position
    block[set][way].valid = 1;
    // cout << "end" << endl;
    // DEBUG_END();
    // vector<uint64_t> tags = get_tags(full_addr);
    // uint64_t tag = tags[way];
    // block[set][way].tag = tag;
}