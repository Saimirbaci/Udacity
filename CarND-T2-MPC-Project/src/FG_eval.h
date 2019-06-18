#ifndef FG_EVAL_H
#define FG_EVAL_H

#include "MPC.h"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include "Eigen-3.3/Eigen/Core"

using CppAD::AD;

class FG_eval {
 public:

  FG_eval(Eigen::VectorXd coeffs, unsigned int &N, double &target_v, double &lf, double &Dt, 
        int hyperparam_cte, int hyperparam_epsi, int hyperparam_delta, 
        int hyperparam_acc, int hyperparam_delta_acc, int hyperparam_delta_seq, int hyperparam_acc_seq
        ):number_of_points_(N), target_v_(target_v), lf_(lf), Dt(Dt), 
          hp_cte_(hyperparam_cte), hp_epsi_(hyperparam_epsi),
          hp_delta_(hyperparam_delta), hp_acc_(hyperparam_acc),
          hp_delta_acc_(hyperparam_delta_acc), hp_delta_seq_(hyperparam_delta_seq),
          hp_acc_seq_(hyperparam_acc_seq) { 
    this->coeffs = coeffs; 
    x_index_ = 0;
    y_index_ = x_index_ + number_of_points_;
    psi_index_= y_index_ + number_of_points_;
    speed_index_= psi_index_ + number_of_points_;
    cte_index_= speed_index_ + number_of_points_;
    psi_error_index_= cte_index_ + number_of_points_;
    delta_index_= psi_error_index_ + number_of_points_;
    a_index_= delta_index_ + number_of_points_ - 1;
  }

  typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
  void operator()(ADvector& fg, const ADvector& vars) {
    // `fg` a vector of the cost constraints, `vars` is a vector of variable values (state & actuators)
    // NOTE: You'll probably go back and forth between this function and
    // the Solver function below.
    // The cost is stored is the first element of `fg`.
    // Any additions to the cost should be added to `fg[0]`.
    fg[0] = 0;
    
    // Reference State Cost
    for (unsigned int i = 0; i < number_of_points_; i++) {
      fg[0] += hp_cte_ * CppAD::pow(vars[cte_index_ + i], 2);
      fg[0] += hp_epsi_ * CppAD::pow(vars[psi_error_index_ + i], 2);
      fg[0] += CppAD::pow(vars[speed_index_ + i] - target_v_, 2);
    }

    for (unsigned int i = 0; i < number_of_points_ - 1; i++) {
      fg[0] += hp_delta_ * CppAD::pow(vars[delta_index_ + i], 2);
      fg[0] += hp_acc_ * CppAD::pow(vars[a_index_ + i], 2);
       
      //Penalize the cost with the angular speed
      fg[0] += hp_delta_acc_ * CppAD::pow(vars[delta_index_ + i] * vars[speed_index_+i], 2);   
    }

    for (unsigned int i = 0; i < number_of_points_ - 2; i++) {
      fg[0] += hp_delta_seq_ * CppAD::pow(vars[delta_index_ + i + 1] - vars[delta_index_ + i], 2);
      fg[0] += hp_acc_seq_ * CppAD::pow(vars[a_index_ + i + 1] - vars[a_index_ + i], 2);
    }

    // Initial constraints
    //
    // We add 1 to each of the starting indices due to cost being located at
    // index 0 of `fg`.
    // This bumps up the position of all the other values.
    fg[1 + x_index_] = vars[x_index_];
    fg[1 + y_index_] = vars[y_index_];
    fg[1 + psi_index_] = vars[psi_index_];
    fg[1 + speed_index_] = vars[speed_index_];
    fg[1 + cte_index_] = vars[cte_index_];
    fg[1 + psi_error_index_] = vars[psi_error_index_];

    for (unsigned int t = 1; t < number_of_points_; t++) {
      AD<double> x1 = vars[x_index_ + t];
      AD<double> y1 = vars[y_index_ + t];      
      AD<double> psi1 = vars[psi_index_ + t];      
      AD<double> v1 = vars[speed_index_ + t];      
      AD<double> cte1 = vars[cte_index_ + t];      
      AD<double> epsi1 = vars[psi_error_index_ + t];

      AD<double> x0 = vars[x_index_ + t - 1];
      AD<double> y0 = vars[y_index_ + t - 1];
      AD<double> psi0 = vars[psi_index_ + t - 1];
      AD<double> v0 = vars[speed_index_ + t - 1];    
      AD<double> cte0 = vars[cte_index_ + t - 1];
      AD<double> epsi0 = vars[psi_error_index_ + t - 1];

      AD<double> a0 = vars[a_index_ + t - 1];
      AD<double> delta0 = vars[delta_index_ + t - 1];

      AD<double> f0 = coeffs[0] + coeffs[1] * x0 + coeffs[2] * CppAD::pow(x0, 2) + coeffs[3] * CppAD::pow(x0, 3);
      AD<double> psides0 = CppAD::atan(coeffs[1] + 2 * coeffs[2] * x0 + 3 * coeffs[3] * CppAD::pow(x0, 2));
      
      // Here's `x` to get you started.
      // The idea here is to constraint this value to be 0.
      //
      // NOTE: The use of `AD<double>` and use of `CppAD`!
      // This is also CppAD can compute derivatives and pass
      // these to the solver.

      // TODO: Setup the rest of the model constraints
      AD<double> speed_latency = v0 * Dt;
      
      fg[1 + x_index_ + t] = x1 - (x0 + speed_latency * CppAD::cos(psi0));
      fg[1 + y_index_ + t] = y1 - (y0 + speed_latency * CppAD::sin(psi0));
      fg[1 + cte_index_ + t] = cte1 - ((f0 - y0) + (speed_latency * CppAD::sin(epsi0)));
      fg[1 + psi_error_index_ + t] = epsi1 - ((psi0 - psides0) - speed_latency/lf_ * delta0);
      fg[1 + psi_index_ + t] = psi1 - (psi0 - speed_latency/lf_ * delta0);
      fg[1 + speed_index_ + t] = v1 - (v0 + a0 * Dt);
      
    }
  }

private:
  Eigen::VectorXd coeffs;

  unsigned int &number_of_points_;
  double &target_v_;
  double &lf_;
  double &Dt;

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

#endif