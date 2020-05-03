#include "processor.h"
#include "linux_parser.h"

Processor::Processor():numberprocessors_(LinuxParser::CpuUtilization().size()){
    usertime_ = new unsigned long long int [numberprocessors_];
    nicetime_ = new unsigned long long int [numberprocessors_];
    systemtime_ = new unsigned long long int [numberprocessors_];
    idletime_ = new unsigned long long int [numberprocessors_];
    ioWait_ = new unsigned long long int [numberprocessors_];
    irq_ = new unsigned long long int [numberprocessors_];
    softIrq_ = new unsigned long long int [numberprocessors_];
    steal_ = new unsigned long long int [numberprocessors_];
    guest_ = new unsigned long long int [numberprocessors_];
    guestnice_ = new unsigned long long int [numberprocessors_];
    prev_usertime_ = new unsigned long long int [numberprocessors_];
    prev_nicetime_ = new unsigned long long int [numberprocessors_];
    prev_systemtime_ = new unsigned long long int [numberprocessors_];
    prev_idletime_ = new unsigned long long int [numberprocessors_];
    prev_ioWait_ = new unsigned long long int [numberprocessors_];
    prev_irq_ = new unsigned long long int [numberprocessors_];
    prev_softIrq_ = new unsigned long long int [numberprocessors_];
    prev_steal_ = new unsigned long long int [numberprocessors_];
    prev_guest_ = new unsigned long long int [numberprocessors_];
    prev_guestnice_ = new unsigned long long int [numberprocessors_];
}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::string cpu_info;
    
    auto cpu_stat = LinuxParser::CpuUtilization();
    //numberprocessors_ = cpu_stat.size();

    std::istringstream linestream(cpu_stat[0]);
    if (!initialized){
        init_proc = new bool[numberprocessors_];
        for(unsigned int i=0; i<numberprocessors_; i++){
            init_proc[i] = false;
        }

        linestream >> cpu_info;
        linestream >> usertime_[0];
        linestream >> nicetime_[0];
        linestream >> systemtime_[0];
        linestream >> idletime_[0] ;
        linestream >> ioWait_[0];
        linestream >> irq_[0];
        linestream >> softIrq_[0];
        linestream >> steal_[0];  
        linestream >> guest_[0];
        linestream >> guestnice_[0];
        initialized = true;
        return 0.1;
    }
    else{
        prev_usertime_[0] = usertime_[0];
        prev_nicetime_[0] = nicetime_[0];
        prev_systemtime_[0] = systemtime_[0];
        prev_idletime_[0] = idletime_[0];
        prev_ioWait_[0] = ioWait_[0];
        prev_irq_[0] = irq_[0];
        prev_softIrq_[0] = softIrq_[0];
        prev_steal_[0] = steal_[0];
        prev_guest_[0] = guest_[0];
        prev_guestnice_[0] = guestnice_[0];

        linestream >> cpu_info;
        linestream >> usertime_[0];
        linestream >> nicetime_[0];
        linestream >> systemtime_[0];
        linestream >> idletime_[0] ;
        linestream >> ioWait_[0];
        linestream >> irq_[0];
        linestream >> softIrq_[0];
        linestream >> steal_[0];  
        linestream >> guest_[0];
        linestream >> guestnice_[0];

        //Taken from
        //https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
        
        auto PrevIdle = prev_idletime_[0] + prev_ioWait_[0];
        auto Idle = idletime_[0] + ioWait_[0];
        auto PrevNonIdle = prev_usertime_[0] + prev_nicetime_[0] + prev_systemtime_[0] + prev_irq_[0] + prev_softIrq_[0] + prev_steal_[0]; 
        auto NonIdle = usertime_[0] + nicetime_[0] + systemtime_[0] + irq_[0] + prev_softIrq_[0] + steal_[0]; 
        auto PrevTotal = PrevIdle + PrevNonIdle;
        auto Total = Idle + NonIdle;
        auto total_diff = Total - PrevTotal;
        auto idle_diff = Idle - PrevIdle;
        auto CPU_Percentage = float(total_diff - idle_diff)/float(total_diff);
        return CPU_Percentage;
    }
      

    return 0.0; 
    
}

float Processor::Utilization(unsigned int i) { 
    std::string cpu_info;
    
    auto cpu_stat = LinuxParser::CpuUtilization();
    std::istringstream linestream(cpu_stat[i]);
    
    if(init_proc != nullptr){
        if (!init_proc[i]){
            linestream >> cpu_info >> usertime_[i] >> nicetime_[i] >> systemtime_[i] >> idletime_[i] >> ioWait_[i] >> irq_[i] >> softIrq_[i] >> steal_[i] >> guest_[i] >> guestnice_[i];
            init_proc[i] = true;
            return 0.0;
        }
        else{
            prev_usertime_[i] = usertime_[i];
            prev_nicetime_[i] = nicetime_[i];
            prev_systemtime_[i] = systemtime_[i];
            prev_idletime_[i] = idletime_[i];
            prev_ioWait_[i] = ioWait_[i];
            prev_irq_[i] = irq_[i];
            prev_softIrq_[i] = softIrq_[i];
            prev_steal_[i] = steal_[i];
            prev_guest_[i] = guest_[i];
            prev_guestnice_[i] = guestnice_[i];
            linestream >> cpu_info >> usertime_[i] >> nicetime_[i] >> systemtime_[i] >> idletime_[i] >> ioWait_[i] >> irq_[i] >> softIrq_[i] >> steal_[i] >> guest_[i] >> guestnice_[i];
            
            auto PrevIdle = prev_idletime_[i] + prev_ioWait_[i];
            auto Idle = idletime_[i] + ioWait_[i];
            auto PrevNonIdle = prev_usertime_[i] + prev_nicetime_[i] + prev_systemtime_[i] + prev_irq_[i] + prev_softIrq_[i] + prev_steal_[i]; 
            auto NonIdle = usertime_[i] + nicetime_[i] + systemtime_[i] + irq_[i] + prev_softIrq_[i] + steal_[i]; 
            auto PrevTotal = PrevIdle + PrevNonIdle;
            auto Total = Idle + NonIdle;
            auto total_diff = Total - PrevTotal;
            auto idle_diff = Idle - PrevIdle;
            auto CPU_Percentage = float(total_diff - idle_diff)/float(total_diff);

            return CPU_Percentage;
        }
    }

    return 0.0; 
    
}