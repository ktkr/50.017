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
	return Vector3f(incoming - 2 * Vector3f::dot(incoming, normal)*normal);

}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
	float d_dot_n = Vector3f::dot(incoming, normal);

}

RayTracer::RayTracer( SceneParser * scene, int max_bounces ):m_scene(scene)
{

	this->m_scene = scene;
	m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

float RayTracer::getReflectivity(float inc_refr_index, float surf_refr_index) const {
	return pow(fabsf((inc_refr_index - surf_refr_index) / (inc_refr_index + surf_refr_index)),2);
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces, Hit& hit ) const
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
		
		Vector3f intersection = ray.getOrigin() + ray.getDirection() * hit.getT();
		//ambient lighting based on texture/diffuse color
		//cambient * ka + sum  L.N * clight * kd + L.R * clight * ks
		if (hit.hasTex) {
			col = m_scene->getAmbientLight()*hit.getMaterial()->getTextureColor(hit);
		}
		else {
			col = m_scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
		}


		//////////////////////////////////SHADOWS/////////////////////////////////////
		for (int k = 0; k < m_scene->getNumLights(); k++) {
			//get light illumination
			Vector3f lightCol;
			Vector3f dirToLight;
			float distToLight = -1.0f;
			m_scene->getLight(k)->getIllumination(intersection, dirToLight, lightCol, distToLight);
			Hit shadowHit = Hit();
			Ray shadowRay = Ray(ray.getOrigin()  +  ray.getDirection() * hit.getT(), dirToLight);
			//Send ray to light source, if intersect an object, discard, else proceed.
			bool shadowIntersected = g->intersect(shadowRay, shadowHit, tmin + EPSILON);
			//should limit the intersection up until the light distance
			if (shadowIntersected) {
				if (shadowHit.getT() > distToLight) {
					Vector3f diffCol = hit.getMaterial()->Shade(ray, hit, dirToLight, lightCol);
					col = col + diffCol;
				}
			}
			else {
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
		//still have bounces
		//////////////////////////////REFLECTIONS/////////////////////////////////////
		//should be a reflected ray with a new hit.
		//currently does not check if material is reflective at all or not, could be faster!
		Ray reflectedRay = Ray(ray.getOrigin() + ray.getDirection()*hit.getT(), mirrorDirection(hit.getNormal(), ray.getDirection()));
		Hit reflectedHit = Hit();
		//Add the color seen by the reflected ray times the reflection color to the color computed for the current ray.
		//Get reflected ray's color and multiply by reflectivity
		Vector3f reflectedCol = traceRay(reflectedRay, tmin+EPSILON, bounces - 1, reflectedHit);
		if (reflectedHit.getMaterial()) {
			col = col + reflectedCol * getReflectivity(hit.getMaterial()->getRefractionIndex(), reflectedHit.getMaterial()->getRefractionIndex());
		}
		else {
			col = col + reflectedCol;
		}

		//////////////////////////////REFRACTIONS//////////////////////////////////////////
		Ray refractedRay = Ray(ray.getOrigin() + ray.getDirection()*hit.getT(),)

		
		return col;
		
	}
	//if no intersection
	return m_scene->getBackgroundColor(ray.getDirection());

}
