/* Enhanced Decision Logger for Energy-Aware Cloud Simulator
 * Logs task allocation decisions with full system state and energy metrics
 * Replaces the original decisionLog.h
 */

#ifndef DECISIONLOG_ENHANCED_H
#define DECISIONLOG_ENHANCED_H

#include <fstream>
#include <ios>

/**
 * Logs a task allocation decision with comprehensive state information
 *
 * CSV Schema:
 * timestamp,task_id,num_vms,cpu_req,mem_req,chosen_cell,chosen_hw_type,accepted,
 * util_cpu_before,util_mem_before,avail_cpu_before,avail_mem_before,
 * avail_storage_before,avail_accelerators_before,energy_kwh,processing_time_sec
 *
 * @param filename Output CSV file path
 * @param timestamp Simulation timestamp
 * @param task_id Unique task identifier
 * @param num_vms Number of VMs requested
 * @param cpu_req CPU cores per VM
 * @param mem_req Memory (GB) per VM
 * @param chosen_cell Cell ID where task was allocated
 * @param chosen_hw_type Hardware type selected (1-4)
 * @param accepted True if task was accepted, false if rejected
 * @param util_cpu_before CPU utilization before allocation [0, 1]
 * @param util_mem_before Memory utilization before allocation [0, 1]
 * @param avail_cpu_before Available CPU cores before allocation
 * @param avail_mem_before Available memory (GB) before allocation
 * @param avail_storage_before Available storage (TB) before allocation
 * @param avail_accelerators_before Available accelerators before allocation
 * @param energy_kwh Actual energy consumed by task (kWh)
 * @param processing_time_sec Actual task execution time (seconds)
 */
inline void log_decision_enhanced(
    const char *filename,
    double timestamp,
    unsigned long task_id,
    int num_vms,
    double cpu_req,
    double mem_req,
    int chosen_cell,
    int chosen_hw_type,
    bool accepted,
    double util_cpu_before,
    double util_mem_before,
    double avail_cpu_before,
    double avail_mem_before,
    double avail_storage_before,
    double avail_accelerators_before,
    double energy_kwh,
    double processing_time_sec)
{
    std::ofstream file(filename, std::ios::app);

    // Write header if file is empty
    file.seekp(0, std::ios::end);
    if (file.tellp() == 0)
    {
        file << "timestamp,task_id,num_vms,cpu_req,mem_req,chosen_cell,chosen_hw_type,accepted,"
             << "util_cpu_before,util_mem_before,avail_cpu_before,avail_mem_before,"
             << "avail_storage_before,avail_accelerators_before,energy_kwh,processing_time_sec\\n";
    }

    // Write decision record
    file << timestamp << ","
         << task_id << ","
         << num_vms << ","
         << cpu_req << ","
         << mem_req << ","
         << chosen_cell << ","
         << chosen_hw_type << ","
         << (accepted ? 1 : 0) << ","
         << util_cpu_before << ","
         << util_mem_before << ","
         << avail_cpu_before << ","
         << avail_mem_before << ","
         << avail_storage_before << ","
         << avail_accelerators_before << ","
         << energy_kwh << ","
         << processing_time_sec << "\\n";

    file.close();
}

#endif // DECISIONLOG_ENHANCED_H