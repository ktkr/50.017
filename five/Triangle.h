#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
		this->a = a;
		this->b = b;
		this->c = c;
		
        hasTex = false;
		
	}

	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
		float A = Matrix3f(a - b, a - c, ray.getDirection()).determinant();
		float beta = Matrix3f(a - ray.getOrigin(), a - c, ray.getDirection()).determinant() / A;
		float gamma = Matrix3f(a - b, a - ray.getOrigin(), ray.getDirection()).determinant() / A;
		float alpha = 1.0f - beta - gamma;

		if (beta + gamma <= 1 && beta >= 0 && gamma >= 0) {

			float t = Matrix3f(a - b, a - c, a- ray.getOrigin()).determinant() /A;

			if (t >= tmin) {

				if (t < hit.getT()) {
					Vector3f norm = alpha * normals[0] + beta * normals[1] + gamma * normals[2];
					hit.set(t, this->material, norm.normalized());
					if (hasTex) {
						Vector2f coord = alpha * texCoords[0] + beta * texCoords[1] + gamma * texCoords[2];
						hit.setTexCoord(coord);
					}
					
				}
				return true;
				
			}
		}


		return false;
	}
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];
protected:
	Vector3f a;
	Vector3f b;
	Vector3f c;

	float A;

};

#endif //TRIANGLE_H
