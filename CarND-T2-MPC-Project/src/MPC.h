#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Vehicle.h"

using namespace std;


class MPC {
 public:
  
  MPC(unsigned int &N, Vehicle &vehicle, double &Dt, double &target_v,
          int hyperparam_cte, int hyperparam_epsi, int hyperparam_delta, 
          int hyperparam_acc, int hyperparam_delta_acc, int hyperparam_delta_seq, int hyperparam_acc_seq)
          :number_of_points_(N), vehicle_(vehicle), Dt_(Dt), target_v_(target_v), 
          hp_cte_(hyperparam_cte), hp_epsi_(hyperparam_epsi),
          hp_delta_(hyperparam_delta), hp_acc_(hyperparam_acc),
          hp_delta_acc_(hyperparam_delta_acc), hp_delta_seq_(hyperparam_delta_seq),
          hp_acc_seq_(hyperparam_acc_seq)
  {
      x_index_ = 0;
      y_index_ = x_index_ + number_of_points_;
      psi_index_= y_index_ + number_of_points_;
      speed_index_= psi_index_ + number_of_points_;
      cte_index_= speed_index_ + number_of_points_;
      psi_error_index_= cte_index_ + number_of_points_;
      delta_index_= psi_error_index_ + number_of_points_;
      a_index_= delta_index_ + number_of_points_ - 1;
  };

  void UpdateNumberOfPoints(int number_of_points);
  virtual ~MPC() {};
  double Solve();

  
private:
  unsigned int &number_of_points_;
  Vehicle &vehicle_;  
  double &Dt_;
  double &target_v_;


  int &hp_cte_;
  int &hp_epsi_; 
  int &hp_delta_; 
  int &hp_acc_; 
  int &hp_delta_acc_; 
  int &hp_delta_seq_; 
  int &hp_acc_seq_;

  unsigned int x_index_;
  unsigned int y_index_;
  unsigned int psi_index_;
  unsigned int speed_index_;
  unsigned int cte_index_;
  unsigned int psi_error_index_;
  unsigned int delta_index_;
  unsigned int a_index_;
  };

#endif /* MPC_H */
