#ifndef PID_H
#define PID_H
#include <vector>

using namespace std;

class PID {
public:


  vector<double> p;// p(3, 0.0);
  vector<double> dp;// dp(3, 0.0);

  double current_timestamp;
  double previous_timestamp;
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;
  unsigned int count;
  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(unsigned int start_timestamp, double Kp, double Ki, double Kd);

 // void InitKpids(SPEED_CONFIG s_cfg, PATH_CONFIG p_cfg, double Kp, double Ki, double Kd);
  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */

  double TotalError();

  double SteeringTotalError(double speed);

  void UpdateParams();

  // Get time when new measurement comes
  void NewMeasurement(double timestamp);

};

#endif /* PID_H */
