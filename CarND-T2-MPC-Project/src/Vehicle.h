#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"

using namespace std;


class Vehicle {
public:
  
    Vehicle(unsigned int N, double lf);
    virtual ~Vehicle();

    void UpdateNumberOfPoints(unsigned int N){
        trajectory_ = Eigen::MatrixXd::Zero(N,2);
    }
    
    void UpdateState(double px, double py, double psi, double v, double steer_value, double throttle_value);
    void UpdateTrajectory(vector<double> ptsx, vector<double> ptsy);
    void CalculateInitialStateRespectToLatency(double latency);

    void SetPredictedPoints(int index, double pred_x, double pred_y);
    void GetPredictedPoints(int index, double &pred_x, double &pred_y);

    double GetXpos(){return x_pos_;}
    double GetYpos(){return y_pos_;}
    double GetPsi(){return psi_;}
    double GetSpeed(){return speed_;}
    double GetCTE(){return cte_;}
    double GetPsiE(){return psi_error_;}
    Eigen::VectorXd GetCoeffs(){return coeffs_;}
    double& GetLf(){return lf_;}

    double GetThrottle(){return throttle_;}
    double GetSteering(){return steering_;}

    void SetThrottle(double throttle){throttle_ = throttle;}
    void SetSteering(double steering){steering_ = steering;}



private:
    double x_pos_; 
    double y_pos_; 
    double psi_; 
    double speed_;
    double cte_;  
    double psi_error_;

    Eigen::VectorXd coeffs_;

    double steering_;
    double throttle_;

    Eigen::MatrixXd trajectory_;

    double lf_;
};

#endif /* VEHICLE_H */
