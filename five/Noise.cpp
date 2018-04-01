#include "Noise.h"
#include "PerlinNoise.h"

Vector3f Noise::getColor(const Vector3f & pos)
{
  //Fill in this function  ONLY.
  //INTERPOLATE BETWEEN TWO COLORS BY WEIGHTED AVERAGE
	float noise = PerlinNoise::octaveNoise(pos, octaves);
	//M(x, y, z) = sin(wx + aN(x, y, z))
	float m= sin(frequency*pos.x() + amplitude * noise);
	//color direction vector multiplied by the amount of noise added to the initial color.
	Vector3f colorLerp = (color[0] - color[1]) * m + color[1];
	return colorLerp;
}

Noise::Noise(int _octaves,const Vector3f & color1, 
	 const Vector3f &color2,float freq,float amp):
octaves(_octaves),frequency(freq),amplitude(amp){
	color[0] = color1;
	color[1] = color2;
	init = true;
}

Noise::Noise(const Noise & n):
octaves(n.octaves),frequency(n.frequency),
	amplitude(n.amplitude),init(n.init)
{
	color[0] = n.color[0];
	color[1] = n.color[1];
}

bool Noise::valid()
{
	return init;
}

Noise::Noise():
octaves(0),init(false)
{}
