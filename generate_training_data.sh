#!/bin/bash
# Training Data Generator - Runs simulator with multiple configurations
# Usage: ./generate_training_data.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INPUT_DIR="$SCRIPT_DIR/input"
OUTPUT_DIR="$SCRIPT_DIR/output/improved"
TRAINING_DIR="$SCRIPT_DIR/../cloud-sim-python/training/simulation_runs"

# Backup original configs
mkdir -p "$INPUT_DIR/backup"
cp "$INPUT_DIR/CellData.json" "$INPUT_DIR/backup/" 2>/dev/null || true
cp "$INPUT_DIR/AppData.json" "$INPUT_DIR/backup/" 2>/dev/null || true

# Create training directory
mkdir -p "$TRAINING_DIR"

run_config() {
    local config_name=$1
    echo ""
    echo "=========================================="
    echo "Running configuration: $config_name"
    echo "=========================================="

    # Clean previous output
    rm -f "$OUTPUT_DIR"/cell_*_decisions.csv

    # Run simulator
    ./cl_sim.out

    # Collect results
    local config_dir="$TRAINING_DIR/$config_name"
    mkdir -p "$config_dir"

    local count=$(ls -1 "$OUTPUT_DIR"/cell_*_decisions.csv 2>/dev/null | wc -l)
    if [ "$count" -gt 0 ]; then
        cp "$OUTPUT_DIR"/cell_*_decisions.csv "$config_dir/"
        echo "Collected $count CSV files to $config_dir"
    else
        echo "WARNING: No CSV files generated!"
    fi
}

# Configuration 1: Baseline (original config)
cp "$INPUT_DIR/backup/CellData.json" "$INPUT_DIR/CellData.json"
cp "$INPUT_DIR/backup/AppData.json" "$INPUT_DIR/AppData.json"
run_config "baseline"

# Configuration 2: High Load
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [15, 50],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [1386228336.48, 5544913345.92],
      "Minimum - maximum VMs per application": [2, 32],
      "Minimum - maximum vCPUs per VM": [4, 16],
      "Minimum - maximum memory per VM": [4, 16],
      "Minimum - maximum storage per VM": [0.02, 0.08],
      "Minimum - maximum network per VM": [0.0025, 0.01],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.7, 0.6, 0.5]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [462076112.16, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 16],
      "Minimum - maximum vCPUs per VM": [8, 32],
      "Minimum - maximum memory per VM": [8, 16],
      "Minimum - maximum storage per VM": [0.01, 0.04],
      "Minimum - maximum network per VM": [0.0005, 0.002],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.8, 0.75, 0.6]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [693114168.24, 4158685009.44],
      "Minimum - maximum VMs per application": [1, 8],
      "Minimum - maximum vCPUs per VM": [4, 16],
      "Minimum - maximum memory per VM": [4, 16],
      "Minimum - maximum storage per VM": [0.04, 0.16],
      "Minimum - maximum network per VM": [0.0025, 0.01],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.85, 0.8, 0.9]
    }
  ]
}
EOF
run_config "high_load"

# Configuration 3: Low Load with small tasks
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [1, 8],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [693114168.24, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [2, 4],
      "Minimum - maximum memory per VM": [2, 4],
      "Minimum - maximum storage per VM": [0.01, 0.02],
      "Minimum - maximum network per VM": [0.001, 0.002],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.5, 0.4, 0.3]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [462076112.16, 1386228336.48],
      "Minimum - maximum VMs per application": [1, 2],
      "Minimum - maximum vCPUs per VM": [2, 8],
      "Minimum - maximum memory per VM": [2, 8],
      "Minimum - maximum storage per VM": [0.005, 0.01],
      "Minimum - maximum network per VM": [0.0002, 0.0005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.6, 0.5, 0.4]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [346557084.12, 1386228336.48],
      "Minimum - maximum VMs per application": [1, 2],
      "Minimum - maximum vCPUs per VM": [2, 4],
      "Minimum - maximum memory per VM": [2, 4],
      "Minimum - maximum storage per VM": [0.02, 0.04],
      "Minimum - maximum network per VM": [0.001, 0.002],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.7, 0.6, 0.8]
    }
  ]
}
EOF
run_config "low_load"

# Configuration 4: CPU Only workload
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [5, 25],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 1,
      "Available implementations": [1],
      "Minimum - maximum instructions per application": [1386228336.48, 5544913345.92],
      "Minimum - maximum VMs per application": [1, 16],
      "Minimum - maximum vCPUs per VM": [4, 16],
      "Minimum - maximum memory per VM": [4, 16],
      "Minimum - maximum storage per VM": [0.02, 0.04],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0],
      "Rho for accelerator execution per implementation": [0]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 1,
      "Available implementations": [1],
      "Minimum - maximum instructions per application": [462076112.16, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 8],
      "Minimum - maximum vCPUs per VM": [8, 32],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.01, 0.02],
      "Minimum - maximum network per VM": [0.0005, 0.001],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0],
      "Rho for accelerator execution per implementation": [0]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 1,
      "Available implementations": [1],
      "Minimum - maximum instructions per application": [693114168.24, 4158685009.44],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.04, 0.08],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0],
      "Rho for accelerator execution per implementation": [0]
    }
  ]
}
EOF
run_config "cpu_only"

# Configuration 5: GPU Heavy workload
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [5, 30],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 2,
      "Available implementations": [1, 2],
      "Minimum - maximum instructions per application": [1386228336.48, 5544913345.92],
      "Minimum - maximum VMs per application": [1, 8],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [8, 16],
      "Minimum - maximum storage per VM": [0.02, 0.04],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.9]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 2,
      "Available implementations": [1, 2],
      "Minimum - maximum instructions per application": [462076112.16, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [8, 16],
      "Minimum - maximum memory per VM": [8, 16],
      "Minimum - maximum storage per VM": [0.01, 0.02],
      "Minimum - maximum network per VM": [0.0005, 0.001],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.85]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 2,
      "Available implementations": [1, 2],
      "Minimum - maximum instructions per application": [693114168.24, 4158685009.44],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [8, 16],
      "Minimum - maximum storage per VM": [0.04, 0.08],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.95]
    }
  ]
}
EOF
run_config "gpu_heavy"

# Configuration 6: DFE Preferred workload
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [5, 25],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 2,
      "Available implementations": [1, 3],
      "Minimum - maximum instructions per application": [1386228336.48, 5544913345.92],
      "Minimum - maximum VMs per application": [1, 8],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.02, 0.04],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.85]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 2,
      "Available implementations": [1, 3],
      "Minimum - maximum instructions per application": [462076112.16, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [8, 16],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.01, 0.02],
      "Minimum - maximum network per VM": [0.0005, 0.001],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.8]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 2,
      "Available implementations": [1, 3],
      "Minimum - maximum instructions per application": [693114168.24, 4158685009.44],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.04, 0.08],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.9]
    }
  ]
}
EOF
run_config "dfe_preferred"

# Configuration 7: MIC Preferred workload
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [5, 25],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 2,
      "Available implementations": [1, 4],
      "Minimum - maximum instructions per application": [1386228336.48, 5544913345.92],
      "Minimum - maximum VMs per application": [1, 8],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.02, 0.04],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.9]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 2,
      "Available implementations": [1, 4],
      "Minimum - maximum instructions per application": [462076112.16, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [8, 16],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.01, 0.02],
      "Minimum - maximum network per VM": [0.0005, 0.001],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.85]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 2,
      "Available implementations": [1, 4],
      "Minimum - maximum instructions per application": [693114168.24, 4158685009.44],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.04, 0.08],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1],
      "Rho for accelerator execution per implementation": [0, 0.95]
    }
  ]
}
EOF
run_config "mic_preferred"

# Configuration 8: Mixed All HW Types
cat > "$INPUT_DIR/AppData.json" << 'EOF'
{
  "Number of applications": 3,
  "Minimum and maximum jobs per second": [10, 35],
  "Applications": [
    {
      "Application ID": 1,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [1386228336.48, 5544913345.92],
      "Minimum - maximum VMs per application": [1, 16],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.02, 0.04],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.7, 0.6, 0.5]
    },
    {
      "Application ID": 2,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [462076112.16, 2772456672.96],
      "Minimum - maximum VMs per application": [1, 8],
      "Minimum - maximum vCPUs per VM": [8, 16],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.01, 0.02],
      "Minimum - maximum network per VM": [0.0005, 0.001],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.8, 0.75, 0.6]
    },
    {
      "Application ID": 3,
      "Number of available implementations": 4,
      "Available implementations": [1, 2, 3, 4],
      "Minimum - maximum instructions per application": [693114168.24, 4158685009.44],
      "Minimum - maximum VMs per application": [1, 4],
      "Minimum - maximum vCPUs per VM": [4, 8],
      "Minimum - maximum memory per VM": [4, 8],
      "Minimum - maximum storage per VM": [0.04, 0.08],
      "Minimum - maximum network per VM": [0.0025, 0.005],
      "Minimum - maximum actual vCPU utilization": [1, 1],
      "Minimum - maximum actual memory utilization": [1, 1],
      "Minimum - maximum actual network utilization": [1, 1],
      "Type of actual utilization (vCPU,Memory,Network)": [1, 1, 1],
      "Accelerator support": [0, 1, 1, 1],
      "Rho for accelerator execution per implementation": [0, 0.85, 0.8, 0.9]
    }
  ]
}
EOF
run_config "mixed_all_hw"

# Restore original configs
cp "$INPUT_DIR/backup/CellData.json" "$INPUT_DIR/CellData.json"
cp "$INPUT_DIR/backup/AppData.json" "$INPUT_DIR/AppData.json"

echo ""
echo "=========================================="
echo "TRAINING DATA GENERATION COMPLETE"
echo "=========================================="
echo "Output directory: $TRAINING_DIR"
echo ""
echo "To merge and train, run:"
echo "  cd ../cloud-sim-python/scripts"
echo "  python dataset_merger.py"
echo "  python train_agent.py"
