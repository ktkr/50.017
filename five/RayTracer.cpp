#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
	return (incoming - 2 * Vector3f::dot(incoming, normal)*normal).normalized();

}

bool transmittedDirection(const Vector3f& normal, const Vector3f& incoming,
	float index_n, float index_nt,
	Vector3f& transmitted)
{	
	Vector3f n = Vector3f(normal);
	Vector3f inc = Vector3f(incoming);
	float d_dot_n = Vector3f::dot(inc, n);
	float sqrtTerm = 1.0f - (pow(index_n, 2) * (1.0f - pow(d_dot_n, 2)) / pow(index_nt, 2));
	bool isTransmitted = (sqrtTerm >= 0);
	if (isTransmitted) {
		transmitted = index_n * (inc - n * d_dot_n) / index_nt - n * sqrtf(sqrtTerm);
		transmitted = transmitted.normalized();
	}
	else {
		transmitted = Vector3f::ZERO;
	}
	return isTransmitted;
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces ):m_scene(scene)
{

	this->m_scene = scene;
	m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

float getReflectivity(float inc_refr_index, float surf_refr_index) {
	return pow((surf_refr_index - inc_refr_index) / (inc_refr_index + surf_refr_index),2);
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,float refr_index, Hit& hit ) const
{	
	//if hit reflective/transparent surface, trace a new ray recursively 
	//implement this before testing with 0 bounce.
	//upon hitting, do whatever raycast actions have be done previously.
	//if never hit, show background color
    Group * g = m_scene->getGroup();
	Vector3f col = Vector3f::ZERO;

	//intersection
	bool intersected = g->intersect(ray, hit, tmin);
	if (intersected) {
		Vector3f hitNormal = hit.getNormal().normalized();


		Vector3f intersection = ray.pointAtParameter(hit.getT());
		//ambient lighting based on texture/diffuse color
		//cambient * ka + sum  L.N * clight * kd + L.R * clight * ks
		
		col = m_scene->getAmbientLight()*hit.getMaterial()->getDiffuseColor();

		//////////////////////////////////SHADOWS/////////////////////////////////////
		for (int k = 0; k < m_scene->getNumLights(); k++) {
			//get light illumination
			Vector3f lightCol;
			Vector3f dirToLight;
			float distToLight;
			m_scene->getLight(k)->getIllumination(intersection, dirToLight, lightCol, distToLight);
			Hit shadowHit = Hit(distToLight,NULL,NULL);
			Ray shadowRay = Ray(intersection, dirToLight);
			//Send ray to light source, if intersect an object, discard, else proceed.
			bool shadowIntersected = g->intersect(shadowRay, shadowHit, EPSILON);
			//should limit the intersection up until the light distance
			if (shadowHit.getT() == distToLight) {
				Vector3f diffCol = hit.getMaterial()->Shade(ray, hit, dirToLight, lightCol);
				col = col + diffCol;
			}

		}
		//ray can only bounce as many times as allowed.
		if (bounces > m_maxBounces) {
			bounces = m_maxBounces;
		}
		//exit condition
		if (bounces == 0) {
			return col;
		}

		float refr_index_nt;
		if (Vector3f::dot(ray.getDirection(), hitNormal) > 0) {
			refr_index_nt = 1.0f;
			hitNormal = hitNormal * -1.0f;
		}
		else {
			refr_index_nt = hit.getMaterial()->getRefractionIndex();
		}

		//still have bounces
		//////////////////////////////REFLECTIONS/////////////////////////////////////
		//should be a reflected ray with a new hit.
		//currently does not check if material is reflective at all or not, could be faster!
		Ray reflectedRay = Ray(intersection, mirrorDirection(hitNormal, ray.getDirection()));
		Hit reflectedHit = Hit();
		Vector3f reflectedCol = Vector3f::ZERO;
		
		//Add the color seen by the reflected ray times the reflection color to the color computed for the current ray.
		
		reflectedCol = traceRay(reflectedRay, EPSILON, bounces - 1, refr_index, reflectedHit);
		//store reflected color for now, need to blend with refracted color
		//read the rt.pdf for this info

		reflectedCol = reflectedCol * hit.getMaterial()->getSpecularColor();
		//reflected weight
		//////////////////////////////REFRACTIONS////////////////////////////////////////// might have problem here
		Vector3f refractedCol = Vector3f::ZERO;
		if (hit.getMaterial()->getRefractionIndex() > 0) { //if refraction possible
			Vector3f refrDirection = Vector3f::ZERO;
			//hack for fixing inside object ray tracing (hit always gives the object's refractive index, not suitable for back intersection)

			bool transmitted = transmittedDirection(hitNormal, ray.getDirection(), refr_index, refr_index_nt, refrDirection);
			if (transmitted) {
				Ray refractedRay = Ray(intersection, refrDirection);
				Hit refractedHit = Hit();
				refractedCol = traceRay(refractedRay, EPSILON, bounces - 1, refr_index_nt, refractedHit);

				refractedCol = refractedCol * hit.getMaterial()->getSpecularColor();
				float c;
				//n <= nt
				if (refr_index <= refr_index_nt) {
					//abs(d.N)
					c = abs(Vector3f::dot(ray.getDirection(), hitNormal));
				}
				else {
					c = abs(Vector3f::dot(refrDirection, hitNormal));
				}

				float reflectivity = getReflectivity(refr_index, refr_index_nt);
				float R = reflectivity + (1.0f - reflectivity) * pow((1.0f - c), 5);
				col = col + R * reflectedCol + (1.0f - R) * refractedCol;
			}
			else {
				col = col + reflectedCol;
			}

		}
		else {

			col = col + reflectedCol;
		}

		
		return col;
		
	}
	//if no intersection
	col = m_scene->getBackgroundColor(ray.getDirection());
	return col;
}
