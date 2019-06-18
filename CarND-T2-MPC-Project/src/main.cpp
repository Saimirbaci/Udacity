#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "MPC.h"
#include "json.hpp"
#include "Vehicle.h"
#include "Utils.h"

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.rfind("}]");
  if (found_null != string::npos) {
    return "";
  } else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 2);
  }
  return "";
}

int main() {
  
  struct timespec tp;
  clock_gettime(CLOCK_REALTIME, &tp);
  unsigned int start_timestamp = tp.tv_sec;

  //Tuning params
  int hyperparam_cte = 500;
  int hyperparam_epsi = 500;
  int hyperparam_delta = 9;
  int hyperparam_acc = 5;
  int hyperparam_delta_acc = 350;
  int hyperparam_delta_seq = 180;
  int hyperparam_acc_seq = 7;

  uWS::Hub h;
  unsigned int N = 12;
  double Dt = 0.08;
  double Lf = 2.67;
  double target_v = 55;
  unsigned int latency = 100;

  Vehicle vehicle(N, Lf);
  MPC mpc(N, vehicle, Dt, target_v, hyperparam_cte, hyperparam_epsi, hyperparam_delta, 
          hyperparam_acc, hyperparam_delta_acc, hyperparam_delta_seq, hyperparam_acc_seq);

  h.onMessage([&latency, &start_timestamp, &N, &Dt, &mpc, &vehicle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    string sdata = string(data).substr(0, length);
    //cout << sdata << endl;
    if (sdata.size() > 2 && sdata[0] == '4' && sdata[1] == '2') {
      string s = hasData(sdata);
      
      if (s != "") {
        auto j = json::parse(s);
        string event = j[0].get<string>();
        if (event == "telemetry") {
          
          // j[1] is the data JSON object
          double px = j[1]["x"];
          double py = j[1]["y"];
          double psi = j[1]["psi"];
          double v = j[1]["speed"];

          vector<double> ptsx = j[1]["ptsx"];
          vector<double> ptsy = j[1]["ptsy"];

          double steer_value = j[1]["steering_angle"];
          double throttle_value = j[1]["throttle"];

          vehicle.UpdateState(px, py, psi, v, steer_value, throttle_value);
          vehicle.UpdateTrajectory(ptsx, ptsy);
          vehicle.CalculateInitialStateRespectToLatency(Dt);
          
          struct timespec t1;
          struct timespec t2;
      
          clock_gettime(CLOCK_REALTIME, &t1);    
          double cost = mpc.Solve();
          clock_gettime(CLOCK_REALTIME, &t2);

          double time_t1 = (double) (t1.tv_sec-start_timestamp) + (double) (t1.tv_nsec)/1000000000;
          double time_t2 = (double) (t2.tv_sec-start_timestamp) + (double) (t2.tv_nsec)/1000000000;

          std::cout <<"Number of points and duration (N|Dt): " << N << " | " << Dt << "| Time to solve: " << time_t2 - time_t1 << " s| Cost " << cost << std::endl;
          
          json msgJson;
          // NOTE: Remember to divide by deg2rad(25) before you send the steering value back.
          // Otherwise the values will be in between [-deg2rad(25), deg2rad(25] instead of [-1, 1].
                    
          steer_value = vehicle.GetSteering();   
          throttle_value = vehicle.GetThrottle(); 

          msgJson["steering_angle"] = steer_value/(deg2rad(25));
          msgJson["throttle"] = throttle_value;

          //Display the MPC predicted trajectory 
          vector<double> mpc_x_vals;
          vector<double> mpc_y_vals;

          for (unsigned int i = 0; i < N-1; i++) {
              double x, y; 
              vehicle.GetPredictedPoints(i, x, y);
              mpc_x_vals.push_back(x);
              mpc_y_vals.push_back(y);
          }

          msgJson["mpc_x"] = mpc_x_vals;
          msgJson["mpc_y"] = mpc_y_vals;

          //Display the reference line
          vector<double> next_x_vals;
          vector<double> next_y_vals;

          unsigned int number_of_points = 120;
          for (unsigned int i = 0; i < number_of_points; i++){
            next_x_vals.push_back(i);
            next_y_vals.push_back(polyeval(vehicle.GetCoeffs(), i));
          }

          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;

          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          
          // Experimenting to reduce the number of points used for calculation based on the previous cost
          
          if ((cost < 5000)){
              N = 6;
              Dt = 0.12;
              vehicle.UpdateNumberOfPoints(N);
              mpc.UpdateNumberOfPoints(N);
          }
          else if((cost >= 5000) &&( cost < 10000)){
              N = 9;
              Dt = 0.10;
              vehicle.UpdateNumberOfPoints(N);
              mpc.UpdateNumberOfPoints(N);
          }
          else if(cost >= 10000){
              N = 12;
              Dt = 0.08;
              vehicle.UpdateNumberOfPoints(N);
              mpc.UpdateNumberOfPoints(N);
          }
          
          // Latency
          // The purpose is to mimic real driving conditions where
          // the car does actuate the commands instantly.
          //
          // Feel free to play around with this value but should be to drive
          // around the track with 100ms latency.
          //
          // NOTE: REMEMBER TO SET THIS TO 100 MILLISECONDS BEFORE
          // SUBMITTING.
          this_thread::sleep_for(chrono::milliseconds(latency));
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the
  // program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data,
                     size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1) {
      res->end(s.data(), s.length());
    } else {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
