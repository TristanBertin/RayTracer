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
	double focus_distance = 55;

	bool anti_aliasing = false;

	Vector white = Vector(1, 1, 1);
	Vector red = Vector(0.8, 0.1, 0);
	Vector orange = Vector(1, 0.2, 0.3);
	Vector green = Vector(0.2, 0.8, 0.3);
	Vector blue = Vector(0.08, 0.56, 0.91);

	Material mat0 = Material(white, false, NULL);
	Material mat1 = Material(blue, false, NULL);
	Material mat2 = Material(red, false, 1.5);
	Material mat3 = Material(orange, true, NULL);
	Material mat4 = Material(green, false, NULL);
	
	Object *sL = new Sphere(mat0, Vector(10, 20, 40), 6); //Lampe

	Object *s11 = new Sphere(mat3, Vector(15, -26, -5), 8); //Sphere 1
	Object *s12 = new Sphere(mat3, Vector(-12, -32, 5), 7); //Sphere 2
	Object *s13 = new Sphere(mat3, Vector(-1, -13, -15), 7); //Sphere 2

	Object *s3 = new Sphere(mat1, Vector(1000, 0, 0), 970); //Mur droite
	Object *s4 = new Sphere(mat1, Vector(-1000, 0, 0), 970); //Mur gauche
	Object *s5 = new Sphere(mat0, Vector(0, -1000, 0), 960); //Mur bas
	Object *s6 = new Sphere(mat0, Vector(0, 1000, 0), 980); //Mur plafond
	Object *s7 = new Sphere(mat0, Vector(0, 0, -1000), 970); //Mur fond
	Object *s8 = new Sphere(mat3, Vector(0, 0, 1000), 90); //Mur derrière caméra

	Object *t0 = new Triangle(mat1, Vector(-10, 0, -10), Vector(10, 10, -10), Vector(10, -10, -10));

	Object *g1 = new Geometry("Beautiful_Girl.obj", 15, Vector(15, -32, -3), mat0);
	Object *g2 = new Geometry("CUBE.obj", 6, Vector(-5, -33, -15), mat4);


	Scene scene;

	scene.addObject(sL);
	//scene.addObject(s11);
	scene.addObject(s12);
	scene.addObject(s13);
	scene.addObject(s3);
	scene.addObject(s4);
	scene.addObject(s5);
	scene.addObject(s6);
	scene.addObject(s7);
	scene.addObject(s8);

	//scene.addObject(t0);

	scene.addObject(g1); //girl
	//scene.addObject(g2); //cube

	// To test virtual methods and attributes...
	//Vector aa = dynamic_cast<Triangle*>(s20)->A;
	//Vector bb = dynamic_cast<Triangle*>(s20)->B;
	//double b = dynamic_cast<Sphere*>(scene.objects[0])->R;

	int w = W / 2;
	int h = H / 2;
	double fov = 60.0 * 3.14159265 / 180.0;
	double zz = - w / tan(fov / 2);

	double dx = 0;
	double dy = 0;
	
	#pragma omp parallel for

	for (int i = 0; i < H; i++)
	{
		if ((i % 20) == 0)
		{
			cout << i << " / 512"<<endl;
		}

		for (int j = 0; j < W; j++)
		{

			Vector color = Vector(0, 0, 0);

			for (int k = 0; k < scene.N_rays; k++) {


				//This block is now inside the for loop as we use anti-aliasing
				// Bow Muller Method
				double r1 = uniform(engine);
				double r2 = uniform(engine);
				double R = sqrt(-2 * log(r1)) / 2;
				dx = R * cos(2 * 3.14159265*r2);
				dy = R * sin(2 * 3.14159265*r2);

				double dx_aperture = (uniform(engine) - 0.5) * 5;
				double dy_aperture = (uniform(engine) - 0.5) * 5;

				//Vector v(j - w + 0.5, i - h + 0.5, zz);
				Vector v(-j + w - 0.5 + dx, -i + h - 0.5 + dy, zz); //because of inversion
				v.normalize();

				Vector destination = scene.C + v * focus_distance;
				Vector new_origin = scene.C + Vector(dx_aperture, dy_aperture, 0);

				Vector new_direction = (destination - new_origin);
				new_direction.normalize();

				Ray u = Ray(new_origin, new_direction); 
				color = color + scene.getColor(u, 0);
			}

			image[(i*W + j) * 3 + 0] = cast(pow(color[0], gamma)/3.141592);
			image[(i*W + j) * 3 + 1] = cast(pow(color[1], gamma)/3.141592);
			image[(i*W + j) * 3 + 2] = cast(pow(color[2], gamma)/3.141592);
		}
	}

	stbi_write_png("image.png", W, H, 3, &image[0], 0);
	
	return 0;
}
