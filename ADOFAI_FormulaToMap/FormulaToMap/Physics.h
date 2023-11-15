#pragma once
class Physics {
private:
	double radius;
	double moment_of_inertia_per_mass;

	double BPMtoAngularVelocity(double bpm);
	double AngularVelocitytoBPM(double angular_velocity);
	double GetKineticEnergyFromAngularVelocity(double angular_velocity);
	double GetAngularVelocityFromKineticEnergy(double kinetic_energy);


public:
	Physics();
	~Physics();

	double GetBPM(double delta_height, double initial_bpm);
};

