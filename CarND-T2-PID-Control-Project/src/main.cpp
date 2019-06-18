#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include <vector>
#include <time.h>

using namespace std;
// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

double sum(vector<double> v){
    double s = 0.0;
    for (unsigned int i=0; i<v.size(); i++){
        s += v[i];
    }
    return s;
}

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  struct timespec tp;
  clock_gettime(CLOCK_REALTIME, &tp);
  unsigned int start_timestamp = tp.tv_sec;

  uWS::Hub h;

  PID steering_pid;
  PID speed_pid;
  // TODO: Initialize the pid variable.
  
  steering_pid.Init(start_timestamp, 14.02, 45.32, 0.49);

  unsigned int range = 1;

  speed_pid.Init(start_timestamp, 0.9, 0.3, 0);
  double throttle = 0.3;
  double target_speed = 25;
  double d_target_speed = 5; // Set to 0 for constant speed
  double change_speed_time_interval = 15;
  h.onMessage([&change_speed_time_interval, &d_target_speed, &target_speed, &range, &start_timestamp, &speed_pid, &throttle, &steering_pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          //double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          //double current_throttle = std::stod(j[1]["steering_angle"].get<std::string>());
          double req_throttle;
          double steer_value;
          double cse = (speed - target_speed);
          
          //Get timestamp. Using same timestamp for both PID controllers
          
          struct timespec current_tp;
          clock_gettime(CLOCK_REALTIME, &current_tp);
          //unsigned int timestamp = current_tp.tv_nsec;
          
          double time_now = (double) (current_tp.tv_sec-start_timestamp) + (double) (current_tp.tv_nsec)/1000000000;
          
          steering_pid.NewMeasurement(time_now);
          speed_pid.NewMeasurement( time_now);

          double time_from_start = (current_tp.tv_sec - start_timestamp);

          if(time_from_start > range * change_speed_time_interval){
              target_speed += d_target_speed;
              if (target_speed > 60){
                target_speed = 25;
              }
              cout << "Changing target speed: " << target_speed << " >> " <<  target_speed << endl;
              range++;
          }

          
          //std::cout << j[1] << std::endl;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          speed_pid.UpdateError(cse);      
          req_throttle = speed_pid.TotalError();
          if (req_throttle > 1){
              req_throttle = 1;
          }
          else if(req_throttle < 0){
              req_throttle = 0;
          }

          throttle = req_throttle;

          steering_pid.UpdateError(cte);
          steer_value = deg2rad(steering_pid.SteeringTotalError(speed));  
          
          //Smoothen the steer value.
          if ((speed > 20) && (speed < 50)){
              steer_value = (steer_value * (8/speed));
          }
          if ((speed > 50) && (speed < 65)){
              steer_value = (steer_value * (6/speed));
          }
          if ((speed > 65) && (speed < 80)){
              steer_value = (steer_value * (5.2 / speed));
          }

          // Saturate to max possible steering value.
          if (steer_value > 1) {
            steer_value = 1;
          }
          else if (steer_value < -1) {
            steer_value = -1;
          }

          //Reduce speed for better control in cases with high steering value
          if ( (speed > 25) && ((steer_value > 0.5) ||(steer_value < -0.5))){
            throttle = -0.05;
          }
          if((speed > 45) && ((steer_value > 0.25) ||(steer_value < -0.25))){
            throttle = -0.1;
          }
          if((speed > 60) && ((steer_value > 0.2) ||(steer_value < -0.2))){
            throttle = -0.2;
          }
          
          json msgJson;
          msgJson["steering_angle"] =steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
