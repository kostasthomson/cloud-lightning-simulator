inline void log_decision(const char *filename, double timestamp, long long task_id,
                         int num_vms, int cpu_req, double mem_req,
                         int chosen_hw_type, bool accepted, double energy_cost)
{

    std::ofstream file(filename, std::ios::app);

    // Always check file size to determine if header is needed
    file.seekp(0, std::ios::end);
    if (file.tellp() == 0)
    {
        file << "timestamp,task_id,num_vms,cpu_req,mem_req,chosen_hw_type,accepted,energy_cost\n";
    }

    file << timestamp << ","
         << task_id << ","
         << num_vms << ","
         << cpu_req << ","
         << mem_req << ","
         << chosen_hw_type << ","
         << (accepted ? 1 : 0) << ","
         << energy_cost << "\n";

    file.close();
}
