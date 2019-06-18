#ifndef SENSOR_H_
#define SENSOR_H_
#include "Eigen/Dense"
#include "measurement_package.h"
#include <iostream>

using namespace Eigen;
using namespace std;

class Sensor{
public:
    Sensor(Eigen::MatrixXd &P, Eigen::VectorXd &x, bool &is_initialized, long long &last_timestamp, 
            Eigen::MatrixXd &Xsig_pred): 
    P_(P),
    x_(x), 
    is_initialized_(is_initialized), 
    last_timestamp_(last_timestamp), 
    Xsig_pred_(Xsig_pred) {
        dim_x_ = x_.size();
    }
    virtual void Update(const Eigen::VectorXd z)=0;
    void Predict();
    void Measurement(const MeasurementPackage &measurement_pack);

protected:
    Eigen::MatrixXd &P_;
    Eigen::VectorXd &x_;
    bool &is_initialized_;
    long long &last_timestamp_;
    MatrixXd& Xsig_pred_;
    VectorXd z_pred_;
    unsigned int dim_x_;
};

#endif