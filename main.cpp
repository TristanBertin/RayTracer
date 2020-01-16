#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <math.h>

#include <iostream>
using namespace std;
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"

int main() {

	int W = 512;
	int H = 512;
	std::vector<int>;
	std::vector<unsigned char> image(W*H * 3, 0);

	Sphere s11(Vector(15,19,-10), 10, Vector(0.8,0.9,0.9),true, NULL); //Sphere 1
	Sphere s12(Vector(-15, 19, -10), 10, Vector(0.2, 0.9, 1.5), false, 1.5); //Sphere 1

	Sphere s3(Vector(500, 0, 0), 470, Vector(0.1, 0.6, 0.9), false, NULL); //Mur droite
	Sphere s4(Vector(-500, 0, 0), 470, Vector(0.1, 0.6, 0.9), false, NULL); //Mur gauche
	Sphere s5(Vector(0, -500, 0), 470, Vector(0.99, 0.99, 0.99), false, NULL); //Plafond
	Sphere s6(Vector(0, 500, 0), 470, Vector(0.99, 0.99, 0.99), false, NULL); //Mur bas
	Sphere s7(Vector(0, 0, -1000), 950, Vector(0.4, 0.65, 0.45), false, NULL); //Mur fond
	Sphere s8(Vector(0, 0, 1020), 950, Vector(0.99, 0.99, 0.99), true, NULL); //Mur derrière caméra

	Scene scene;
	scene.addSphere(s11);
	scene.addSphere(s12);
	scene.addSphere(s3);
	scene.addSphere(s4);
	scene.addSphere(s5);
	scene.addSphere(s6);
	scene.addSphere(s7);
	scene.addSphere(s8);

	int w = W / 2;
	int h = H / 2;
	double fov = 60.0 * 3.14159265 / 180.0;
	double zz = - w / tan(fov / 2);
	
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			Vector v(j - w + 0.5, i - h + 0.5, zz);
			v.normalize();
			Ray u = Ray(scene.C, v); // rays from camera
			Vector color = scene.getColor(u,0);

			image[(i*W + j) * 3 + 0] = cast(color[0]);
			image[(i*W + j) * 3 + 1] = cast(color[1]);
			image[(i*W + j) * 3 + 2] = cast(color[2]);
		}
	}
	stbi_write_png("image.png", W, H, 3, &image[0], 0);
	
	return 0;
	
	

}
