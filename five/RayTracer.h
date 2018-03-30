#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cassert>
#include <vector>
#include "SceneParser.h"
#include "Ray.h"
#include "Hit.h"

class SceneParser;


class RayTracer
{
public:
  
  RayTracer()
  {
      assert( false );
  }

  RayTracer( SceneParser* scene, int max_bounces //more arguments as you need...
  );
  ~RayTracer();

  float getReflectivity(float inc_refr_index, float surf_refr_index) const;
  
  Vector3f traceRay( Ray& ray, float tmin, int bounces, Hit& hit ) const;
private:
  SceneParser* m_scene;

  int m_maxBounces;

};

#endif // RAY_TRACER_H
