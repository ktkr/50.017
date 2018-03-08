
#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	vector<Vector3f> state;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		// for this system, we care about the position and the velocity
		float r = 0.1f;
		//float r = static_cast <float> (rand()) / static_cast <float> (1);
		state.push_back(Vector3f(0.5f, 0.866f,0));
		state.push_back(Vector3f());
		//springs.push_back(Vector3f(1, 0, 0) - Vector3f());
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
	float mass = 0.1f;
	float g = 9.81f;
	float drag_coeff = 0.05f;
	float spring_const = 2.0f;
	float rest_length = 0.3f;
	Vector3f weight = Vector3f(0,-mass*g,0);
	for (unsigned i = 0; i < state.size(); i+=2) {
		
		Vector3f distance = state[i];
		Vector3f drag = Vector3f(-drag_coeff * state[i+1]); //choose the velocity, odd indices
		Vector3f spring_force = Vector3f(-spring_const * (distance.abs() - rest_length)* distance / distance.abs());//self minus rest length, take other point as 0,0,0
		Vector3f total_force = weight + drag + spring_force;
		f.push_back(state[i+1]);
		f.push_back(total_force/mass);
	}
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i+=2) {
		Vector3f pos = getState()[i];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
