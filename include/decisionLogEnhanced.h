/* Enhanced Decision Logger for Energy-Aware Cloud Simulator
 * Logs task allocation decisions with full system state and energy metrics
 * Supports per-cell distributed logging for MPI parallel execution
 */

#ifndef DECISIONLOG_ENHANCED_H
#define DECISIONLOG_ENHANCED_H

#include <fstream>
#include <ios>
#include <string>
#include <sstream>
#include <iomanip>
#include <mutex>

class DecisionLogger
{
private:
    std::string baseDir;
    int cellId;
    std::ofstream logFile;
    std::mutex writeMutex;
    bool headerWritten;

public:
    DecisionLogger() : cellId(-1), headerWritten(false) {}

    void init(const std::string &outputDir, int _cellId)
    {
        baseDir = outputDir;
        cellId = _cellId;

        std::ostringstream filename;
        filename << baseDir << "/cell_" << cellId << "_decisions.csv";

        logFile.open(filename.str(), std::ios::out | std::ios::trunc);
        if (logFile.is_open())
        {
            logFile << "num_vms,cpu_req,mem_req,storage_req,network_req,"
                       "acc_req,rho_acc,requested_instructions,"
                       "util_cpu_before,util_mem_before,"
                       "avail_cpu_before,avail_mem_before,avail_storage_before,"
                       "avail_accelerators_before,total_cpu,total_mem,total_storage,"
                       "total_accelerators,avail_network,total_network,util_network,"
                       "cpu_idle_power,cpu_max_power,acc_idle_power,acc_max_power,"
                       "compute_cap_per_cpu,compute_cap_acc,"
                       "running_vms,overcommit_cpu,overcommit_mem,num_tasks,"
                       "energy_kwh,chosen_hw_type,accepted" << std::endl;
            headerWritten = true;
        }
    }

    void log(int num_vms,
             double cpu_req,
             double mem_req,
             double storage_req,
             double network_req,
             int acc_req,
             double rho_acc,
             double requested_instructions,
             int chosen_hw_type,
             bool accepted,
             double util_cpu_before,
             double util_mem_before,
             double avail_cpu_before,
             double avail_mem_before,
             double avail_storage_before,
             double avail_accelerators_before,
             double total_cpu,
             double total_mem,
             double total_storage,
             double total_accelerators,
             double avail_network,
             double total_network,
             double util_network,
             double cpu_idle_power,
             double cpu_max_power,
             double acc_idle_power,
             double acc_max_power,
             double compute_cap_per_cpu,
             double compute_cap_acc,
             int running_vms,
             double overcommit_cpu,
             double overcommit_mem,
             int num_tasks,
             double energy_kwh)
    {
        if (!logFile.is_open())
            return;

        std::lock_guard<std::mutex> lock(writeMutex);

        logFile << std::fixed << std::setprecision(6)
                << num_vms << ","
                << cpu_req << ","
                << mem_req << ","
                << storage_req << ","
                << network_req << ","
                << acc_req << ","
                << rho_acc << ","
                << requested_instructions << ","
                << util_cpu_before << ","
                << util_mem_before << ","
                << avail_cpu_before << ","
                << avail_mem_before << ","
                << avail_storage_before << ","
                << avail_accelerators_before << ","
                << total_cpu << ","
                << total_mem << ","
                << total_storage << ","
                << total_accelerators << ","
                << avail_network << ","
                << total_network << ","
                << util_network << ","
                << cpu_idle_power << ","
                << cpu_max_power << ","
                << acc_idle_power << ","
                << acc_max_power << ","
                << compute_cap_per_cpu << ","
                << compute_cap_acc << ","
                << running_vms << ","
                << overcommit_cpu << ","
                << overcommit_mem << ","
                << num_tasks << ","
                << energy_kwh << ","
                << chosen_hw_type << ","
                << (accepted ? 1 : 0) << std::endl;
    }

    void close()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    ~DecisionLogger()
    {
        close();
    }
};

inline double computeEstimatedEnergy(
    double cpu_req, int num_vms, double duration,
    double *Ps, double *Pis, int hwType)
{
    double avgUtil = 0.5;
    double powerPerCore = Pis[hwType] + avgUtil * (Ps[hwType] - Pis[hwType]);
    double totalPower = powerPerCore * cpu_req * num_vms;
    return (totalPower * duration) / (1000.0 * 3600.0);
}

inline void logDecisionImprovedSOSM(
    DecisionLogger &logger,
    int num_vms,
    double cpu_req,
    double mem_req,
    double storage_req,
    double network_req,
    int acc_req,
    double rho_acc,
    double requested_instructions,
    int chosen_hw_type,
    bool accepted,
    double **sPMSA,
    int type,
    double *Ps,
    double *Pis,
    double *Paccs,
    double *Piaccs,
    double *Cs,
    double *Caccs,
    double avail_network,
    double total_network,
    int running_vms,
    double overcommit_cpu,
    double overcommit_mem,
    int num_tasks,
    double estimatedDuration = 10.0)
{
    double util_cpu_before = 0.0;
    double util_mem_before = 0.0;
    double avail_cpu_before = sPMSA[type][0];
    double avail_mem_before = sPMSA[type][2];
    double avail_storage_before = sPMSA[type][4];
    double avail_accelerators_before = sPMSA[type][6];

    double total_cpu = sPMSA[type][1];
    double total_mem = sPMSA[type][3];
    double total_storage = sPMSA[type][5];
    double total_accelerators = sPMSA[type][7];

    if (sPMSA[type][1] > 0)
    {
        util_cpu_before = 1.0 - (sPMSA[type][0] / sPMSA[type][1]);
    }
    if (sPMSA[type][3] > 0)
    {
        util_mem_before = 1.0 - (sPMSA[type][2] / sPMSA[type][3]);
    }

    double util_network = 0.0;
    if (total_network > 0)
    {
        util_network = 1.0 - (avail_network / total_network);
    }

    double cpu_idle_power = Pis[type];
    double cpu_max_power = Ps[type];
    double acc_idle_power = Piaccs[type];
    double acc_max_power = Paccs[type];
    double compute_cap_per_cpu = Cs[type];
    double compute_cap_acc = Caccs[type];

    double energy_kwh = 0.0;
    if (accepted && type >= 0)
    {
        energy_kwh = computeEstimatedEnergy(
            cpu_req, num_vms, estimatedDuration, Ps, Pis, type);
    }

    logger.log(num_vms, cpu_req, mem_req, storage_req, network_req,
               acc_req, rho_acc, requested_instructions,
               chosen_hw_type, accepted,
               util_cpu_before, util_mem_before, avail_cpu_before,
               avail_mem_before, avail_storage_before, avail_accelerators_before,
               total_cpu, total_mem, total_storage, total_accelerators,
               avail_network, total_network, util_network,
               cpu_idle_power, cpu_max_power, acc_idle_power, acc_max_power,
               compute_cap_per_cpu, compute_cap_acc,
               running_vms, overcommit_cpu, overcommit_mem, num_tasks,
               energy_kwh);
}

#endif // DECISIONLOG_ENHANCED_H