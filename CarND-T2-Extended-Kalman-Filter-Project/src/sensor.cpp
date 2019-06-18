#include "radar.h"
#include "tools.h"
#include <iostream>
using namespace std;

void Sensor::Measurement(const MeasurementPackage &measurement_pack){
	float dt = (measurement_pack.timestamp_ - last_timestamp_) / 1000000.0; //convert to seconds
    if(last_timestamp_ != 0){
      //If negative diff than there is some sensor error, or in case of restarting the simulation
	  //Lets initialize again
        if ((dt < 0 )) {
            is_initialized_ = false;
		}
    }
    if(!is_initialized_){
        
		P_ = Eigen::MatrixXd(4, 4);
    	P_ << 1,0,0,0,
	  		0,1,0,0,
	  		0,0,1000,0,
	  		0,0,0,1000;
        //Initialise the kalman filter position.
        //When initialising in this case is not very important which measurement came first, radar or laser
        //the filter will adjust fast.
        x_[0] = measurement_pack.raw_measurements_[0]; 
        x_[1] = measurement_pack.raw_measurements_[0];      
        x_[2] = 0;
        x_[3] = 0;

        is_initialized_ = true;
		last_timestamp_ = measurement_pack.timestamp_;
        return;
    }
    
    float noise_ax = 18.0;
    float noise_ay = 9.0;
    
    last_timestamp_ = measurement_pack.timestamp_;

    F_(0,2) = dt;
	F_(1,3) = dt;
	float dt_2 = dt * dt;
	float dt_3 = dt_2 * dt;
	float dt_4 = dt_3 * dt;

	Q_ <<  dt_4/4*noise_ax, 0, dt_3/2*noise_ax, 0,
	       0, dt_4/4*noise_ay, 0, dt_3/2*noise_ay,
	       dt_3/2*noise_ax, 0, dt_2*noise_ax, 0,
	       0, dt_3/2*noise_ay, 0, dt_2*noise_ay;
	
	Predict();
    Update(measurement_pack.raw_measurements_);
}

void Sensor::Predict() {
  /**
  TODO:
    * predict the state
  */
	x_ = F_*x_;
	P_ = F_*P_*F_.transpose() + Q_;
}