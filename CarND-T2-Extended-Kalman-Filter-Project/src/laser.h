#ifndef LASER_H
#define LASER_H
#include "Eigen/Dense"
#include "sensor.h"

class Laser : public Sensor{
public:
    Laser(Eigen::MatrixXd &P, Eigen::VectorXd &x, bool &is_initialized, long long &last_timestamp):
    Sensor(P,x,is_initialized,last_timestamp){
        R_laser_ = Eigen::MatrixXd(2, 2);
        R_laser_ << 0.0225, 0,
                0, 0.0225;
    }

    void Update(const Eigen::VectorXd z);
private:

    Eigen::MatrixXd R_laser_;
};
#endif