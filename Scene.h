#include <vector>
#include <iostream>
#include <random>

struct scene_out //Structure of the output of the method intersect (index of the sphere and distance)
{
	int ind; //indice object
	double t; //minimal distance
	Vector P; //intersection point
	Vector N; //normal
};

class Scene
{
public:

	std::vector<Object*> objects;
	
	Vector C = Vector(0, -20, 70); // position de la caméra
	Vector L = Vector(10, 20, 40); //Light
	double I = 450000000; //Intensité
	bool multi_rays = false;

	void addObject(Object *O)
	{
		objects.push_back(O);
	}

	scene_out intersect(Ray R)
	{
		// Initialisation of all the variables that will be passed to main.cpp
		double tmin = 5000000000000.0;
		double tmax = tmin;
		int index = 100;
		scene_out result = {-1, NULL, Vector(NULL,NULL,NULL), Vector(NULL, NULL, NULL)}; //if no intersection

		for (int i = 0; i < objects.size(); i++) //we iterate over the spheres
		{
			object_out inter_out = objects[i]->intersect(R);
			double t = inter_out.t;

			if (t != NULL && t < tmin) //if intesection, we check if inferior to the min already found
			{
				tmin = t;
				result.P = R.C + R.u * t;
				result.N = inter_out.N;
				index = i;
			}
		}

		if (tmin == tmax) //nothing has been intersected
		{
			scene_out result = {-1, NULL, Vector(NULL,NULL,NULL), Vector(NULL, NULL, NULL) };
			return result;
		}

		result.t = tmin;
		result.ind = index;

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
			
			scene_out Z1 = intersect(r);
			double distance_to_object = Z1.t;

			if (distance_to_object != NULL) // if there exist an intersection
			{
				int index = Z1.ind;

				Object *s = objects[index];

				Vector coefColor = s->mat.Color;

				Vector P = Z1.P; //intersection point

				Vector g = L - P; // We create another ray from P to Light to create shadows

				double g_norm = g.getNorm2();
				g.normalize();

				Vector normal = Z1.N; //normal vector

				Vector PP = P + normal * 0.0000001;//intersection point slighty moved out of the sphere
				Ray second_Ray = Ray(PP, g);

				scene_out Z2 = intersect(second_Ray);
				double dist_obj_obj2 = Z2.t;

				Vector indirect_intensity = Vector(0, 0, 0);

				if (multi_rays == true)
				{
					// Ajout de la contribution indirecte
					double r1 = uniform(engine);
					double r2 = uniform(engine);
					Vector aleatoire = Vector(uniform(engine) - 0.5, uniform(engine) - 0.5, uniform(engine) - 0.5);
					Vector tangent1 = normal ^ aleatoire;
					Vector tangent2 = tangent1 ^ normal;

					Vector direction_aleatoire_repere_local(cos(2 * 3.1415926535*r1) * sqrt(1 - r2), sin(2 * 3.1415926535*r1) * sqrt(1 - r2), sqrt(r2));
					Vector direction_aleatoire = normal * direction_aleatoire_repere_local[2] + tangent1 * direction_aleatoire_repere_local[0] + tangent2 * direction_aleatoire_repere_local[1];

					Ray rayon_aleatoire = Ray(P + normal * 0.00001, direction_aleatoire);
					indirect_intensity = getColor(rayon_aleatoire, nb_steps + 1); //*s->albedo;
				}

				if (s->mat.isMirror == true) //if surface is a mirror
				{
					if ((dist_obj_obj2 != NULL) && (dist_obj_obj2 < sqrt(g_norm))) // si objet qui créé de l'ombre (c'est à dire entre objet direct et lampe)
					{
						Vector out = Vector(0, 0, 0);
						return out + indirect_intensity;
					}
					else //pas d'ombre
					{
						Ray reflector = s->reflection(r.u, P, normal);
						reflector.C = PP;
						Vector out = getColor(reflector, nb_steps + 1); //on récupère la couleur du nouveau rayon
						return out + indirect_intensity;
					}
				}

				else if (s->mat.Optic_indice != NULL) //surface is transparent
				{
					Ray refracted = s->refraction(r.u, P, normal);

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




