#!/usr/bin/env python3
import json
import subprocess
import shutil
from pathlib import Path
from typing import Dict, List, Any

SIMULATOR_DIR = Path(__file__).parent
INPUT_DIR = SIMULATOR_DIR / "input"
TRAINING_DIR = SIMULATOR_DIR.parent / "ml-cloud-provisioning" / "training" / "simulation_runs"

ALLOCATION_MECHANISMS = ["Traditional", "SOSM", "Improved SOSM"]

MECHANISM_OUTPUT_DIRS = {
    "Traditional": SIMULATOR_DIR / "output" / "traditional",
    "SOSM": SIMULATOR_DIR / "output" / "sosm",
    "Improved SOSM": SIMULATOR_DIR / "output" / "improved",
}

CPU_POWER_BINS = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
CPU_POWER_DEFAULT = [163, 170.1251, 172.6057, 175.4264, 179.7886, 183.6121, 189.9905, 196.8095, 206.3168, 215.9316, 220.2148]
CPU_POWER_EFFICIENT = [120, 125, 128, 132, 138, 145, 152, 160, 170, 180, 190]
CPU_POWER_HIGH = [200, 215, 225, 240, 260, 280, 305, 335, 370, 410, 450]


def create_hw_type(hw_id: int, name: str, num_servers: int, cpus_per_server: int,
                   memory_per_server: int, storage: float, compute_cap: float,
                   has_accelerator: bool, acc_per_server: int, acc_compute: float,
                   cpu_power: List[float], acc_idle: float, acc_max: float) -> Dict:
    return {
        "HW type name": name,
        "HW type ID": hw_id,
        "Number of servers": num_servers,
        "Number of CPUs per server": cpus_per_server,
        "Memory per server": memory_per_server,
        "Storage per server": storage,
        "Processors overcommitment ratio": 1,
        "Memory overcommitment ratio": 1,
        "Compute capability": compute_cap,
        "Accelerators": 1 if has_accelerator else 0,
        "Number of accelerators per server": acc_per_server if has_accelerator else 0,
        "Accelerator compute capability": acc_compute if has_accelerator else 0,
        "Type of CPU model": 3,
        "CPU idle power consumption": 0,
        "CPU max power consumption": 0,
        "CPU number of points for interpolation": 11,
        "CPU utilization bins": CPU_POWER_BINS,
        "CPU power consumption": cpu_power,
        "CPU sleep power consumption": cpu_power[0],
        "Type of accelerator model": 1 if has_accelerator else 0,
        "Accelerator idle power consumption": acc_idle,
        "Accelerator max power consumption": acc_max,
        "Accelerator sleep power consumption": acc_idle
    }


def create_cell(cell_id: int, bandwidth: float, hw_types: List[Dict]) -> Dict:
    return {
        "Cell ID": cell_id,
        "Cell interconnection bandwidth": bandwidth,
        "Network bandwidth overcommitment ratio": 1,
        "Number of hardware(HW) types": len(hw_types),
        "HW types": hw_types
    }


def create_cell_config(sim_time: int, cells: List[Dict]) -> Dict:
    return {
        "Maximum simulation time": sim_time,
        "Update interval": 100,
        "Number of Cells": len(cells),
        "Cells": cells
    }


def create_app(app_id: int, implementations: List[int], instr_range: List[float],
               vm_range: List[int], vcpu_range: List[int], mem_range: List[int],
               storage_range: List[float], network_range: List[float],
               acc_support: List[int], rho_acc: List[float]) -> Dict:
    return {
        "Application ID": app_id,
        "Number of available implementations": len(implementations),
        "Available implementations": implementations,
        "Minimum - maximum instructions per application": instr_range,
        "Minimum - maximum VMs per application": vm_range,
        "Minimum - maximum vCPUs per VM": vcpu_range,
        "Minimum - maximum memory per VM": mem_range,
        "Minimum - maximum storage per VM": storage_range,
        "Minimum - maximum network per VM": network_range,
        "Minimum - maximum actual vCPU utilization": [1, 1],
        "Minimum - maximum actual memory utilization": [1, 1],
        "Minimum - maximum actual network utilization": [1, 1],
        "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
        "Accelerator support": acc_support,
        "Rho for accelerator execution per implementation": rho_acc
    }


def create_app_config(apps: List[Dict], job_rate: List[int]) -> Dict:
    return {
        "Number of applications": len(apps),
        "Minimum and maximum jobs per second": job_rate,
        "Applications": apps
    }


def create_broker_config(num_cells: int, mechanism: str) -> Dict:
    broker = {
        "Number of functions": 1,
        "Weights of functions": [1, 0, 0, 0, 0],
        "Number of Resources per vRM": 100,
        "Number of vRMs per pSwitch": 10,
        "Number of pSwitch per pRouter": 5,
        "Poll Interval Cell Manager": 20,
        "Poll Interval pRouter": 20,
        "Poll Interval pSwitch": 20,
        "Poll Interval vRM": 20,
        "vRM deployment strategy": 1
    }
    return {
        "_comment": "Resource allocation mechanism, choose between 'SOSM', 'Improved SOSM', 'Traditional', and 'ML'",
        "Resource allocation mechanism": mechanism,
        "Brokers": [broker for _ in range(num_cells)]
    }


CONFIGURATIONS = {
    "baseline": {
        "description": "Baseline balanced configuration",
        "cells": [
            {"servers": [5000, 5000, 5000, 5000], "cpus": 20, "mem": 128, "bandwidth": 20},
        ],
        "apps": "mixed",
        "job_rate": [5, 15],
        "sim_time": 1000
    },
    "baseline_v2": {
        "description": "Baseline with 2 cells",
        "cells": [
            {"servers": [4000, 4000, 4000, 4000], "cpus": 16, "mem": 64, "bandwidth": 15},
            {"servers": [4000, 4000, 4000, 4000], "cpus": 16, "mem": 64, "bandwidth": 15},
        ],
        "apps": "mixed",
        "job_rate": [5, 15],
        "sim_time": 1000
    },
    "balanced_medium": {
        "description": "Balanced datacenter with all HW types",
        "cells": [
            {"servers": [5000, 5000, 5000, 5000], "cpus": 20, "mem": 128, "bandwidth": 20},
            {"servers": [5000, 5000, 5000, 5000], "cpus": 20, "mem": 128, "bandwidth": 20},
        ],
        "apps": "mixed",
        "job_rate": [5, 15],
        "sim_time": 1000
    },
    "cpu_dominant": {
        "description": "CPU-heavy datacenter with minimal accelerators",
        "cells": [
            {"servers": [15000, 2000, 1500, 1500], "cpus": 24, "mem": 64, "bandwidth": 15},
            {"servers": [15000, 2000, 1500, 1500], "cpus": 24, "mem": 64, "bandwidth": 15},
            {"servers": [10000, 1000, 1000, 1000], "cpus": 16, "mem": 32, "bandwidth": 10},
        ],
        "apps": "cpu_intensive",
        "job_rate": [10, 25],
        "sim_time": 1200
    },
    "cpu_only": {
        "description": "CPU-only applications",
        "cells": [
            {"servers": [20000, 0, 0, 0], "cpus": 32, "mem": 64, "bandwidth": 20},
            {"servers": [15000, 0, 0, 0], "cpus": 24, "mem": 48, "bandwidth": 15},
        ],
        "apps": "cpu_intensive",
        "job_rate": [10, 30],
        "sim_time": 1000
    },
    "gpu_heavy": {
        "description": "GPU-accelerated datacenter for ML/AI workloads",
        "cells": [
            {"servers": [3000, 12000, 2000, 3000], "cpus": 32, "mem": 256, "bandwidth": 40},
            {"servers": [2000, 10000, 1500, 2500], "cpus": 32, "mem": 256, "bandwidth": 40},
        ],
        "apps": "gpu_intensive",
        "job_rate": [3, 12],
        "sim_time": 1000
    },
    "dfe_preferred": {
        "description": "DFE-heavy datacenter",
        "cells": [
            {"servers": [2000, 3000, 12000, 3000], "cpus": 24, "mem": 192, "bandwidth": 30},
            {"servers": [2000, 2000, 10000, 2000], "cpus": 24, "mem": 192, "bandwidth": 30},
        ],
        "apps": "hpc",
        "job_rate": [3, 10],
        "sim_time": 1000
    },
    "mic_preferred": {
        "description": "MIC-heavy datacenter",
        "cells": [
            {"servers": [2000, 2000, 2000, 14000], "cpus": 32, "mem": 256, "bandwidth": 40},
            {"servers": [1500, 1500, 1500, 12000], "cpus": 32, "mem": 256, "bandwidth": 40},
        ],
        "apps": "hpc",
        "job_rate": [3, 10],
        "sim_time": 1000
    },
    "small_efficient": {
        "description": "Small energy-efficient datacenter",
        "cells": [
            {"servers": [2000, 1000, 500, 500], "cpus": 16, "mem": 64, "bandwidth": 10, "power": "efficient"},
        ],
        "apps": "light",
        "job_rate": [2, 8],
        "sim_time": 800
    },
    "large_enterprise": {
        "description": "Large enterprise datacenter with heterogeneous resources",
        "cells": [
            {"servers": [25000, 12000, 6000, 5000], "cpus": 28, "mem": 192, "bandwidth": 30},
            {"servers": [20000, 15000, 8000, 7000], "cpus": 32, "mem": 256, "bandwidth": 35},
            {"servers": [15000, 8000, 5000, 4000], "cpus": 24, "mem": 128, "bandwidth": 25},
        ],
        "apps": "enterprise",
        "job_rate": [15, 40],
        "sim_time": 1500
    },
    "hpc_cluster": {
        "description": "HPC cluster with DFE and MIC accelerators",
        "cells": [
            {"servers": [5000, 2000, 8000, 5000], "cpus": 48, "mem": 512, "bandwidth": 100},
            {"servers": [4000, 1500, 6000, 8000], "cpus": 48, "mem": 512, "bandwidth": 100},
        ],
        "apps": "hpc",
        "job_rate": [2, 8],
        "sim_time": 1000
    },
    "high_load": {
        "description": "High utilization scenario",
        "cells": [
            {"servers": [8000, 8000, 4000, 4000], "cpus": 20, "mem": 128, "bandwidth": 20},
            {"servers": [8000, 8000, 4000, 4000], "cpus": 20, "mem": 128, "bandwidth": 20},
        ],
        "apps": "mixed",
        "job_rate": [25, 50],
        "sim_time": 800
    },
    "low_load": {
        "description": "Low utilization scenario",
        "cells": [
            {"servers": [10000, 5000, 3000, 2000], "cpus": 20, "mem": 128, "bandwidth": 20},
        ],
        "apps": "light",
        "job_rate": [1, 5],
        "sim_time": 1200
    },
    "memory_intensive": {
        "description": "Memory-heavy workloads (databases, caching)",
        "cells": [
            {"servers": [6000, 4000, 2000, 2000], "cpus": 16, "mem": 512, "bandwidth": 30},
            {"servers": [5000, 3000, 2000, 2000], "cpus": 16, "mem": 384, "bandwidth": 25},
        ],
        "apps": "memory_heavy",
        "job_rate": [5, 15],
        "sim_time": 1000
    },
    "edge_computing": {
        "description": "Edge computing with smaller nodes",
        "cells": [
            {"servers": [800, 300, 150, 300], "cpus": 8, "mem": 32, "bandwidth": 5},
            {"servers": [600, 200, 100, 200], "cpus": 8, "mem": 32, "bandwidth": 5},
            {"servers": [400, 150, 80, 150], "cpus": 4, "mem": 16, "bandwidth": 3},
        ],
        "apps": "edge",
        "job_rate": [8, 20],
        "sim_time": 600
    },
    "mixed_all_hw": {
        "description": "Mixed workloads with all HW types equally represented",
        "cells": [
            {"servers": [6000, 6000, 6000, 6000], "cpus": 24, "mem": 128, "bandwidth": 25},
            {"servers": [6000, 6000, 6000, 6000], "cpus": 24, "mem": 128, "bandwidth": 25},
        ],
        "apps": "mixed",
        "job_rate": [8, 20],
        "sim_time": 1200
    },
    "scarce_resources": {
        "description": "Scarce resources to create more rejections",
        "cells": [
            {"servers": [500, 500, 250, 250], "cpus": 8, "mem": 32, "bandwidth": 5},
        ],
        "apps": "enterprise",
        "job_rate": [15, 30],
        "sim_time": 800
    },
    "abundant_resources": {
        "description": "Abundant resources to minimize rejections",
        "cells": [
            {"servers": [50000, 30000, 15000, 15000], "cpus": 48, "mem": 512, "bandwidth": 100},
        ],
        "apps": "light",
        "job_rate": [2, 6],
        "sim_time": 800
    },
    "mixed_accel": {
        "description": "Mixed accelerator workloads",
        "cells": [
            {"servers": [4000, 8000, 8000, 8000], "cpus": 32, "mem": 256, "bandwidth": 50},
            {"servers": [4000, 8000, 8000, 8000], "cpus": 32, "mem": 256, "bandwidth": 50},
        ],
        "apps": "gpu_intensive",
        "job_rate": [5, 12],
        "sim_time": 1000
    },
}

APP_PROFILES = {
    "mixed": [
        create_app(1, [1, 2, 3], [1e9, 5e9], [1, 16], [4, 16], [4, 32], [0.01, 0.05], [0.001, 0.005], [0, 1, 1], [0, 0.7, 0.5]),
        create_app(2, [1, 2, 3], [5e8, 3e9], [1, 8], [8, 32], [8, 64], [0.02, 0.08], [0.002, 0.01], [0, 1, 1], [0, 0.8, 0.6]),
        create_app(3, [1, 4], [8e8, 4e9], [1, 4], [4, 12], [4, 16], [0.01, 0.04], [0.001, 0.003], [0, 1], [0, 0.9]),
    ],
    "cpu_intensive": [
        create_app(1, [1], [2e9, 8e9], [1, 32], [8, 32], [2, 8], [0.01, 0.02], [0.0005, 0.001], [0], [0]),
        create_app(2, [1], [1e9, 5e9], [1, 16], [16, 48], [4, 16], [0.01, 0.03], [0.001, 0.002], [0], [0]),
        create_app(3, [1], [5e8, 3e9], [1, 8], [4, 16], [2, 8], [0.005, 0.015], [0.0005, 0.001], [0], [0]),
    ],
    "gpu_intensive": [
        create_app(1, [1, 2], [5e9, 2e10], [1, 8], [4, 16], [16, 128], [0.1, 0.5], [0.01, 0.05], [0, 1], [0, 0.85]),
        create_app(2, [1, 2], [1e10, 5e10], [1, 4], [8, 32], [32, 256], [0.2, 1.0], [0.02, 0.1], [0, 1], [0, 0.9]),
        create_app(3, [2], [2e10, 1e11], [1, 2], [16, 64], [64, 512], [0.5, 2.0], [0.05, 0.2], [1], [0.95]),
    ],
    "light": [
        create_app(1, [1, 2], [1e8, 5e8], [1, 4], [2, 8], [2, 8], [0.005, 0.02], [0.0005, 0.002], [0, 1], [0, 0.5]),
        create_app(2, [1], [5e7, 2e8], [1, 2], [1, 4], [1, 4], [0.002, 0.01], [0.0002, 0.001], [0], [0]),
    ],
    "enterprise": [
        create_app(1, [1, 2, 3], [1e9, 5e9], [2, 16], [4, 16], [8, 64], [0.02, 0.1], [0.002, 0.01], [0, 1, 1], [0, 0.7, 0.6]),
        create_app(2, [1, 2], [5e8, 2e9], [1, 8], [8, 24], [16, 128], [0.05, 0.2], [0.005, 0.02], [0, 1], [0, 0.75]),
        create_app(3, [1, 4], [2e9, 8e9], [1, 4], [4, 16], [4, 32], [0.01, 0.05], [0.001, 0.005], [0, 1], [0, 0.8]),
        create_app(4, [1], [1e8, 5e8], [1, 32], [2, 8], [2, 16], [0.01, 0.03], [0.001, 0.003], [0], [0]),
    ],
    "hpc": [
        create_app(1, [1, 3, 4], [1e10, 1e11], [4, 64], [16, 64], [32, 256], [0.1, 0.5], [0.01, 0.05], [0, 1, 1], [0, 0.9, 0.85]),
        create_app(2, [1, 3], [5e9, 5e10], [2, 32], [32, 96], [64, 512], [0.2, 1.0], [0.02, 0.1], [0, 1], [0, 0.95]),
        create_app(3, [1, 4], [2e10, 2e11], [1, 16], [48, 128], [128, 1024], [0.5, 2.0], [0.05, 0.2], [0, 1], [0, 0.88]),
    ],
    "memory_heavy": [
        create_app(1, [1, 2], [5e8, 2e9], [1, 8], [4, 16], [64, 256], [0.1, 0.5], [0.01, 0.05], [0, 1], [0, 0.6]),
        create_app(2, [1], [2e8, 1e9], [1, 4], [8, 24], [128, 512], [0.2, 1.0], [0.02, 0.1], [0], [0]),
        create_app(3, [1, 2], [1e9, 4e9], [1, 16], [2, 8], [32, 128], [0.05, 0.2], [0.005, 0.02], [0, 1], [0, 0.55]),
    ],
    "edge": [
        create_app(1, [1, 2], [1e7, 1e8], [1, 2], [1, 4], [1, 4], [0.001, 0.005], [0.0001, 0.0005], [0, 1], [0, 0.4]),
        create_app(2, [1, 4], [5e6, 5e7], [1, 1], [2, 8], [2, 8], [0.002, 0.01], [0.0002, 0.001], [0, 1], [0, 0.5]),
        create_app(3, [1], [1e7, 5e7], [1, 4], [1, 2], [0.5, 2], [0.001, 0.003], [0.0001, 0.0003], [0], [0]),
    ],
}


def generate_cell_data(config: Dict) -> Dict:
    cells = []
    for i, cell_cfg in enumerate(config["cells"]):
        power_profile = cell_cfg.get("power", "default")
        if power_profile == "efficient":
            cpu_power = CPU_POWER_EFFICIENT
        elif power_profile == "high":
            cpu_power = CPU_POWER_HIGH
        else:
            cpu_power = CPU_POWER_DEFAULT

        hw_types = [
            create_hw_type(1, "CPU", cell_cfg["servers"][0], cell_cfg["cpus"], cell_cfg["mem"], 1.0,
                          cell_cfg["cpus"] * 4400.04, False, 0, 0, cpu_power, 0, 0),
            create_hw_type(2, "CPU+GPU", cell_cfg["servers"][1], cell_cfg["cpus"], cell_cfg["mem"], 1.0,
                          cell_cfg["cpus"] * 4400.04, True, 4, 587505.34, cpu_power, 32, 250),
            create_hw_type(3, "CPU+DFE", cell_cfg["servers"][2], cell_cfg["cpus"], cell_cfg["mem"], 1.0,
                          cell_cfg["cpus"] * 4400.04, True, 4, 507494.4, cpu_power, 176.8, 225),
            create_hw_type(4, "CPU+MIC", cell_cfg["servers"][3], cell_cfg["cpus"], cell_cfg["mem"], 1.0,
                          cell_cfg["cpus"] * 4400.04, True, 4, 295959.3, cpu_power, 17.3, 25.1),
        ]
        cells.append(create_cell(i + 1, cell_cfg["bandwidth"], hw_types))

    return create_cell_config(config["sim_time"], cells)


def generate_app_data(config: Dict) -> Dict:
    apps = APP_PROFILES[config["apps"]]
    return create_app_config(apps, config["job_rate"])


def run_simulation(config_name: str, mechanism: str) -> bool:
    print(f"    Running with {mechanism}...")
    import platform
    import os
    if platform.system() == "Linux" or os.path.exists("/mnt/c"):
        result = subprocess.run(
            ["./cl_sim.out"],
            cwd=str(SIMULATOR_DIR),
            capture_output=True,
            text=True
        )
    else:
        cmd = "cd /mnt/c/projects/demo/cloudlightning-simulator && ./cl_sim.out 2>&1"
        result = subprocess.run(["wsl", "bash", "-c", cmd], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"    Error: {result.stderr[:200] if result.stderr else result.stdout[:200]}")
        return False
    if "Elapsed time" in result.stdout:
        for line in result.stdout.split("\n"):
            if "Elapsed time" in line or "submitted tasks" in line:
                print(f"    {line.strip()}")
    return True


def copy_results(config_name: str, mechanism: str) -> int:
    mech_short = mechanism.lower().replace(" ", "_")
    target_dir = TRAINING_DIR / f"{config_name}_{mech_short}"
    target_dir.mkdir(parents=True, exist_ok=True)

    output_dir = MECHANISM_OUTPUT_DIRS[mechanism]
    csv_files = list(output_dir.glob("cell_*_decisions.csv"))
    for src in csv_files:
        shutil.copy2(src, target_dir / src.name)

    return len(csv_files)


def ensure_output_dirs():
    for output_dir in MECHANISM_OUTPUT_DIRS.values():
        output_dir.mkdir(parents=True, exist_ok=True)


def main():
    print("=" * 60)
    print("MULTI-MECHANISM TRAINING DATA GENERATION")
    print("=" * 60)
    print(f"Mechanisms: {', '.join(ALLOCATION_MECHANISMS)}")
    print(f"Configurations: {len(CONFIGURATIONS)}")
    print()

    ensure_output_dirs()

    total_simulations = 0
    successful_simulations = 0

    for config_name, config in CONFIGURATIONS.items():
        print(f"\n[{config_name}] {config['description']}")

        cell_data = generate_cell_data(config)
        app_data = generate_app_data(config)

        for mechanism in ALLOCATION_MECHANISMS:
            total_simulations += 1
            broker_data = create_broker_config(len(config["cells"]), mechanism)

            with open(INPUT_DIR / "CellData.json", "w", newline='\n') as f:
                json.dump(cell_data, f, indent=2)
            with open(INPUT_DIR / "AppData.json", "w", newline='\n') as f:
                json.dump(app_data, f, indent=2)
            with open(INPUT_DIR / "BrokerData.json", "w", newline='\n') as f:
                json.dump(broker_data, f, indent=2)

            if run_simulation(config_name, mechanism):
                num_files = copy_results(config_name, mechanism)
                mech_short = mechanism.lower().replace(" ", "_")
                print(f"    Copied {num_files} CSV files to training/{config_name}_{mech_short}/")
                successful_simulations += 1
            else:
                print(f"    Simulation failed!")

    print("\n" + "=" * 60)
    print(f"DONE - {successful_simulations}/{total_simulations} simulations completed")
    print("Run train_pipeline.py to train the model")
    print("=" * 60)


if __name__ == "__main__":
    main()
