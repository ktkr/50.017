#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor( d_color),specularColor(s_color), shininess(s)
  {

  }

  virtual ~Material()
    {

    }

  Vector3f getDiffuseColor(Hit& h) 
  { 
	  if (t.valid() && h.hasTex) {
		  Vector3f tex_col = t(h.texCoord[0], h.texCoord[1]);
		  return tex_col;
	  }
    return  diffuseColor;
  }
    

  Vector3f Shade( const Ray& ray, const Hit& hit,
                  const Vector3f& dirToLight, const Vector3f& lightColor ) {
	  //diffuse L.N if L.N > 0
	  //else 0
	  Vector3f pixelColor;
	  Vector3f n = hit.getNormal().normalized();
	  float d = Vector3f::dot(dirToLight, n);

	  Vector3f ref_d = (ray.getDirection() - 2 * Vector3f::dot(ray.getDirection(), n)*n);
	  
	  float sp = Vector3f::dot(dirToLight, ref_d);

	  if (d < 0) {
		  d = 0.0f;
	  }

	  if (sp < 0) {
		  sp = 0.0f;
	  }

	  else {
		  sp = pow(sp, shininess);
	  }

	  if (t.valid()) {
		  //use texture color instead of object diffuse color
		  Vector3f tex_col = t(hit.texCoord[0],hit.texCoord[1]); 
		  pixelColor = d * lightColor * tex_col + sp * lightColor * specularColor;
	  }
	  else {
		  pixelColor = d * lightColor * diffuseColor + sp * lightColor * specularColor;
	  }

    return pixelColor; 
		
  }

  void loadTexture(const char * filename){
    t.load(filename);
  }
 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
