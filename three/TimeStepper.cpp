#include "TimeStepper.hpp"
#include <iostream>
using namespace std;
///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> s = particleSystem->getState();
	vector<Vector3f> f = particleSystem->evalF(s);

	for (unsigned i = 0; i < s.size();i++) {
		s[i] += stepSize*f[i];
		
	}
	particleSystem->setState(s);
	particleSystem->draw();
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> s = particleSystem->getState();
	vector<Vector3f> f0 = particleSystem->evalF(s);
	vector<Vector3f> temp_s;
	for (unsigned i = 0; i < s.size(); i++) {
		temp_s.push_back( s[i] + stepSize * f0[i]);
	}

	vector<Vector3f> f1 = particleSystem->evalF(temp_s);

	for (unsigned i = 0; i < s.size(); i++) {
		s[i] += stepSize / 2 * (f0[i] + f1[i]);
	}

	particleSystem->setState(s);
	particleSystem->draw();

}

