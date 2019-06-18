#include "radar.h"
#include "tools.h"
#include <iostream>
using namespace std;


void Radar::Update(const VectorXd z){
  unsigned int dim_x = x_.size();
  unsigned int n_sigmas = 2*n_aug_ + 1;

  Eigen::VectorXd z_pred = VectorXd(dim_radar_);
  Eigen::VectorXd z_diff = VectorXd(dim_radar_);
  MatrixXd Zsig = MatrixXd( dim_radar_, n_sigmas);
  VectorXd diff_x = VectorXd( n_aug_ );
  MatrixXd Tc = MatrixXd( dim_x, dim_radar_ );
  MatrixXd K = MatrixXd( dim_x, dim_radar_ );
  
// from sigma points to measurement space
  for( int i = 0; i < n_sigmas; i++ )
  {
    double px = Xsig_pred_(0,i);
    double py = Xsig_pred_(1,i);
    double v = Xsig_pred_(2,i);
    double psi = Xsig_pred_(3,i);
	double normal = sqrt(px*px+py*py); 
	Zsig(0,i) = normal;
    Zsig(1,i) = atan2(py,px);
    Zsig(2,i) = ( px*cos(psi)*v + py*sin(psi)*v )/normal;
  }

  // mean predicted measurement
  z_pred.fill(0.);
  for( int i = 0; i < n_sigmas; i++ )
  {
    z_pred = z_pred + weights_(i)*Zsig.col(i);
  }

  z_pred(1) = fmod(z_pred(1), M_PI);

  S_radar_.fill(0.);
  for( int i = 0; i < n_sigmas; i++ )
  {
    z_diff = Zsig.col(i) - z_pred;

	z_diff(1) = fmod(z_diff(1), M_PI);

    S_radar_ = S_radar_ + weights_(i) * z_diff * z_diff.transpose();
  }

  S_radar_ = S_radar_ + R_radar_;

  // cross correlation matrix
  Tc.fill(0.);
  for( int i = 0; i < n_sigmas; i++ )
  {
      diff_x = Xsig_pred_.col(i) - x_;
      z_diff = Zsig.col(i) - z_pred;

	  diff_x(1) = fmod(diff_x(1), M_PI);

	  z_diff(1) = fmod(z_diff(1), M_PI);
      Tc = Tc + weights_(i) * diff_x * z_diff.transpose();
  }

  K = Tc * S_radar_.inverse();
  

  z_diff = z - z_pred;

  z_diff(1) = fmod(z_diff(1), M_PI);
  
  x_ = x_ + K * z_diff;
  P_ = P_ - K * S_radar_ * K.transpose();
}
