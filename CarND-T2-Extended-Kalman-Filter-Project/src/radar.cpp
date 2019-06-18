#include "radar.h"
#include "tools.h"
#include <iostream>
using namespace std;

void Radar::Update(const VectorXd z){
    
    MatrixXd I = MatrixXd::Identity(x_.size(), x_.size());
	Tools t;

	Eigen::MatrixXd Hj = t.CalculateJacobian(x_);

	double rho = sqrt(x_[0]*x_[0] + x_[1]*x_[1]);
    double theta = atan2(x_[1], x_[0]);
    double rho_dot = 0;

    if(rho < 0.001)
    	rho = 0.001;

    rho_dot = (x_[0]*x_[2] + x_[1]*x_[3]) / rho;

    VectorXd Hx = VectorXd(3);
	Hx << rho, theta, rho_dot;
	VectorXd y = z - Hx;
	
	y(1) = fmod(y(1), M_PI);
	
	MatrixXd Hjt = Hj.transpose();
	MatrixXd P_Hjt = P_ * Hjt;
	MatrixXd S = Hj * P_Hjt + R_radar_;
	MatrixXd Si = S.inverse();
	MatrixXd K = P_Hjt * Si;

	x_ = x_ + (K * y);
	P_ = (I - K * Hj) * P_;
}