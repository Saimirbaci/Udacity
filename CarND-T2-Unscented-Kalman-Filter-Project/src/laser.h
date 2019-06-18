#ifndef LASER_H
#define LASER_H
#include "Eigen/Dense"
#include "sensor.h"

class Laser : public Sensor{
public:
    Laser(Eigen::MatrixXd &P, Eigen::VectorXd &x, bool &is_initialized, long long &last_timestamp, 
            Eigen::MatrixXd &Xsig_pred):
    Sensor(P,x,is_initialized,last_timestamp, Xsig_pred){
        R_laser_ = Eigen::MatrixXd(2, 2);
        R_laser_ << 0.0225, 0,
                0, 0.0225;
         
        H_ = Eigen::MatrixXd(2, 5);
        H_ << 1,0,0,0,0,
	          0,1,0,0,0;
    }

    void Update(const Eigen::VectorXd z);
private:

  Eigen::MatrixXd R_laser_;
  Eigen::MatrixXd H_;

};
#endif