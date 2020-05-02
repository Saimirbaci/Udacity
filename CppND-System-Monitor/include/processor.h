#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  float Utilization(unsigned int proc_id);
  unsigned int NumberProcessors(){return numberprocessors_;};

  ~Processor(){
      delete init_proc;
      delete usertime_;
      delete guest_;
      delete nicetime_;
      delete guestnice_;
      delete idlealltime_;
      delete idletime_;
      delete ioWait_;
      delete systemalltime_;
      delete systemtime_;
      delete irq_;
      delete softIrq_;
      delete virtalltime_;
      delete totaltime_;
      delete steal_;
      delete prev_usertime_;
      delete prev_guest_;
      delete prev_nicetime_;
      delete prev_guestnice_;
      delete prev_idlealltime_;
      delete prev_idletime_;
      delete prev_ioWait_;
      delete prev_systemalltime_;
      delete prev_systemtime_;
      delete prev_irq_;
      delete prev_softIrq_;
      delete prev_virtalltime_;
      delete prev_totaltime_;
      delete prev_steal_;      
  }
  // TODO: Declare any necessary private members
 private:
    unsigned int numberprocessors_ = 1;
    bool initialized = false;
    bool *init_proc = nullptr ;
    unsigned long long int *usertime_;
    unsigned long long int *guest_;
    unsigned long long int *nicetime_;
    unsigned long long int *guestnice_;
    unsigned long long int *idlealltime_;
    unsigned long long int *idletime_;
    unsigned long long int *ioWait_;
    unsigned long long int *systemalltime_;
    unsigned long long int *systemtime_;
    unsigned long long int *irq_;
    unsigned long long int *softIrq_;
    unsigned long long int *virtalltime_;
    unsigned long long int *totaltime_;
    unsigned long long int *steal_;
    unsigned long long int *prev_usertime_;
    unsigned long long int *prev_guest_;
    unsigned long long int *prev_nicetime_;
    unsigned long long int *prev_guestnice_;
    unsigned long long int *prev_idlealltime_;
    unsigned long long int *prev_idletime_;
    unsigned long long int *prev_ioWait_;
    unsigned long long int *prev_systemalltime_;
    unsigned long long int *prev_systemtime_;
    unsigned long long int *prev_irq_;
    unsigned long long int *prev_softIrq_;
    unsigned long long int *prev_virtalltime_;
    unsigned long long int *prev_totaltime_;
    unsigned long long int *prev_steal_;

};

#endif