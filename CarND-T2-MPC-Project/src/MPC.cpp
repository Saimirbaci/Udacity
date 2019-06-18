#include "MPC.h"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include "Eigen-3.3/Eigen/Core"

#include "FG_eval.h"

// TODO: Set the timestep length and duration

// This value assumes the model presented in the classroom is used.
//
// It was obtained by measuring the radius formed by running the vehicle in the
// simulator around in a circle with a constant steering angle and velocity on a
// flat terrain.
//
// Lf was tuned until the the radius formed by the simulating the model
// presented in the classroom matched the previous radius.
//
// This is the length from front to CoG that has a similar radius.


void MPC::UpdateNumberOfPoints(int number_of_points){
      number_of_points_ = number_of_points;
      x_index_ = 0;
      y_index_ = x_index_ + number_of_points_;
      psi_index_= y_index_ + number_of_points_;
      speed_index_= psi_index_ + number_of_points_;
      cte_index_= speed_index_ + number_of_points_;
      psi_error_index_= cte_index_ + number_of_points_;
      delta_index_= psi_error_index_ + number_of_points_;
      a_index_= delta_index_ + number_of_points_ - 1;
  }

double MPC::Solve() {
  bool ok = true;

typedef CPPAD_TESTVECTOR(double) Dvector;

  // TODO: Set the number of model variables (includes both states and inputs).
  // For example: If the state is a 4 element vector, the actuators is a 2
  // element vector and there are 10 timesteps. The number of variables is:
  //

  size_t n_vars = number_of_points_ * 6 + (number_of_points_ - 1) * 2;
  // TODO: Set the number of constraints
  size_t n_constraints = number_of_points_ * 6;

  // Initial value of the independent variables.
  // SHOULD BE 0 besides initial state.
  Dvector vars(n_vars);
  for (unsigned int i = 0; i < n_vars; i++) {
    vars[i] = 0;
  }

  Dvector vars_lowerbound(n_vars);
  Dvector vars_upperbound(n_vars);
  // TODO: Set lower and upper limits for variables.

  // Set the initial variable values
  vars[x_index_]    = vehicle_.GetXpos();
  vars[y_index_]    = vehicle_.GetYpos();
  vars[psi_index_]  = vehicle_.GetPsi();
  vars[speed_index_] = vehicle_.GetSpeed();
  vars[cte_index_]  = vehicle_.GetCTE();
  vars[psi_error_index_] = vehicle_.GetPsiE();

  // Set all non-actuators upper and lowerlimits
  // to the max negative and positive values.
  for (unsigned int i = 0; i < delta_index_; i++) {
    vars_lowerbound[i] = -1.0e19;
    vars_upperbound[i] = 1.0e19;
  }

  // The upper and lower limits of delta are set to -25 and 25
  // degrees (values in radians).
  // NOTE: Feel free to change this to something else.
  for (unsigned int i = delta_index_; i < a_index_; i++) {
    vars_lowerbound[i] = -0.436332;
    vars_upperbound[i] = 0.436332;
  }

  // Acceleration/decceleration upper and lower limits.
  // NOTE: Feel free to change this to something else.
  for (unsigned int i = a_index_; i < n_vars; i++) {
    vars_lowerbound[i] = -1.0;
    vars_upperbound[i] = 1.0;
  }

  // Lower and upper limits for the constraints
  // Should be 0 besides initial state.
  Dvector constraints_lowerbound(n_constraints);
  Dvector constraints_upperbound(n_constraints);
  for (unsigned int i = 0; i < n_constraints; i++) {
    constraints_lowerbound[i] = 0;
    constraints_upperbound[i] = 0;
  }

  constraints_lowerbound[x_index_] = vehicle_.GetXpos();
  constraints_lowerbound[y_index_] = vehicle_.GetYpos();
  constraints_lowerbound[psi_index_] = vehicle_.GetPsi();
  constraints_lowerbound[speed_index_] = vehicle_.GetSpeed();
  constraints_lowerbound[cte_index_] = vehicle_.GetCTE();
  constraints_lowerbound[psi_error_index_] = vehicle_.GetPsiE();

  constraints_upperbound[x_index_] = vehicle_.GetXpos();
  constraints_upperbound[y_index_] = vehicle_.GetYpos();;
  constraints_upperbound[psi_index_] = vehicle_.GetPsi();
  constraints_upperbound[speed_index_] = vehicle_.GetSpeed();
  constraints_upperbound[cte_index_] = vehicle_.GetCTE();
  constraints_upperbound[psi_error_index_] = vehicle_.GetPsiE();

  auto coeffs = vehicle_.GetCoeffs();

  // object that computes objective and constraints
  FG_eval fg_eval(coeffs, number_of_points_, target_v_, vehicle_.GetLf(), Dt_,  
        hp_cte_, hp_epsi_, hp_delta_, hp_acc_, hp_delta_acc_, hp_delta_seq_, hp_acc_seq_);
  
  //
  // NOTE: You don't have to worry about these options
  //
  // options for IPOPT solver
  std::string options;
  // Uncomment this if you'd like more print information
  options += "Integer print_level  0\n";
  // NOTE: Setting sparse to true allows the solver to take advantage
  // of sparse routines, this makes the computation MUCH FASTER. If you
  // can uncomment 1 of these and see if it makes a difference or not but
  // if you uncomment both the computation time should go up in orders of
  // magnitude.
  options += "Sparse  true        forward\n";
  //options += "Sparse  true        reverse\n";
  // NOTE: Currently the solver has a maximum time limit of 0.5 seconds.
  // Change this as you see fit.
  options += "Numeric max_cpu_time          0.5\n";

  // place to return solution
  CppAD::ipopt::solve_result<Dvector> solution;

  // solve the problem

  CppAD::ipopt::solve<Dvector, FG_eval>(
      options, vars, vars_lowerbound, vars_upperbound, constraints_lowerbound,
      constraints_upperbound, fg_eval, solution);
  
  // Check some of the solution values
  ok &= solution.status == CppAD::ipopt::solve_result<Dvector>::success;

  // Cost
  auto cost = solution.obj_value;

  vehicle_.SetThrottle(solution.x[a_index_]);
  vehicle_.SetSteering(solution.x[delta_index_]);

  //Setting the predicted points
  for (unsigned int i = 0; i < number_of_points_ - 1; i++) {
      vehicle_.SetPredictedPoints(i, solution.x[x_index_ + i + 1], solution.x[y_index_ + i + 1]);
  }

  return cost;
}
