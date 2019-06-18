#include "Vehicle.h"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "Utils.h"


Vehicle::Vehicle(unsigned int N, double lf) {
    trajectory_ = Eigen::MatrixXd::Zero(N,2);
    lf_ = lf;
}

Vehicle::~Vehicle() {}

void Vehicle::UpdateState(double px, double py, double psi, double v, double steer_value, double throttle_value){
    x_pos_ = px; 
    y_pos_ = py; 
    psi_ = psi; 
    speed_ = v;
    throttle_ = throttle_value;
    steering_ = steer_value;
  }

void Vehicle::UpdateTrajectory(vector<double> ptsx, vector<double> ptsy){
  Eigen::VectorXd x_coordinates(ptsx.size());
  Eigen::VectorXd y_coordinates(ptsy.size());

  for (unsigned int i = 0; i < ptsx.size(); i++) {
    x_coordinates[i] = (ptsx[i] - x_pos_) * cos(-psi_) - (ptsy[i] - y_pos_) * sin(-psi_);
    y_coordinates[i] = (ptsx[i] - x_pos_) * sin(-psi_) + (ptsy[i] - y_pos_) * cos(-psi_);
  }

  coeffs_ = polyfit(x_coordinates, y_coordinates, 3);
  cte_ = polyeval(coeffs_, 0);  
  psi_error_ = -atan(coeffs_[1]);

}

void Vehicle::CalculateInitialStateRespectToLatency(double latency){

  const double x0 = 0;
  const double y0 = 0;

  double psi = 0 - ( speed_ * steering_ * latency / lf_ );
  double x_pos = x0 + (speed_ * cos(psi) * latency);
  double y_pos = y0 + ( speed_ * sin(psi) * latency);
  
  double speed = speed_ + throttle_ * latency;
  double cte = cte_ + ( speed_ * sin(psi_error_) * latency );
  double psi_error = psi_error_ - ( speed * atan(coeffs_[1]) * latency / lf_ );
  
  x_pos_ = x_pos;
  y_pos_ = y_pos;
  psi_ = psi;
  speed_ = speed;
  cte_ = cte;
  psi_error_ = psi_error;
}

void Vehicle::SetPredictedPoints(int index, double pred_x, double pred_y){
    trajectory_(index, 0) = pred_x;
    trajectory_(index, 1) = pred_y; 
}

void Vehicle::GetPredictedPoints(int index, double &pred_x, double &pred_y){
    pred_x = trajectory_(index, 0);
    pred_y = trajectory_(index, 1); 
}