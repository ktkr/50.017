#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#include "spring.h"

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"
class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	int indexOf(int row, int column);
	ClothSystem(int length);
	vector<Vector3f> evalF(vector<Vector3f> state);
	vector<Vector3f> state; //first layer indices for row. second layer: Even indices for position, Odd indices for velocity
	void draw();

	//initialize structural springs
	vector<Spring> structural;
	//initialize shear springs
	vector<Spring> shear;
	//initialize flex springs
	vector<Spring> flex;

private:
	//assume cloth is always square
	int side_length;
};


#endif
