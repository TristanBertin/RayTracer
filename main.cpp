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
	
	Vector C(0, 0, 60); // position de la caméra

	Sphere s1(Vector(-13,15,-2), 10.0, Vector(0.8,0.9,0.9)); //Sphere 1
	Sphere s3(Vector(160, 0, 0), 130.0, Vector(0.9, 0.2, 0.1)); //Mur
	Sphere s4(Vector(-160, 0, 0), 130, Vector(0.1, 0.6, 0.9)); //Mur 
	Sphere s5(Vector(0, -160, 0), 130, Vector(0.9, 0.65, 0.25)); //Mur
	Sphere s6(Vector(0, 160, 0), 130, Vector(0.9, 0.9, 0.9)); //Mur bas
	Sphere s7(Vector(0, 0, -160), 130, Vector(0.4, 0.65, 0.45)); //Mur fond


	Vector L(15, -11, 50); //Light
	double I = 900000; //Intensité

	Scene scene;
	scene.addSphere(s1);
	scene.addSphere(s3);
	scene.addSphere(s4);
	scene.addSphere(s5);
	scene.addSphere(s6);
	scene.addSphere(s7);
	
	int w = W / 2;
	int h = H / 2;
	double fov = 60.0 * 3.14159265 / 180.0;
	double zz = - w / tan(fov / 2);
	

	for (int i = 0; i < H; i++) {

		for (int j = 0; j < W; j++) 
		{
			// 1ere etape
			Vector v(j - w + 0.5, i - h + 0.5, zz);
			v.normalize();

			Ray u = Ray(C, v);
			//double t = s.intersect(u);

			t_out result = scene.intersect(u);

			double t = result.t;
		
			if (t != NULL) // if there exist an intersection
			{
				int index = result.ind;
				Sphere s = scene.spheres[index];
				Vector coefColor = s.Color;

				Vector P = C + v * t;

				// We create another ray from P
				Vector g = L - P;
				double g_norm = g.getNorm2();
				g.normalize();

				Vector l = L - P;
				Vector normale = P - s.O; //normal vector
				normale.normalize();

				Vector PP = P + normale * 0.000001;//intersection point slighty moved out of the sphere

				Ray second_Ray = Ray(PP, g);
				t_out gg = scene.intersect(second_Ray);
				double tt = gg.t;

				//cout << "tt" << tt << "  norm2" << g_norm << endl;

				if (tt != NULL)
				{
					//cout << "heyyyyy tt" << tt << endl;
				}

				if ((tt == NULL)||(tt!=NULL && tt>g_norm)) //pas d'intersection pour le second rayon ou intersection mais derrière lampe
				{

					double d = l.getNorm2();
					l.normalize();

					float compa = l.dot(normale);

					if (compa > 0)
					{
						double color = compa * I / d;

						image[(i*W + j) * 3 + 0] = cast(color * coefColor[0]);
						image[(i*W + j) * 3 + 1] = cast(color * coefColor[1]);
						image[(i*W + j) * 3 + 2] = cast(color * coefColor[2]);
					}
				}
	
			}
			
		}
	}
	
	stbi_write_png("image.png", W, H, 3, &image[0], 0);
	
	return 0;
	
	

}
