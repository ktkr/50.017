#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
	}

	//this : convention already calls superclass constructor
	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		this->radius = radius;
		this->center = center;
	}
	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		//H(P) = P.P - r^2 = 0
		float a = r.getDirection().absSquared();
		float b = 2 * Vector3f::dot(r.getDirection(), r.getOrigin()-center);
		float c = Vector3f::dot(r.getOrigin()-center, r.getOrigin()-center) - this->radius*this->radius;
		float d = sqrt(b*b - 4 * a * c);
		float t1 = (-b + d) / (2 * a);
		float t2 = (-b - d) / (2 * a);
		//t initialized as float max
		//if both t are greater than tmin, choose smaller positive then only set h if less than current t
		//if t >= tmin
		float t = FLT_MAX;
		bool intersected = false;
		if (t1 >= tmin) {
			if (t2 >= tmin) {
				//choose smaller t
				t = (t1 < t2) ? t1 : t2;	
			}
			else {
				t = t1;
			}
			intersected = true;
		}
		//if t1 is behind eye
		else {
			if (t2 >= tmin) {
				t = t2;
				intersected = true;
			}			
			//else t is FLT_MAX, nothing changes.
		}

		//if t < current t
		if (t < h.getT()) {
			//check
			h.set(t, this->material, (r.pointAtParameter(t)-center).normalized());

		}
		return intersected;
	}

protected:
	Vector3f center = Vector3f::ZERO;
	float radius = 1.0f;

};


#endif
