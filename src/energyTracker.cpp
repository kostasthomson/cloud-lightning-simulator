#include "energyTracker.h"
#include <algorithm>

EnergyTracker::EnergyTracker(const power *_powerModels, int _numberOfTypes)
    : powerModels(_powerModels), numberOfTypes(_numberOfTypes) {}

double EnergyTracker::computeTaskEnergy(
    int hwType,
    double avgCpuUtil,
    double avgMemUtil,
    double avgAccUtil,
    int numAccelerators,
    double duration) const
{
    // Validate inputs
    if (hwType < 0 || hwType >= numberOfTypes)
    {
        return 0.0;
    }

    if (duration <= 0.0)
    {
        return 0.0;
    }

    // Clamp utilizations to [0, 1]
    avgCpuUtil = std::max(0.0, std::min(1.0, avgCpuUtil));
    avgMemUtil = std::max(0.0, std::min(1.0, avgMemUtil));
    avgAccUtil = std::max(0.0, std::min(1.0, avgAccUtil));

    // Compute average power during task execution
    double avgPower_W = computeInstantaneousPower(
        hwType, avgCpuUtil, avgMemUtil, avgAccUtil, numAccelerators);

    // Energy = Power × Time
    // Convert: Watts × seconds → kWh
    double energy_kWh = (avgPower_W * duration) / (1000.0 * 3600.0);

    return energy_kWh;
}

double EnergyTracker::computeInstantaneousPower(
    int hwType,
    double cpuUtil,
    double memUtil,
    double accUtil,
    int numAccelerators) const
{
    // Validate HW type
    if (hwType < 0 || hwType >= numberOfTypes)
    {
        return 0.0;
    }

    // Clamp utilizations
    cpuUtil = std::max(0.0, std::min(1.0, cpuUtil));
    memUtil = std::max(0.0, std::min(1.0, memUtil));
    accUtil = std::max(0.0, std::min(1.0, accUtil));

    // Use dominant resource utilization for CPU power
    // This is a common approach in cloud energy modeling
    double u = std::max(cpuUtil, memUtil);
    double rho = accUtil;
    int active = (u > 0.0 || rho > 0.0) ? 1 : 0;

    // FIXED: Create non-const copy of power object to call consumption()
    // The power::consumption() method is not const-qualified in your simulator
    power powerModelCopy = powerModels[hwType];

    // Call consumption on the copy
    double power_W = powerModelCopy.consumption(u, rho, active, numAccelerators);

    return power_W;
}