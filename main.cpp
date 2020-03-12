#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <math.h>
#include <random>

#include <map>

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
#include "Bbox.h"
#include "Material.h"

#include "Object.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Geometry.h"
#include "Scene.h"


int main() {

	int W = 512;
	int H = 512;
	std::vector<int>;
	std::vector<unsigned char> image(W*H * 3, 0);

	double gamma = 1/2.2;
	double gamma2 = sqrt(2.2);
	double N_rays = 3;

	Vector white = Vector(1, 1, 1);
	Vector red = Vector(1, 0, 0);
	Vector orange = Vector(1, 0.2, 0.3);
	Vector green = Vector(0, 1, 0);
	Vector blue = Vector(0.08, 0.56, 0.91);

	Material mat0 = Material(white, false, NULL);
	Material mat1 = Material(blue, false, NULL);
	Material mat2 = Material(red, false, 1.5);
	Material mat3 = Material(orange, false, NULL);
	Material mat4 = Material(blue, true, NULL);
	
	Object *s11 = new Sphere(mat4, Vector(20, -31, -5), 15); //Sphere 1
	Object *s12 = new Sphere(mat2, Vector(-15, -35, -20), 8); //Sphere 2

	Object *s3 = new Sphere(mat1, Vector(1000, 0, 0), 970); //Mur droite
	Object *s4 = new Sphere(mat1, Vector(-1000, 0, 0), 970); //Mur gauche
	Object *s5 = new Sphere(mat0, Vector(0, -1000, 0), 960); //Mur bas
	Object *s6 = new Sphere(mat0, Vector(0, 1000, 0), 980); //Mur plafond
	Object *s7 = new Sphere(mat0, Vector(0, 0, -1000), 970); //Mur fond
	Object *s8 = new Sphere(mat3, Vector(0, 0, 1000), 90); //Mur derrière caméra

	Object *t0 = new Triangle(mat1, Vector(-10, 0, -10), Vector(10, 10, -10), Vector(10, -10, -10));

	Object *g1 = new Geometry("Beautiful_Girl.obj", 18, Vector(0, -38, 10), mat2);
	Object *g2 = new Geometry("CUBE.obj", 7, Vector(-5, -33, -10), mat4);

	Scene scene;


	scene.addObject(s11);
	//scene.addObject(s12);
	scene.addObject(s3);
	scene.addObject(s4);
	scene.addObject(s5);
	scene.addObject(s6);
	scene.addObject(s7);
	scene.addObject(s8);

	//scene.addObject(t0);

	scene.addObject(g1);
	scene.addObject(g2);

	// To test virtual methods and attributes...
	//Vector aa = dynamic_cast<Triangle*>(s20)->A;
	//Vector bb = dynamic_cast<Triangle*>(s20)->B;
	//double b = dynamic_cast<Sphere*>(scene.objects[0])->R;

	int w = W / 2;
	int h = H / 2;
	double fov = 60.0 * 3.14159265 / 180.0;
	double zz = - w / tan(fov / 2);
	
	#pragma omp parallel for 

	for (int i = 0; i < H; i++)
	{
		if ((i % 20) == 0)
		{
			cout << i << " / 512"<<endl;
		}

		for (int j = 0; j < W; j++)
		{
			//Vector v(j - w + 0.5, i - h + 0.5, zz);
			Vector v(-j + w - 0.5, -i + h - 0.5, zz); //because of inversion
			v.normalize();
			Ray u = Ray(scene.C, v); // rays from camera

			Vector color = Vector(0, 0, 0);

			for (int k = 0; k < N_rays; k++)

				color = color + scene.getColor(u, 0);

			image[(i*W + j) * 3 + 0] = cast(pow(color[0], gamma)/3.141592);
			image[(i*W + j) * 3 + 1] = cast(pow(color[1], gamma)/3.141592);
			image[(i*W + j) * 3 + 2] = cast(pow(color[2], gamma)/3.141592);
		}
	}

	stbi_write_png("image.png", W, H, 3, &image[0], 0);
	
	return 0;
}
