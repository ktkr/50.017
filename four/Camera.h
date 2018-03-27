#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		this->center = center;
		//w = direction, u = w � up, v = u � w , w = direction, u = horizontal, v = up
		this->direction = direction.normalized(); //w
		this->horizontal = Vector3f::cross(direction, up).normalized(); //u
		this->up = Vector3f::cross(horizontal, direction).normalized(); //v, recalculate up because up and direction may not be perpendicular
		this->fov = angle;
		D = 1.0f / tan(angle / 2.0f);
	}

	virtual Ray generateRay( const Vector2f& point){
		//r = (x*u, aspect*y*v, D*w)
		//D = 1/ tan(angle/2)		
		Vector3f dir = (point.x() * this->horizontal + point.y()*this->up + D*this->direction).normalized();
		return Ray(center, dir);

	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float fov;
	float D;
};

#endif //CAMERA_H
