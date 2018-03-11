#include <iostream>
#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	vector<Vector3f> state;
	float x;
	float y;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		// for this system, we care about the position and the velocity
		x = (float(i) / float(m_numParticles-1) *2 - 1)*0.5f; 
		y = -sqrt(0.5 - x * x);
		
		state.push_back(Vector3f(x, y, 0));
		Vector3f(x, y, 0).print();
		//state.push_back(Vector3f(0.5f, 0.866f,0));
		//instantiate without velocity
		state.push_back(Vector3f(0.0f));
		//springs.push_back(Vector3f(1, 0, 0) - Vector3f());
	}
	
	for (unsigned i = 0; i < state.size()/2 -1; i++) {
		Spring s(2 * i, 2 * (i + 1), (state[2 * i] - state[2 * (i + 1)]).abs());
		springs.push_back(s);
	}

	setState(state);

}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	// YOUR CODE HERE
	// FIXED POINT IS AT (0,0,0)
	// MAKE SURE STEP SIZE IS REALLY SMALL
	float mass = 0.05f;
	float g = 9.81f;
	float drag_coeff = 0.05f;
	float spring_const = 30.0f;
	Vector3f weight = Vector3f(0, -mass * g, 0);
	//for each particle
	for (unsigned i = 0; i < state.size(); i += 2) {

		Vector3f distance = state[i];
		Vector3f drag = Vector3f(-drag_coeff * state[i + 1]); //choose the velocity, odd indices

		Vector3f total_force = weight + drag;
		f.push_back(state[i + 1]); //velocity
		f.push_back(total_force); //force
	}

	//for each spring
	for (unsigned i = 0; i < springs.size(); i++) {
		Vector3f distance = state[springs[i].node1] - state[springs[i].node2];
		Vector3f spring_force = Vector3f(-spring_const * (distance.abs() - springs[i].rest_length)* distance / distance.abs());
		f[springs[i].node1] += spring_force;
		f[springs[i].node2] += -spring_force; //invert the force applied (3rd law)		
	}


	for (unsigned i = 0; i < state.size(); i += 2) {
		f[i+1] = f[i+1] / mass; //force divide by mass at the end.
	}
	
	//set one point force to 0.
	f[0] = Vector3f(0);
	f[1] = Vector3f(0);

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = getState()[2*i];//  position of particle 
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
