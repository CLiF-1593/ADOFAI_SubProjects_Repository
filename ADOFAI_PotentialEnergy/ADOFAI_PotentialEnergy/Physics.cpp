#include "Physics.h"
#include <cmath>
#include <iostream>
using namespace std;

#define gravitational_acceleration 9.8
#define pi 3.141592653
#define unit_distance 1.0

double Physics::GetKineticEnergyFromAngularVelocity(double angular_velocity) {
	// Kinetic Energy = Translational Motion Energy + Rotational Motion Energy
	// Kinetic Energy = 0.5 * (m*r*r + I) * omega * omega
	// Kinetic Energy Per Unit Mass = (r * r + I/m) * omega * omega

	double multiplier = (this->radius * this->radius + this->moment_of_inertia_per_mass);
	return multiplier * angular_velocity * angular_velocity * 0.5;
}

double Physics::GetAngularVelocityFromKineticEnergy(double kinetic_energy) {
	// Kinetic Energy = Translational Motion Energy + Rotational Motion Energy
	// Kinetic Energy = 0.5 * (m*r*r + I) * omega * omega
	// Kinetic Energy Per Unit Mass = 0.5 * (r * r + I/m) * omega * omega
	// Angular Velocity (omega) = sqrt((2 * KE per Mass) / (r * r + I/m))
	double denominator = this->radius * this->radius + this->moment_of_inertia_per_mass;
	return sqrt(2.0 * kinetic_energy / denominator);
}

double Physics::BPMtoAngularVelocity(double bpm) {
	// angular velocity = bpm * 2/3 * pi / 60
	return bpm * 2.0 / 3.0 * pi / 60.0;
}

double Physics::AngularVelocitytoBPM(double angular_velocity) {
	// angular velocity = bpm * 2/3 * pi / 60
	// bpm = angular velocity * 3/2 * 60 / pi
	return angular_velocity * 3.0 / 2.0 * 60.0 / pi;
}

Physics::Physics() {
	// Constant Calculating

	// r = 3 / (2 * pi) * d + d / 5
	this->radius = 3.0 * 0.5 / pi * unit_distance + unit_distance / 5.0;
	
	// 0.5 * r * r + R * R, r = d/5, R = d/sqrt(3)
	this->moment_of_inertia_per_mass = 0.5 * (unit_distance / 5.0) * (unit_distance / 5.0) + (unit_distance / sqrt(3.0) * (unit_distance / sqrt(3.0)));
}

Physics::~Physics() {
}

double Physics::GetBPM(double delta_height, double initial_bpm) {
	double initial_angular_velocity = this->BPMtoAngularVelocity(initial_bpm);
	double initial_kinetic_energy = this->GetKineticEnergyFromAngularVelocity(initial_angular_velocity);
	// Potential Energy = Mass * Gravitational Acceleration * Height
	// Potential Energy Per Unit Mass = Gravitational Acceleration * Height
	double delta_potential_energy = gravitational_acceleration * delta_height;
	
	// Delta Potential Energy + Delta Kinetic Energy = 0
	// Delta Potential Energy + Final Kinetic Energy - Initial Kinetic Energy = 0
	// Final Kinetic Energy = Initial Kinetic Energy - Delta Potential Energy
	double final_kinetic_energy = initial_kinetic_energy - delta_potential_energy;
	double final_angular_velocity = this->GetAngularVelocityFromKineticEnergy(final_kinetic_energy);

	return this->AngularVelocitytoBPM(final_angular_velocity);
}
