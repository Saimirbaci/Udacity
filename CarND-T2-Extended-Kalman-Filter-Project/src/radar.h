#ifndef RADAR_H_
#define RADAR_H_
#include "Eigen/Dense"
#include "sensor.h"

using Eigen::VectorXd;
class Radar : public Sensor{
public:
    Radar(Eigen::MatrixXd &P, Eigen::VectorXd &x, bool &is_initialized, long long &last_timestamp): 
    Sensor(P,x,is_initialized,last_timestamp){
        R_radar_ = Eigen::MatrixXd(3, 3);
        R_radar_ << 0.09, 0, 0,
        0, 0.0009, 0,
        0, 0, 0.09;
    }

    void Update(const Eigen::VectorXd z);
private:

    Eigen::MatrixXd R_radar_ ;
    
};
#endif