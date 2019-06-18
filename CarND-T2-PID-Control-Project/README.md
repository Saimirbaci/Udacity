# CarND-Controls-PID

Self-Driving Car Engineer Nanodegree Program

---

## Model Design

I implemented PID for controlling the steering, one PI for controlling the speed of the vehicle. Tuning was performed 
manually by looking at the cte error. 

## Steering control

For the PID steering control I used different set of coefficients based on the received cross track error and speed range. The sensed speed value is also used to smoothen the steering when driving on high speed.
## Tuning

Both PIDs and PI controller were tuned manually by changing the components P, I and D coeffiecients. The coefficients were tuned based on the CTE and the speed of the vehicle. I also use the CTE value to decrease the speed of the vehicle so that a better control can be achieved, when the vehicle is driving in high speeds.

The video shows the Vehicle control on different speeds.

# P, I & D components

## P Component 

The P(Proportional) component is the main reactive component, and uses directly the feedback error. If P is to high the system might become unstable, and if it is too low the controller will be less responsive to the error. The P component is the term that contribute most to the output change.  

## I Component

The I component helps to fine tune the controller, and it could bring the error close to zero. A low I component leads to a slower reaction, but a large I will probably lead to an unstable state. 

## D Component

The D components is used to control the reactiveness of the controller, based on the rate of change of the error. If D is high might lead to a considerable change on the output which could lead to bigger rate of change for the error, thus making the system unstable. 


## Demo Video

Demo is under [video](video/demo.mp4)

