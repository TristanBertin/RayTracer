#include <vector>
#include <iostream>
#include <random>

struct t_out //Structure of the output of the method intersect (index of the sphere and distance)
{
	int ind;
	double t; //minimal distance
	Vector P; //intersection point
};

class Scene
{
public:

	std::vector<Object*> objects;
	
	Vector C = Vector(0, 15, 45); // position de la caméra
	Vector L = Vector(0, -20, 35); //Light
	double I = 40000000; //Intensité

	void addObject(Object *O)
	{
		objects.push_back(O);
	}

	t_out intersect(Ray R)
	{
		// Initialisation of all the variables that will be passed to main.cpp
		double tmin = 5000000000000.0;
		double tmax = tmin;
		int index = 100;
		Vector P = Vector(NULL, NULL, NULL); //if no intersection

		for (int i = 0; i < objects.size(); i++) //we iterate over the spheres
		{
			if (R.u[0] > 10000 || R.u[0] < -10000) {
				cout << "RRRRRRR      " << R.u[0] << "    " << R.u[1] << "    " << R.u[2] << endl;
			}

			double t = objects[i]->intersect(R);

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
		if (nb_steps == 5)
		{	
			//cout << r.u[0] << endl;
			return Vector(0, 0, 0);
		}

		else
		{
			t_out Z1 = intersect(r);
			//cout << "ccccc    " << nb_steps << "    " << r.C[0] << "    " << r.C[1] << "    " << r.C[2] << endl;
			double distance_to_object = Z1.t;

			if (distance_to_object != NULL) // if there exist an intersection
			{
				int index = Z1.ind;
				Object *s = objects[index];
				Vector coefColor = objects[index]->Color;
				Vector P = Z1.P; //intersection point

				Vector g = L - P;// We create another ray from P to Light to create shadows
				double g_norm = g.getNorm2();
				g.normalize();

				Vector normal = P - s->O(); //normal vector
				normal.normalize();

				Vector PP = P + normal * 0.0000001;//intersection point slighty moved out of the sphere
				Ray second_Ray = Ray(PP, g);
				t_out Z2 = intersect(second_Ray);
				double dist_obj_obj2 = Z2.t;

				// Ajout de la contribution indirecte
				double r1 = uniform(engine);
				double r2 = uniform(engine);
				Vector aleatoire = Vector(uniform(engine)-0.5, uniform(engine)-0.5, uniform(engine)-0.5);
				Vector tangent1 = normal ^ aleatoire;
				Vector tangent2 = tangent1 ^ normal;


				Vector direction_aleatoire_repere_local(cos(2 * 3.1415926535*r1) * sqrt(1 - r2), sin(2 * 3.1415926535*r1) * sqrt(1 - r2), sqrt(r2));
				Vector direction_aleatoire = normal * direction_aleatoire_repere_local[2] + tangent1 * direction_aleatoire_repere_local[0] + tangent2 * direction_aleatoire_repere_local[1];

				Ray rayon_aleatoire = Ray(P + normal * 0.00001, direction_aleatoire);
				Vector indirect_intensity = getColor(rayon_aleatoire, nb_steps + 1); //*s->albedo;

				if (s->isMirror == true) //if surface is a mirror
				{
					if ((dist_obj_obj2 != NULL) && (dist_obj_obj2 < sqrt(g_norm))) // si objet qui créé de l'ombre (c'est à dire entre objet direct et lampe)
					{
						Vector out = Vector(0, 0, 0);
						return out + indirect_intensity;
					}
					else //pas d'ombre
					{
						Ray reflector = s->reflection(r.u, P);
						reflector.C = PP;
						Vector out = getColor(reflector, nb_steps + 1); //on récupère la couleur du nouveau rayon
						return out + indirect_intensity;
					}
				}

				else if (s->Optic_indice != NULL) //surface is transparent
				{
					Ray refracted = s->refraction(r.u, P);

					if (r.u.dot(normal) < 0)
					{
						refracted.C = P - normal * 0.000001;
					}
					else
					{
						refracted.C = P + normal * 0.0000001;
					}
					return getColor(refracted, nb_steps + 1) + indirect_intensity;

				}


				else //if surface is not a mirror nor transparent
				{
					if ((dist_obj_obj2 != NULL) && (dist_obj_obj2 < sqrt(g_norm))) // si objet qui créé de l'ombre (c'est à dire entre objet direct et lampe)
					{
						Vector out = Vector(0, 0, 0);
						return out + indirect_intensity;
					}
					else
					{
						float compa = g.dot(normal);
						if (compa > 0)
						{
							double color = compa * I / g_norm;
							Vector out = Vector(color * coefColor[0], color * coefColor[1], color * coefColor[2]);
							return out + indirect_intensity;
						}
						else
						{
							Vector out = Vector(0, 0, 0);
							return out + indirect_intensity;
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
		


		
	}


};




