/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 *  
 *  Modified: March 7 
 *         Saimir Baci
 */


#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>
#include <map>
#include "particle_filter.h"

using namespace std;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
	
	random_generator.seed(117);

	normal_distribution<double> dist_x(x, std[0]);
	normal_distribution<double> dist_y(y, std[1]);
	normal_distribution<double> dist_theta(theta, std[2]);

	for (int i = 0; i < num_particles; ++i) {
		
		 particles[i].id = i;
		 particles[i].x = dist_x(random_generator);
		 particles[i].y  = dist_y(random_generator);
		 particles[i].theta = dist_theta(random_generator);
		 particles[i].weight = 1.0;
	}
	is_initialized = true;	
}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {

	double v_yaw_rate = (velocity/yaw_rate);
	double dv = velocity * delta_t;
	
	random_generator.seed(331);
	
	normal_distribution<double> dist_x(0.0, std_pos[0]);
	normal_distribution<double> dist_y(0.0, std_pos[1]);
	normal_distribution<double> dist_theta(0.0, std_pos[2]);
	
	for (int i = 0; i < num_particles; ++i) {
		
		if (yaw_rate != 0){
			particles[i].x += v_yaw_rate * (sin(particles[i].theta + yaw_rate*delta_t) - sin(particles[i].theta));
			particles[i].y += v_yaw_rate * (cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*delta_t));
			particles[i].theta += yaw_rate*delta_t;
		}
		else{
			particles[i].x += dv * (cos(particles[i].theta));
			particles[i].y += dv * (sin(particles[i].theta));
		}
		
		particles[i].x += dist_x(random_generator);
		particles[i].y += dist_y(random_generator);
		particles[i].theta += dist_theta(random_generator);	
	}
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		const std::vector<LandmarkObs> &observations, const Map &map_landmarks) {

	double theta;	
	double sigma_x = std_landmark[0];
	double sigma_y = std_landmark[1];

	// calculate normalization term
	double gaussian = (1/(2 * M_PI * sigma_x * sigma_y));
	double two_sigma_square_x = 2 * sigma_x * sigma_x;
	double two_sigma_square_y = 2 * sigma_y * sigma_y;

	std::map<int, std::vector<LandmarkObs>> projected_observations;
	
	for (int i = 0; i < num_particles; ++i) {
		std::vector<int> landmarks;
		
		particles[i].sense_x.clear();
		particles[i].sense_y.clear();
		particles[i].associations.clear();

		weights[i] = 1.0f;
		theta = particles[i].theta; 

		//Get the index of the landmarks that are in sensor range
		for (int j=0; j < map_landmarks.size(); j++){
			if (dist(map_landmarks[j].x_f, map_landmarks[j].y_f, particles[i].x, particles[i].y) < sensor_range){
				landmarks.push_back(j);
			}
		}
		
		for (int j=0; j<observations.size(); j++){
			double curr_x, curr_y, curr_id, median_x, median_y, gaussian_exponent;
			int landmark_id;

			// transform coordinates
			curr_x = particles[i].x + (cos(theta) * observations[j].x) - (sin(theta) * observations[j].y);
			curr_y = particles[i].y + (sin(theta) * observations[j].x) + (cos(theta) * observations[j].y);

			//Apply nearest neighbour
			double min_distance = numeric_limits<double>::max();
			for(auto k: landmarks){
				double map_x = map_landmarks[k].x_f;
				double map_y = map_landmarks[k].y_f;

				double dst = dist(map_x, map_y, curr_x, curr_y);
				if (min_distance > dst){
					min_distance = dst;
					landmark_id = map_landmarks[k].id_i;
					median_x = map_landmarks[k].x_f;
					median_y = map_landmarks[k].y_f;
				}
			}

			particles[i].sense_x.push_back(curr_x);
			particles[i].sense_y.push_back(curr_y);
			particles[i].associations.push_back(landmark_id);

			// calculate exponent
			gaussian_exponent = ((curr_x - median_x)*(curr_x - median_x))/(two_sigma_square_x) +
			 					((curr_y - median_y)*(curr_y - median_y))/(two_sigma_square_y);

			// calculate weight using normalization terms and exponent
			weights[i] = weights[i] * gaussian * exp(-gaussian_exponent);
		}
		particles[i].weight = weights[i];		
	}
}

void ParticleFilter::resample() {

	vector<Particle> survived_particles;
	discrete_distribution<> dist_particles(weights.begin(), weights.end());

	for (int i = 0; i < num_particles; i++) {
        int index = dist_particles(random_generator);
        survived_particles.push_back(std::move(particles[index]));
    }
	particles.clear();
	particles = survived_particles;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
