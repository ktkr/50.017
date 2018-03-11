#include "ClothSystem.h"
//TODO: Initialize here
#include <iostream>
int ClothSystem::indexOf(int row, int column) {
	return 2 * (row * side_length + column);
}

ClothSystem::ClothSystem(int length)
{
	side_length = length;
#pragma region state

	//initialize cloth grid
	for (unsigned i = 0; i < length; i++) {
		
		for (unsigned j = 0; j < length; j++) {
			state.push_back(Vector3f(j*0.2f + 0.5f,i*-0.2f + 3.0f,0)); // position
			state.push_back(Vector3f::ZERO); // velocity
		}
	}

	wind = false;
	moving = false;
	wireframe = false;

	setState(state);
#pragma endregion
#pragma region springs
	for (unsigned i = 0; i < length; i++) {
		for (unsigned j = 0; j < length; j++) { //-1 from segmenting , e.g. 8 points, 7 segments
			float rest_length;
			//generate structural springs
			if (j + 1 < length) {
				rest_length = (state[indexOf(i, j)] - state[indexOf(i, j + 1)]).abs();
				Spring s_horiz(indexOf(i, j), indexOf(i, j + 1), rest_length); //horizontal springs
				structural.push_back(s_horiz);
			}
			if (i + 1 < length) {
				rest_length = (state[indexOf(i, j)] - state[indexOf(i+1, j)]).abs();
				Spring s_vert(indexOf(i, j), indexOf(i + 1, j), rest_length);
				structural.push_back(s_vert);
			}
			//generate shear springs
			if (i + 1 < length && j + 1 < length) {
				rest_length = (state[indexOf(i, j)] - state[indexOf(i + 1, j + 1)]).abs();
				Spring s_tl_br(indexOf(i, j), indexOf(i + 1, j + 1), rest_length);//top left to bottom right
				shear.push_back(s_tl_br);
				rest_length = (state[indexOf(i + 1, j)] - state[indexOf(i, j + 1)]).abs();
				Spring s_bl_tr(indexOf(i + 1, j), indexOf(i, j + 1), rest_length);
				shear.push_back(s_bl_tr);
			}
			
			//generate flex springs
			//do checks on exceeding the index.
			if (j + 2 < length) {
				rest_length = (state[indexOf(i, j)] - state[indexOf(i, j + 2)]).abs();
				Spring s_horiz2(indexOf(i, j), indexOf(i, j + 2), rest_length);
				flex.push_back(s_horiz2);
			}
			if (i + 2 < length) {
				rest_length = (state[indexOf(i, j)] - state[indexOf(i + 2, j)]).abs();
				Spring s_vert2(indexOf(i, j), indexOf(i + 2, j), rest_length);

				flex.push_back(s_vert2);
			}
		}
	}
#pragma endregion
}

#pragma region eval
// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	float mass = 0.005f;
	float g = 9.81f;
	float drag_coeff = 0.1f;
	float spring_const = 5.0f;
	Vector3f weight = Vector3f(0, -mass * g, 0);

	//for each particle, generate the drag and weight
	for (unsigned i = 0; i < state.size() / 2; i++) {
		Vector3f drag = Vector3f(-drag_coeff * state[2*i + 1]); //choose the velocity, odd indices

		Vector3f total_force = weight + drag;
		f.push_back(state[2*i+1]); //velocity
		f.push_back(total_force); //force
	}

	//apply spring forces
	for (unsigned i = 0; i < structural.size(); i++) {
		Vector3f distance = state[structural[i].node1] - state[structural[i].node2];
		Vector3f struct_force = Vector3f(-spring_const * (distance.abs() - structural[i].rest_length)* distance / distance.abs());
		f[structural[i].node1] += struct_force; //since it's node 1 - node 2, we should add to node 1 and reverse the force to node 2
		f[structural[i].node2] += -struct_force;
	}

	for (unsigned i = 0; i < shear.size(); i++) {
		Vector3f distance = state[shear[i].node1] - state[shear[i].node2];

		Vector3f shear_force = Vector3f(-spring_const * (distance.abs() - shear[i].rest_length)* distance / distance.abs());
		f[shear[i].node1] += shear_force; //since it's node 1 - node 2, we should add to node 1 and reverse the force to node 2
		f[shear[i].node2] += -shear_force;
	}

	for (unsigned i = 0; i < flex.size(); i++) {
		Vector3f distance = state[flex[i].node1] - state[flex[i].node2];

		Vector3f flex_force = Vector3f(-spring_const * (distance.abs() - flex[i].rest_length)* distance / distance.abs());
		f[flex[i].node1] += flex_force; //since it's node 1 - node 2, we should add to node 1 and reverse the force to node 2
		f[flex[i].node2] += -flex_force;
	}

	if (wind) {
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Vector3f wind = Vector3f::ZERO + Vector3f(0,0,r*0.1);
		
		for (int i = 0; i < f.size() / 2; i++) {
			
			f[2 * i + 1] += wind;
		}
	}

	for (unsigned i = 0; i < f.size() / 2; i++) {
		f[2 * i + 1] = f[2 * i + 1] / mass;
	}

	if (!moving) {
		f[0] = Vector3f::ZERO;
		f[1] = Vector3f::ZERO;

		f[2 * side_length - 2] = Vector3f::ZERO;
		f[2 * side_length - 1] = Vector3f::ZERO;
	}
	else {
		Vector3f speed;
		if (getState()[0].z() > 3){
			forward = false;
		}
		if (getState()[0].z() < -3) {
			forward = true;
		}

		if (forward) {
			speed = Vector3f(0, 0, 3.0f);
		}
		else {
			speed = Vector3f(0, 0, -3.0f);
		}
		f[0] = speed;
		f[1] = Vector3f::ZERO;

		f[2 * side_length - 2] = speed;
		f[2 * side_length - 1] = Vector3f::ZERO;
	}
	//return velocity, force for each particle
	return f;
}
#pragma endregion

#pragma region draw
void ClothSystem::draw()
{

	//draw the wireframe
	if (wireframe) {

		for (unsigned i = 0; i < state.size() / 2; i++) {
			Vector3f pos = getState()[2 * i];//  position of particle 
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			glutSolidSphere(0.075f, 10.0f, 10.0f);
			glPopMatrix();
		}

		for (int i = 0; i < structural.size(); i++) {
			//glDrawLine whatever
			
			vector<Vector3f> s = getState();
			glPushMatrix();
			glLineWidth(0.1f);
			glBegin(GL_LINES);
			glClearColor(0, 0, 0, 1);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glVertex3f(s[structural[i].node2].x(), s[structural[i].node2].y(), s[structural[i].node2].z());
			glVertex3f(s[structural[i].node1].x(), s[structural[i].node1].y(), s[structural[i].node1].z());
			glEnd();
			glPopMatrix();
		}
	}
	//draw the cloth
	else {
		vector<Vector3f> n;
		vector<Vector3f> s = getState();
		if (side_length > 1) { //ensure that the cloth can actually be drawn
			for (int i = 0; i < side_length; i++) {
				for (int j = 0; j < side_length; j++) {
					bool hasRight = j + 1 < side_length;
					bool hasBottom = i + 1 < side_length;
					Vector3f h;
					Vector3f v;
					if (hasRight && hasBottom) {
						h = s[indexOf(i, j)] - s[indexOf(i, j + 1)];
						v = s[indexOf(i, j)] - s[indexOf(i + 1, j)];
					}
					//if at the bottom of cloth
					else if (hasRight && !hasBottom) {
						h = s[indexOf(i, j)] - s[indexOf(i, j + 1)];
						v = s[indexOf(i-1, j)] - s[indexOf(i, j)];

					}
					//if at the side of cloth
					else if (!hasRight && hasBottom) {
						h = s[indexOf(i, j-1)] - s[indexOf(i, j)];
						v = s[indexOf(i, j)] - s[indexOf(i +1, j)];
					}
					//at the bottom right corner of cloth
					else if (!hasRight && !hasBottom) {
						h = s[indexOf(i, j-1)] - s[indexOf(i, j)];
						v = s[indexOf(i-1, j)] - s[indexOf(i, j)];
					}
					n.push_back(Vector3f::cross(h, v));

				}
			}
			//create triangles
			glPushMatrix();
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < side_length; i++) {
				for (int j = 0; j < side_length; j++) {
					if (i + 1 < side_length && j + 1 < side_length) {
						//draw top right triangle
						glNormal3d(n[i*side_length + j].x(), n[i*side_length + j].y(), n[i*side_length + j].z());
						glVertex3d(s[indexOf(i, j)].x(), s[indexOf(i, j)].y(), s[indexOf(i, j)].z());

						glNormal3d(n[(i + 1)*side_length + j + 1].x(), n[(i + 1)*side_length + j + 1].y(), n[(i + 1)*side_length + j + 1].z());
						glVertex3d(s[indexOf(i + 1, j + 1)].x(), s[indexOf(i + 1, j + 1)].y(), s[indexOf(i + 1, j + 1)].z());

						glNormal3d(n[i*side_length + j+1].x(), n[i*side_length + j+1].y(), n[i*side_length + j+1].z());
						glVertex3d(s[indexOf(i, j+1)].x(), s[indexOf(i, j+1)].y(), s[indexOf(i, j+1)].z());

						
						//draw bottom left triangle
						glNormal3d(n[i*side_length + j].x(), n[i*side_length + j].y(), n[i*side_length + j].z());
						glVertex3d(s[indexOf(i, j)].x(), s[indexOf(i, j)].y(), s[indexOf(i, j)].z());

						glNormal3d(n[(i+1)*side_length + j].x(), n[(i+1)*side_length + j].y(), n[(i+1)*side_length + j].z());
						glVertex3d(s[indexOf(i+1, j)].x(), s[indexOf(i+1, j)].y(), s[indexOf(i+1,j)].z());

						glNormal3d(n[(i + 1)*side_length + j + 1].x(), n[(i + 1)*side_length + j + 1].y(), n[(i + 1)*side_length + j + 1].z());
						glVertex3d(s[indexOf(i + 1, j + 1)].x(), s[indexOf(i + 1, j + 1)].y(), s[indexOf(i + 1, j + 1)].z());

					}
				}
			}
			glEnd();
			glPopMatrix();
		}
	}

}

#pragma endregion