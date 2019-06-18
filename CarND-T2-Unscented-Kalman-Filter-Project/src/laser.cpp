#include "laser.h"
#include <iostream>
using namespace std;


void Laser::Update(const Eigen::VectorXd z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(x_.size(), x_.size());
	Eigen::VectorXd z_pred = H_ * x_;
	Eigen::VectorXd y = z - z_pred;
	Eigen::MatrixXd Ht = H_.transpose();
	Eigen::MatrixXd S = H_ * P_ * Ht + R_laser_;
	Eigen::MatrixXd Si = S.inverse();
	Eigen::MatrixXd PHt = P_ * Ht;
	Eigen::MatrixXd K = PHt * Si;
	x_ = x_ + (K * y);
	P_ = (I - K * H_) * P_;
}