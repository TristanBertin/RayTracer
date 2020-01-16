#include <vector>
#include <iostream>
using namespace std;

struct t_out //Structure of the output of the method intersect (index of the sphere and distance)
{
	int ind;
	double t; //minimal distance
	Vector P; //intersection point
};

class Scene
{

public:
	std::vector<Sphere> spheres;
	Vector C = Vector(0, 0, 45); // position de la caméra
	Vector L = Vector(0, -20, 35); //Light
	double I = 1100000; //Intensité

	void addSphere(Sphere S) 
	{
		spheres.push_back(S); 
	}

	t_out intersect(Ray R)
	{
		// Initialisation of all the variables that will be passed to main.cpp
		double tmin = 5000000000000.0;
		double tmax = tmin;
		int index = 100;
		Vector P = Vector(NULL, NULL, NULL); //if no intersection

		for (int i = 0; i < spheres.size(); i++) //we iterate over the spheres
		{
			double t = spheres[i].intersect(R);

			if (t != NULL && t < tmin) //if intesection, we check if inferior to the min already found
			{
				index = i; 
				tmin = t;
				P = R.C + R.u * t;
			}
		}

		if (tmin == tmax) //no sphere has been intesected
		{
			tmin = NULL;
		}

		t_out result = {index, tmin, P}; //we return an object with the index of the sphere, and the minimal distance

		return result;
	}


	Vector getColor(Ray r, int nb_steps)
	{
		t_out Z1 = intersect(r);
		double distance_to_object = Z1.t;

		if (distance_to_object != NULL) // if there exist an intersection
		{
			int index = Z1.ind;
			Sphere s = spheres[index];
			Vector coefColor = spheres[index].Color;
			Vector P = Z1.P; //intersection point

			Vector g = L - P;// We create another ray from P to Light to create shadows
			double g_norm = g.getNorm2();
			g.normalize();

			Vector normal = P - s.O; //normal vector
			normal.normalize();

			Vector PP = P + normal * 0.0000001;//intersection point slighty moved out of the sphere
			Ray second_Ray = Ray(PP, g);
			t_out Z2 = intersect(second_Ray);
			double dist_obj_obj2 = Z2.t;

			if (s.Specu == true) //if surface is a mirror
			{
				if (nb_steps == 5)
				{
					return Vector(0, 0, 0);
				}

				else
				{
					if ((dist_obj_obj2 != NULL) && (dist_obj_obj2 < sqrt(g_norm))) // si objet qui créé de l'ombre (c'est à dire entre objet direct et lampe)
					{
						Vector out = Vector(0, 0, 0);
						return out;
					}
					else //pas d'ombre
					{
						Ray reflector = s.reflection(r.u, P);
						reflector.C = PP;
						Vector out = getColor(reflector, nb_steps + 1); //on récupère la couleur du nouveau rayon
						return out;
					}
				}
			}

			else if (s.optic_indice != NULL) //surface is transparent
			{

				if (nb_steps == 10)
				{
					return Vector(0, 0, 0);
				}

				else //NO SHADOW !!!!
				{
					
					Ray refracted = s.refraction(r.u, P);

					if (r.u.dot(normal) < 0)
					{
						refracted.C = P - normal * 0.000001;
					}
					else
					{
						refracted.C = P + normal * 0.0000001;
					}
					return getColor(refracted, nb_steps + 1);

				}

			}

			else //if surface is not a mirror nor transparent
			{
				if ((dist_obj_obj2 != NULL) && (dist_obj_obj2 < sqrt(g_norm))) // si objet qui créé de l'ombre (c'est à dire entre objet direct et lampe)
				{
					Vector out = Vector(0, 0, 0);
					return out;
				}
				else
				{
					float compa = g.dot(normal);
					if (compa > 0)
					{
						double color = compa * I / g_norm;
						Vector out = Vector(cast(color * coefColor[0]), cast(color * coefColor[1]), cast(color * coefColor[2]));
						return out;
					}
					else
					{
						Vector out = Vector(0, 0, 0);
						return out;
					}
				}
				

			}
			
		}
		
		else //no direct intersection
		{
			Vector out = Vector(0, 0, 0);
			return out;
		}
		
	}






};




