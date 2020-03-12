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

class Sphere: public Object
{
public:
	Sphere() {};
	Sphere(Material material_, Vector o, double r) : Object(material_), O(o), R(r) {};

	Vector O;
	double R;



	object_out intersect(Ray& r)
	{

		object_out result = {NULL, Vector(NULL,NULL,NULL), Vector(NULL, NULL, NULL)}; // (t, P, N)

		double a = 1;
		double b = 2 * r.u.dot(r.C-O);
		double c = (r.C - O).getNorm2() - R * R;

		double delta = b * b - 4 * a * c;
		
		if (delta < 0) //no intersection
		{
			return result; 
		}

		double sqrtdelta = sqrt(delta);
		double t1 = (-b - sqrtdelta) / (2 * a);
		double t2 = (-b + sqrtdelta) / (2 * a);

		if ((t1 < 0)&&(t2<0))
		{	
			return result;
		}

		else if ((t1 > 0) && (t2 > 0))
		{
			result.t = t1;
			result.P = r.C + r.u * result.t;
			result.N = getNormal(result.P);
		}

		else if (t1 == t2)
		{
			result.t = t1;
			result.P = r.C + r.u * result.t;
			result.N = getNormal(result.P);
		}

		else if ((t1 <= 0) && (t2 >= 0))
		{
			result.t = t2;
			result.P = r.C + r.u * result.t;
			result.N = getNormal(result.P);
		}
		else
		{
			cout << "error : intersetct sphere" << endl;
		}

		return result;
			
	}

	Vector getNormal(Vector P)
	{
		Vector n = P - O;
		n.normalize();
		return n;
	}


};





