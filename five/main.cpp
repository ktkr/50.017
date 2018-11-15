#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <random>

#include "SceneParser.h"
#include "Image.h"
#include <string.h>
#include "RayTracer.h"
using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"

void GaussianBlur() {

}

int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
	
    
  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.
  SceneParser sp = SceneParser(argv[2]);
  int width = stoi(argv[4]);
  int height = stoi(argv[5]);
  char * outFile = argv[7];
  int bounces = stoi(argv[10]);
  //always filter, jitter, bounce.
  Camera * cam = sp.getCamera();
  Group * g = sp.getGroup();
  float tmin = cam->getTMin();
  int nWidth = width * 3;
  int nHeight = height * 3;
  Image image(width, height);
  Image jittered(nWidth,nHeight);
  RayTracer rt = RayTracer(&sp,bounces);
  srand(NULL);

  //for (int i = 0; i < width; i++) {
	 // for (int j = 0; j < height; j++) {
		//  float x = (float(i) / float(width - 1) * 2 - 1);
		//  float y = (float(j) / float(height - 1) * 2 - 1);
		//  //use randomness
		// 
		//  //create ray, find intersections.
		//  Ray r = cam->generateRay(Vector2f(x, y));
		//  Hit h = Hit();
		//  Vector3f col = rt.traceRay(r, tmin, bounces, 1.0f, h);
		//  //write color
		//  image.SetPixel(i, j, col);

	 // }
	 // if (i % 100 == 0) {
		//  std::cout << "\n" << "i =" << i << endl;
	 // }
	 // else {
		//  std::cout << ".";
	 // }
  //}
  //random_device rd;
  //mt19937 gen(rd());
  //const uniform_real_distribution<float> frand(-0.5f, 0.5f);

  //image.SaveImage(outFile);
  //3x resolution for jittered
  for (int i = 0; i < nWidth; i++) {
	  for (int j = 0; j < nHeight; j++) {
		  //float ri = frand(gen);
		  //float rj = frand(gen);
		  float ri = (-0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		  float rj = (-0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		  float x = (2* float(i+ri) / float(nWidth-1) - 1);
		  float y = (2* float(j+rj) / float(nHeight-1) - 1);
		  //use randomness


		  //create ray, find intersections.
		  Ray r = cam->generateRay(Vector2f(x, y));
		  Hit h = Hit();
		  Vector3f col = rt.traceRay(r, tmin, bounces, 1.0f, h);
		  //write color
		  jittered.SetPixel(i, j, col);
		  
		}
	  if (i % 100 == 0) {
		  std::cout <<"\n" << "i =" << i << endl;
	  }
	  else {
		  std::cout << ".";
	  }
  }
  Image gaussian(nWidth, nHeight);
  float K[] = { 0.1201f,0.2339f,0.2931f,0.2339f,0.1201f };
  for (int i = 0; i < nWidth; i++) {
	  for (int j = 0; j < nHeight; j++) {
		  
		  vector<int> y;
		  for (int k = -2; k < 3; k++) {
			  if (j + k < 0) {
				  y.push_back(0);
			  }
			  else if (j + k >= nWidth) {
				  y.push_back(nWidth - 1);
			  }
			  else {
				  y.push_back(j + k);
			  }
			  
		  }
		  Vector3f col = Vector3f::ZERO;
		  for (int k = 0; k < 5; k++) {
			  col += jittered.GetPixel(i, y[k])*K[k];
		  }
		  gaussian.SetPixel(i, j, col);
	  }
  }
  Image gaussian2(nWidth, nHeight);
  for (int i = 0; i < nWidth; i++) {
	  for (int j = 0; j < nHeight; j++) {

		  vector<int> x;
		  for (int k = -2; k < 3; k++) {
			  if (i + k < 0) {
				  x.push_back(0);
			  }
			  else if (i + k >= nHeight) {
				  x.push_back(nHeight - 1);
			  }
			  else {
				  x.push_back(i + k);
			  }

		  }
		  Vector3f col = Vector3f::ZERO;
		  for (int k = 0; k < 5; k++) {
			  col += gaussian.GetPixel(x[k], j)*K[k];
		  }
		  gaussian2.SetPixel(i, j, col);
	  }
  }
  //Down-sampling, average 3x3s
  for (int i = 0; i < width; i++) {
	  for (int j = 0; j < height; j++) {
		  Vector3f averageCol = Vector3f::ZERO;
		  for (int k = 0; k < 3; k++) {
			  for (int l = 0; l < 3; l++) {
				  averageCol += gaussian2.GetPixel(i*3 + k, j*3 + l);
			  }
		  }
		  averageCol = averageCol / 9.0f;
		  image.SetPixel(i, j, averageCol);
	  }
  }

  image.SaveImage(outFile);
 // std::getchar();

}

