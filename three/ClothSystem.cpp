#include "ClothSystem.h"
#include "spring.h"
//TODO: Initialize here

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
			state.push_back(Vector3f(j*0.1f,i*0.1f,0)); // position
			state.push_back(Vector3f(0.0f)); // velocity
		}
	}

	setState(state);
#pragma endregion
#pragma region springs
	for (unsigned i = 0; i < length - 1; i++) {
		for (unsigned j = 0; j < length - 1; j++) { //-1 from segmenting , e.g. 8 points, 7 segments

			//generate structural springs
			float rest_length = (state[indexOf(i,j)] - state[indexOf(i,j+1)]).abs();
			Spring s_horiz(indexOf(i,j), indexOf(i,j+1), rest_length); //horizontal springs
			rest_length = (state[indexOf(i, j)] - state[indexOf(i, j + 1)]).abs();
			Spring s_vert(indexOf(i, j), indexOf(i + 1, j), rest_length);
			structural.push_back(s_horiz);
			structural.push_back(s_vert);

			//generate shear springs
			float rest_length = (state[indexOf(i, j)] - state[indexOf(i + 1, j + 1)]).abs();
			Spring s_tl_br(indexOf(i, j), indexOf(i + 1, j + 1), rest_length);//top left to bottom right
			rest_length = (state[indexOf(i+1, j)] - state[indexOf(i, j + 1)]).abs();
			Spring s_bl_tr(indexOf(i + 1, j), indexOf(i, j + 1), rest_length);
			shear.push_back(s_tl_br);
			shear.push_back(s_bl_tr);

			//generate flex springs
			//do checks on exceeding the index.
			if (i + 2 < length && j + 2 < length) {
				float rest_length = (state[indexOf(i, j)] - state[indexOf(i, j + 2)]).abs();
				Spring s_horiz2(indexOf(i, j), indexOf(i, j + 2), rest_length);//top left to bottom right
				rest_length = (state[indexOf(i, j)] - state[indexOf(i + 2, j)]).abs();
				Spring s_vert2(indexOf(i, j), indexOf(i + 2, j), rest_length);
				flex.push_back(s_horiz2);
				flex.push_back(s_vert2);
			}
		}
	}
#pragma endregion
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	return vector<Vector3f>();
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	//for(unsigned i =0 ; i < )

}

