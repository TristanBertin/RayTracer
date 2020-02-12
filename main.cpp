#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <math.h>
#include <random>

#include <iostream>
using namespace std;

std::default_random_engine engine;
std::uniform_real_distribution<double> uniform(0, 1);

#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Vector.h"
#include "Ray.h"
#include "Object.h"
#include "Triangle.h"
#include "Sphere.h"

#include "Scene.h"


int main() {

	int W = 512;
	int H = 512;
	std::vector<int>;
	std::vector<unsigned char> image(W*H * 3, 0);
	double gamma = 1/2.2;
	double gamma2 = sqrt(2.2);
	double N_rays = 8;

	Object *s11 = new Sphere(Vector(0.8,0.9,0.9),false, NULL, Vector(15, 19, -10), 10); //Sphere 1
	Object *s12 = new Sphere(Vector(0.2, 0.9, 1.5), false, 1.5, Vector(-15, 19, -10), 10); //Sphere 2

	Object *s3 = new Sphere(Vector(0.1, 0.6, 0.9), false, NULL, Vector(500, 0, 0), 470); //Mur droite
	Object *s4 = new Sphere(Vector(0.1, 0.6, 0.9), false, NULL, Vector(-500, 0, 0), 470); //Mur gauche
	Object *s5 = new Sphere(Vector(0.99, 0.99, 0.99), false, NULL, Vector(0, -500, 0), 470); //Plafond
	Object *s6 = new Sphere(Vector(0.99, 0.99, 0.99), false, NULL, Vector(0, 500, 0), 470); //Mur bas
	Object *s7 = new Sphere(Vector(0.99, 0.99, 0.99), false, NULL, Vector(0, 0, -1000), 950); //Mur fond
	Object *s8 = new Sphere(Vector(0.99, 0.99, 0.99), false, NULL, Vector(0, 0, 1020), 950); //Mur derrière caméra
	
	Scene scene;


	scene.addObject(s11);

	scene.addObject(s12);
	scene.addObject(s3);
	scene.addObject(s4);
	scene.addObject(s5);
	scene.addObject(s6);
	scene.addObject(s7);
	scene.addObject(s8);

	Vector a = scene.objects[0]->Color;
	double b = scene.objects[0]->R();

	int w = W / 2;
	int h = H / 2;
	double fov = 60.0 * 3.14159265 / 180.0;
	double zz = - w / tan(fov / 2);
	
	#pragma omp parallel for 

	for (int i = 0; i < H; i++)
	{
		if ((i % 50) == 0)
		{
			cout << i << " / 512"<<endl;
		}
		for (int j = 0; j < W; j++)
		{
			Vector v(j - w + 0.5, i - h + 0.5, zz);
			v.normalize();
			Ray u = Ray(scene.C, v); // rays from camera

			Vector color = scene.getColor(u, 0);

			for (int k=0; k < N_rays; k++)

				color = color + scene.getColor(u, 0);

			image[(i*W + j) * 3 + 0] = cast(pow(color[0], gamma)/3.141592);
			image[(i*W + j) * 3 + 1] = cast(pow(color[1], gamma)/3.141592);
			image[(i*W + j) * 3 + 2] = cast(pow(color[2], gamma)/3.141592);

			
		}
	}
	stbi_write_png("image.png", W, H, 3, &image[0], 0);
	
	return 0;
	
	

}
