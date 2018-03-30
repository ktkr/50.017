#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
  Transform(){}
 Transform( const Matrix4f& m, Object3D* obj ):o(obj){
	 this->o = obj;
	 this->m = m;
  }
  ~Transform(){
  }
  virtual bool intersect( const Ray& r , Hit& h , float tmin){
	  Ray transformed = Ray((m.inverse()*Vector4f(r.getOrigin(),1)).xyz(), (m.inverse()*Vector4f(r.getDirection(),0)).xyz());
	  bool intersected = o->intersect(transformed, h, tmin);
	  if (intersected) {
		  h.set(h.getT(), h.getMaterial(), (m.inverse().transposed()* Vector4f(h.getNormal(), 0)).xyz().normalized());
	  }
	  return intersected;

  }


 protected:
  Object3D* o; //un-transformed object	
  Matrix4f m;
};

#endif //TRANSFORM_H
