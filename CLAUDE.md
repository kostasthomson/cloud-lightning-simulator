# CloudLightning Simulator

## Project Overview
C++ discrete-event cloud simulator implementing the Self-Organising, Self-Managing (SOSM) resource allocation framework. Generates training data (CSV logs) for ML-based provisioning models.

## Architecture

### Core Components
- **cell**: Top-level container with multiple hardware types, power models, and network
- **resource**: Individual server with CPU, memory, storage, accelerators, compute capability
- **power**: Power consumption models (CPU bins, accelerator idle/max power)
- **netw**: Network bandwidth management (physical, available, utilized)
- **task**: Workload unit with VM count, resource requirements, implementation types

### SOSM Hierarchy (improvedSosmBroker)
```
Cell Manager (sPMSA aggregated state)
    └── pRouters (per hardware type)
        └── pSwitches (groups of vRMs)
            └── vRMs (Virtual Resource Managers)
                └── Resources (physical servers)
```

### Hardware Types
- Type 1: CPU-only
- Type 2: GPU (accelerator=1)
- Type 3: DFE (accelerator=2)
- Type 4: MIC (accelerator=3)

## Key Files

### Headers (include/)
| File | Purpose |
|------|---------|
| `cell.h` | Cell container with resources, power, network |
| `resource.h` | Server resource management |
| `power.h` | Power consumption models |
| `netw.h` | Network bandwidth tracking |
| `task.h` | Task/workload definition |
| `improvedSosmBroker.h` | SOSM broker implementation |
| `decisionLogEnhanced.h` | CSV logging for ML training data |

### Sources (src/)
| File | Purpose |
|------|---------|
| `cl_sim.cpp` | Main simulation entry point |
| `improvedSosmBroker.cpp` | Task allocation with SOSM algorithm |
| `cell.cpp` | Cell lifecycle and statistics |

### Configuration (input/)
| File | Purpose |
|------|---------|
| `CellData.json` | Cell and resource configuration |
| `BrokerData.json` | SOSM broker parameters |
| `AppData.json` | Application/task definitions |

## CSV Logging (decisionLogEnhanced.h)

### Output Location
`output/improved/cell_{id}_decisions.csv`

### Current Fields
```
num_vms, cpu_req, mem_req, util_cpu_before, util_mem_before,
avail_cpu_before, avail_mem_before, avail_storage_before,
avail_accelerators_before, energy_kwh, chosen_hw_type, accepted
```

### sPMSA Array Structure (per hardware type)
- [0]: Available processors
- [1]: Total processors
- [2]: Available memory
- [3]: Total memory
- [4]: Available storage
- [5]: Total storage
- [6]: Available accelerators
- [7]: Total accelerators

## Power Model

### CPU Power
Piecewise linear interpolation from utilization bins:
```cpp
double* cpubins;  // Utilization breakpoints [0.0, 0.1, ..., 1.0]
double* cpuP;     // Power at each breakpoint
```

### Accelerator Power
```cpp
double accPmin;  // Idle power
double accPmax;  // Max power
// P_acc = P_idle + rho * (P_max - P_idle)
```

## Build & Run

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)

export OMP_NUM_THREADS=4
./cl_sim ../input/CellData.json ../input/BrokerData.json ../input/AppData.json
```

## Integration with ML Project
CSV logs from `output/improved/` feed into `../ml-cloud-provisioning/` for training energy-aware neural network models. The goal is to learn optimal hardware type selection minimizing energy consumption.

## Conventions
- Use existing SOSM patterns for new brokers
- Power values in Watts, energy in kWh
- Resource quantities use double for overcommitment support
- Thread parallelism via OpenMP (`OMP_NUM_THREADS`)
