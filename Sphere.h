
class Sphere
{
public:
	Sphere(const Vector& O, double R, const Vector& Color) : O(O), R(R), Color(Color) {};
	Vector O;
	double R;
	Vector Color; //Color as an attribute of the Sphere class

	double intersect(Ray& r)
	{
		double a = 1;
		double b = 2 * r.u.dot(r.C-O);
		double c = (r.C - O).getNorm2() - R * R;

		double delta = b * b - 4 * a * c;
		
		if (delta < 0) 
		{
			return NULL; //no intersection
		}

		double sqrtdelta = sqrt(delta);
		double t1 = (-b + sqrtdelta) / (2 * a);

		if (t1 < 0)
		{
			return NULL; //all the intersections are behind the camera
		}

		return (-b - sqrtdelta) / (2 * a); // return the closest solution 
		
	}

};



double cast(double c) //to be sure that the color is between 0 and 255
{
	if (c > 255)
	{
		c = 255;
	}

	if (c < 0)
	{
		c = 0;
	}

	return c;
}

