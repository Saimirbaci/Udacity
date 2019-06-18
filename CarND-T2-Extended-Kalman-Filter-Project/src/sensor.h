#ifndef SENSOR_H_
#define SENSOR_H_
#include "Eigen/Dense"
#include "measurement_package.h"
class Sensor{
public:
    Sensor(Eigen::MatrixXd &P, Eigen::VectorXd &x, bool &is_initialized, long long &last_timestamp): 
    P_(P),
    x_(x), 
    is_initialized_(is_initialized), 
    last_timestamp_(last_timestamp) {
    F_ = Eigen::MatrixXd(4, 4);
    F_  << 1, 0, 1, 0,
          0, 1, 0, 1,
          0, 0, 1, 0,
          0, 0, 0, 1;

    Q_ = Eigen::MatrixXd(4, 4);
    Q_ << 0,0,0,0,
	   0,0,0,0,
	   0,0,0,0,
	   0,0,0,0;

    H_ = Eigen::MatrixXd(2, 4);
    H_ << 1,0,0,0,
	    0,1,0,0;

    }

    virtual void Update(const Eigen::VectorXd z)=0;
    void Predict();
    void Measurement(const MeasurementPackage &measurement_pack);

protected:
    Eigen::MatrixXd &P_;
    Eigen::VectorXd &x_;
    bool &is_initialized_;
    long long &last_timestamp_;

    Eigen::MatrixXd F_;
    Eigen::MatrixXd H_;
    Eigen::MatrixXd Q_;
};

#endif