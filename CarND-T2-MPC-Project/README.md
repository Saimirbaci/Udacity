# CarND-Controls-MPC
Self-Driving Car Engineer Nanodegree Program

---

## Model

The Vehicle state is reprenseted by a class named [Vehicle](src/Vehicle.h). The class members contain information related to the position, orientation, cross track error, steering angle, accelaration and the predicted trajectory from the [MPC](src/MPC.h) class. 

The MPC class uses the information provided by [Vehicle](src/Vehicle.h) to get hold of the current state, build the constraints based on it and solve the problem. 

The equations used to update the state of the vehicle, follow the generic kinematic model, and they are build in the class [FG_eval](src/FG_eval.h)


## Timestep length and elapsed duration (N & Dt)

I designed the model so that the N (number of points) and Dt (elapsed duration) used for solving the equations in the model is changed based on the cost of the previous prediction, a lower cost leads to better path predictions, which means that the vehicle state is more precise, this can allow us to reduce the number of points to used for prediction in the next step, and increase the time horizon for the prediction. What I observed following this strategy is that when driving on straight paths not many points are needed for the computations, and the time horizon can be bigger, this leads to faster computations since a smaller number of points is used for solving the equations. The trajectory predicted when using less points was less accurate, but still fairly acceptable when on a straight path.  

Regarding the Dt (elapsed duration), similar reasoning applies. A large Dt leads to incorrect prediction of the trajectory and in an inefficient control rate. A small Dt leads to shorter time horizon, which as expected makes it harder for a correct prediction of the trajectory for future states. 

## Polynomial fitting and Mpc preprocessing

The received waypoints are transformed to the Vehicle coordinate system during the preprocessing step at [line](src/Vehicle.cpp#L25). I set the vehicle as center of the coordinate system, this leads to x,y state variables to be set to zero. A polynom is fitted to the preprocessed points, and the computed coefficients are used to calculate the cross track error (CTE) and the orientation error (PsiE). 

## Model predictive control with latency

The next step in the preprocessing is to compute the initial state taking into account the latency and the actuation values of the previous step. For this I used 100 ms as latency.c


## Demo 

This [video](videos/video.mp4) shows the predicted trajectory, using different Ns and Dts selected based on the previous cost.

In the [images](images) folder, I included snapshots of the predicted trajectory from different parts of the simulator track. Below are presented snapshots from the video in different parts of the truck.

![](images/img_0000220.jpg?raw=true)

![](images/img_0000275.jpg?raw=true)

![](images/img_0000654.jpg?raw=true)

![](images/img_0000693.jpg?raw=true)

![](images/img_0000853.jpg?raw=true)

![](images/img_0001021.jpg?raw=true)

![](images/img_0001111.jpg?raw=true)

![](images/img_0001206.jpg?raw=true)

![](images/img_0001303.jpg?raw=true)

![](images/img_0001530.jpg?raw=true)

![](images/img_0001531.jpg?raw=true)

![](images/img_0001647.jpg?raw=true)

![](images/img_0001819.jpg?raw=true)
