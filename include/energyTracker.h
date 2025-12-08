/* Energy Tracker for Cloud Simulator
 * Computes actual energy consumption for tasks based on power models
 * Author: Thesis Implementation
 * Date: December 2025
 */

#ifndef ENERGYTRACKER_H
#define ENERGYTRACKER_H

#include "power.h"
#include <algorithm>

class EnergyTracker
{
private:
    const power *powerModels; // Reference to cell's power consumption models
    int numberOfTypes;        // Number of HW types

public:
    /**
     * Constructor
     * @param _powerModels Pointer to array of power models (one per HW type)
     * @param _numberOfTypes Number of hardware types
     */
    EnergyTracker(const power *_powerModels, int _numberOfTypes);

    /**
     * Computes energy consumed by a task during execution
     * Uses the existing power::consumption() method
     *
     * @param hwType Hardware type index (0 to numberOfTypes-1)
     * @param avgCpuUtil Average CPU utilization [0, 1]
     * @param avgMemUtil Average memory utilization [0, 1]
     * @param avgAccUtil Average accelerator utilization [0, 1]
     * @param numAccelerators Number of accelerators used
     * @param duration Task execution time in seconds
     * @return Energy consumed in kWh
     */
    double computeTaskEnergy(
        int hwType,
        double avgCpuUtil,
        double avgMemUtil,
        double avgAccUtil,
        int numAccelerators,
        double duration) const;

    /**
     * Computes instantaneous power consumption
     * @param hwType Hardware type index
     * @param cpuUtil Current CPU utilization [0, 1]
     * @param memUtil Current memory utilization [0, 1]
     * @param accUtil Current accelerator utilization [0, 1]
     * @param numAccelerators Number of accelerators
     * @return Power in Watts
     */
    double computeInstantaneousPower(
        int hwType,
        double cpuUtil,
        double memUtil,
        double accUtil,
        int numAccelerators) const;
};

#endif // ENERGYTRACKER_H