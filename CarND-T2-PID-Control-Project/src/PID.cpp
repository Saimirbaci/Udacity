#include "PID.h"
#include <iostream>
#include <vector>
#include <map>
#include <math.h>

using namespace std;


PID::PID() {}

PID::~PID() {}

void PID::Init(unsigned int start_timestamp, double Kp, double Ki, double Kd) {
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
  p_error = 0;
  i_error = 0;
  d_error = 0;
  struct timespec tp;
  clock_gettime(CLOCK_REALTIME, &tp);
  previous_timestamp = (tp.tv_sec-start_timestamp) + (tp.tv_nsec/1000000000);
}

void PID::UpdateError(double error) {  
  d_error = ( error - p_error) / 0.001;
  p_error = error;
  i_error += (error * 0.001);
}

void PID::NewMeasurement(double timestamp){
  current_timestamp = timestamp;
}

double PID::SteeringTotalError(double speed){
    double v = - (Kp * (85/(1 + speed)) * p_error + Kd * d_error + Ki * speed * i_error);
    return v;
}
double PID::TotalError() {
    return  - (Kp  * p_error + Kd * d_error + Ki * i_error);
}

