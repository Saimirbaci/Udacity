#ifndef RADAR_H_
#define RADAR_H_
#include "Eigen/Dense"
#include "sensor.h"

using Eigen::VectorXd;
class Radar : public Sensor{
public:
    Radar(Eigen::MatrixXd &P, Eigen::VectorXd &x, bool &is_initialized, long long &last_timestamp, 
            Eigen::MatrixXd &Xsig_pred): 
    Sensor(P, x, is_initialized, last_timestamp, Xsig_pred){
        dim_radar_ = 3;
        n_aug_ = 7;
        lambda_ = 3 - n_aug_;

        R_radar_ = Eigen::MatrixXd(dim_radar_, dim_radar_);
        R_radar_ << 0.09, 0, 0,
        0, 0.0009, 0,
        0, 0, 0.09;

        S_radar_ = MatrixXd( dim_radar_,  dim_radar_ );
        weights_ = VectorXd( 2*n_aug_+1 );
        weights_(0) = lambda_/( lambda_ + n_aug_ );
        for( int i=1; i<2*n_aug_+1; i++ ) {
            weights_(i) = 0.5/( n_aug_ + lambda_ );
        }
    }

    void Update(const Eigen::VectorXd z);

private:
  
  unsigned int dim_radar_;
  int n_aug_;
  double lambda_;

  Eigen::MatrixXd R_radar_ ;  

  MatrixXd S_radar_;

  VectorXd weights_;
};
#endif