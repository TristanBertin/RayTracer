#include <vector>
#include <iostream>
using namespace std;

struct t_out //Structure of the output of the method intersect (index of the sphere and distance)
{
	int ind;
	double t; //minimal distance
};

class Scene
{

public:
	std::vector<Sphere> spheres;

	void addSphere(Sphere S) 
	{
		spheres.push_back(S); 
	}

	t_out intersect(Ray R)
	{
		// Initialisation of all the variables that will be passed to main.cpp
		double tmin = 5000000.0;
		double tmax = tmin;
		int index = 100;
		Vector col = Vector(0, 0, 0);


		for (int i = 0; i < spheres.size(); i++) //we iterate over the spheres
		{
			double t = spheres[i].intersect(R);

			if (t != NULL && t < tmin) //if intesection, we check if inferior to the min already found
			{
				index = i; 
				tmin = t;
			}
		}

		if (tmin == tmax) //no sphere has been intesected
		{
			tmin = NULL;
		}

		t_out result = {index, tmin}; //we return an object with the index of the sphere, t and the vector of color's coefficients

		return result;
	}






	/*
	Scene(const int nb) : nb(nb) {};
	Sphere *spheres[5];
	*/

	

	
		
	/*
	double 
	CurrentSphere intersection(Ray R);
	std::vector<Sphere> spheres() const { return sphereTab; }
	*/
};




