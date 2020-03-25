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

Vector getRandomDir(Vector n) {
	double r1 = uniform(engine);
	double r2 = uniform(engine);
	Vector local_random_dir(cos(2 * 3.14159265*r1)*sqrt(1 - r2), sin(2 * 3.14159265*r1)*sqrt(1 - r2), sqrt(r2));
	//Vector3d rand_vec(uniform(engine)-0.5, uniform(engine)-0.5, uniform(engine)); 
	Vector rand_vec = Vector(1, 0, 0);
	Vector tangent1 = n ^ rand_vec;
	tangent1.normalize();
	Vector tangent2 = tangent1 ^ n;
	tangent2.normalize();
	return n * local_random_dir[2] + tangent1 * local_random_dir[0] + tangent2 * local_random_dir[1];
}

class Scene
{
public:

	std::vector<Object*> objects;
	
	Vector C = Vector(0, -20, 50); // position de la caméra
	Vector L = Vector(10, 20, 40); //Light
	//double I = 80000000;; //Intensité
	//int N_rays = 400;
	//double I = 6000000;

	int N_rays = 30;
	double I = 340000000;

	//double I = 400000000;; //Intensité
	//int N_rays = 30;
	bool indirect_I = true;
	bool spherical_light = true;

	Material mat3 = Material(Vector(1,1,1), false, NULL);

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

		for (int i = 1; i < objects.size(); i++) //we iterate over the spheres, not the first one !!
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
		Vector color_pixel = Vector(0, 0, 0);

		if (nb_steps == 5) //max recursive calls
		{
			//cout << r.u[0] << endl;
			return color_pixel; //(0,0,0)
		}

		scene_out Z1 = intersect(r);
		double distance_to_object = Z1.t;

		if (distance_to_object != NULL) // if there exist an intersection
		{
			int index = Z1.ind;
			Object *s = objects[index];
			Vector coefColor = s->mat.Color;
			Vector P = Z1.P; //intersection point
			Vector normal = Z1.N; //normal vector
			Vector PP = P + normal * 0.0001;//intersection point slighty moved out of the sphere
			double scale_light = 1.0;



			/*if (index == 0) //if ray gets to sphere of light
			{
				double R = dynamic_cast<Sphere*>(s)->R;
				Vector HH = dynamic_cast<Sphere*>(s)->mat.Color * (I / (4 * 3.14159265 * R*R));
				return HH;
			}*/

			if (normal.dot(r.u) > 0) normal = normal * (-1); //be careful HERE

			if (s->mat.isMirror == true) //if surface is a mirror
			{
				Ray reflector = s->reflection(r.u, P, normal);
				reflector.C = PP;
				Vector out = getColor(reflector, nb_steps + 1); //on récupère la couleur du nouveau rayon
				return out;
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
				return getColor(refracted, nb_steps + 1);
			}

			Vector l;
			if (spherical_light == false)
			{
				l = L - P; // We create another ray from P to Light to create shadows
			}
			else
			{
				Vector OO = dynamic_cast<Sphere*>(objects[0])->O;
				l = P - OO;
				l.normalize();
				Vector random_dir = getRandomDir(l);
				scale_light = random_dir.dot(l);
				Vector new_L = OO + random_dir * dynamic_cast<Sphere*>(objects[0])->R;
				l = (new_L - P);
			}

			double l_norm = l.getNorm2();
			l.normalize();
			Ray second_Ray = Ray(PP, l);
			scene_out Z2 = intersect(second_Ray);
			double dist_lamp_obj = Z2.t;
				
			if ((dist_lamp_obj != NULL) && (dist_lamp_obj < sqrt(l_norm))) // si objet qui créé de l'ombre (c'est à dire entre objet direct et lampe)
			{
				return color_pixel;
			}

			else //if surface is not a mirror nor transparent, and no shadow
			{
				float compa = l.dot(normal);

				if (compa > 0)
				{
					double intensity = compa * I / l_norm;
					color_pixel = coefColor * (intensity * scale_light / 3.141592);
				}
					
			}

			
			Vector random_dir = getRandomDir(normal);
			Ray random_ray(PP, random_dir);
			color_pixel = color_pixel + getColor(random_ray, nb_steps + 1); //* s->mat.Color;
			
		}

		return color_pixel;

	}


};




