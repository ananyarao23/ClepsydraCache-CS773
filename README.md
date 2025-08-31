# ClepsydraCache-CS773

Contributors: Deeksha Dhiwakar (22B0988), Ananya Rao (22B0980)

A ChampSim-based cache research project implementing advanced cache replacement policies, with a focus on TTL (Time-To-Live) replacement strategies and cryptographic encoder components.

## Overview

This repository contains a modified version of ChampSim (a trace-based CPU simulator) extended with custom cache replacement policies and cryptographic encoding capabilities. The project explores novel cache management techniques to improve system performance.

## Key Features

- **Custom Cache Replacement Policies**: Implementation of TTL-based replacement strategies
- **Cryptographic Encoder**: PRINCE cipher implementation for secure cache operations
- **Multi-core Simulation**: Support for 2-core simulation configurations
- **Comprehensive Prefetcher Suite**: Multiple prefetching algorithms for different cache levels
- **Flexible Configuration**: Modular design allowing easy swapping of components

## Project Structure

```
├── bin/                    # Compiled binaries
├── branch/                 # Branch predictors (bimodal, gshare, perceptron, etc.)
├── encoder/                # Cryptographic encoder implementation (PRINCE cipher)
├── inc/                    # Header files
├── prefetcher/             # Cache prefetchers for all levels (L1I, L1D, L2C, LLC, TLB)
├── replacement/            # Cache replacement policies including TTL implementation
├── src/                    # Core ChampSim source code
├── scripts/                # Utility scripts
├── tracer/                 # Trace generation tools
├── build_champsim.sh       # Build script
├── run_2core.sh           # Execution script for 2-core simulations
├── workloads.txt          # Available trace workloads
└── Makefile               # Build configuration
```

## Prerequisites

- **GCC 7**: Required for compilation
- **Linux Environment**: Tested on Ubuntu/Debian systems
- **Trace Files**: ChampSim trace files (download separately)

### Installing GCC 7 on Ubuntu

```bash
sudo apt update
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get install gcc-7 g++-7

# Set alternatives
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 0
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 0

# Configure if multiple versions exist
sudo update-alternatives --config gcc
sudo update-alternatives --config g++
```

## Quick Start

### 1. Build the Simulator

Use the provided build script with the following parameters:

```bash
./build_champsim.sh [branch_pred] [l1i_pref] [l1d_pref] [l2c_pref] [llc_pref] \
                    [itlb_pref] [dtlb_pref] [stlb_pref] [btb_repl] [l1i_repl] \
                    [l1d_repl] [l2c_repl] [llc_repl] [itlb_repl] [dtlb_repl] \
                    [stlb_repl] [num_cores] [tail_name]
```

**Example build commands:**

```bash
# Standard LRU configuration
./build_champsim.sh bimodal no no no no no no no lru lru lru lru lru lru lru lru 2 no

# TTL replacement for LLC
./build_champsim.sh bimodal no no no no no no no lru lru lru lru ttl_replacement lru lru lru 2 no
```

### 2. Download Trace Files

Download the required trace files from:
[Trace Files Repository](https://drive.google.com/drive/folders/1tG-oFIsxFza3GYPzUsMvj_vmLYvQeEcL?usp=sharing)

Extract traces to a `traces/` directory or update the path in `run_2core.sh`.

### 3. Run Simulations

Execute 2-core simulations using:

```bash
./run_2core.sh <binary_path> <workload1_number> <workload2_number>
```

**Example:**

```bash
# Run with workloads 1 and 2 from workloads.txt
./run_2core.sh bin/bimodal-no-no-no-no-no-no-no-lru-lru-lru-lru-ttl_replacement-lru-lru-lru-2core-no 1 2
```

## Available Components

### Branch Predictors
- `bimodal`: Two-level adaptive predictor
- `gshare`: Global history share predictor
- `perceptron`: Perceptron-based predictor
- `hashed_perceptron`: Enhanced perceptron variant

### Prefetchers
- **L1I**: Instruction cache prefetchers (Entangling, FNLMMA, MANA)
- **L1D**: Data cache prefetchers (IP-stride, IPCP, Berti, Bingo, etc.)
- **L2C**: L2 cache prefetchers
- **LLC**: Last-level cache prefetchers
- **TLB**: Translation lookaside buffer prefetchers

### Replacement Policies
- `lru`: Least Recently Used
- `ttl_replacement`: Time-To-Live based replacement (custom implementation)

## Configuration Files

- **workloads.txt**: Contains available trace files
- **Makefile**: Build configuration with C++11 support and optimization flags
- **build_champsim.sh**: Automated build script with parameter validation
- **run_2core.sh**: Execution wrapper for multi-core simulations

## Research Focus

This project specifically investigates:

1. **TTL-based Cache Replacement**: Novel time-aware replacement policies
2. **Cryptographic Cache Security**: PRINCE cipher integration for secure caching
3. **Multi-core Performance**: Dual-core simulation and workload interactions
4. **Prefetching Optimization**: Evaluation of various prefetching strategies

## Output and Results

Simulation results are stored in the `results/` directory with detailed performance metrics including:
- Cache hit/miss rates
- Instructions per cycle (IPC)
- Memory bandwidth utilization
- Replacement policy effectiveness

## Development

### Key Files for Modification

- `src/cache.cc`: Core cache implementation
- `src/main.cc`: Main simulation loop
- `replacement/ttl_replacement.llc_repl`: TTL replacement policy
- `encoder/`: Cryptographic components

### Adding New Components

1. **New Replacement Policy**: Add `.llc_repl` file in `replacement/` directory
2. **New Prefetcher**: Add appropriate `.pref` file in `prefetcher/` directory
3. **New Branch Predictor**: Add `.bpred` file in `branch/` directory

## Troubleshooting

- **Build Errors**: Ensure GCC 7 is properly installed and configured
- **Missing Traces**: Verify trace file paths in `run_2core.sh`
- **Deadlock Assertions**: Check cache configuration parameters for conflicts

## License

This project extends ChampSim and follows its original licensing terms. See `LICENSE` file for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement changes with appropriate tests
4. Submit a pull request with detailed description

## Acknowledgments

Built upon the ChampSim simulation framework. Special thanks to the original ChampSim developers and the computer architecture research community.

---

For detailed usage instructions, see `HOWTO.md`. For questions or issues, please open a GitHub issue.
