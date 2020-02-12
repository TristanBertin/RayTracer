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
	Sphere(Vector Color, bool isMirror, double Optic_indice, Vector o, double r) : Object(Color, isMirror, Optic_indice), _O(o), _R(r) {};
	
	Vector _O;
	double _R;
	virtual Vector O() { return _O; };
	virtual double R() { return _R; };


	double intersect(Ray& r)
	{
		//cout << optic_indice << endl;
		double a = 1;
		double b = 2 * r.u.dot(r.C-_O);
		double c = (r.C - _O).getNorm2() - _R * _R;

		double delta = b * b - 4 * a * c;
		
		if (delta < 0) //no intersection
		{
			return NULL; 
		}

		double sqrtdelta = sqrt(delta);
		double t1 = (-b - sqrtdelta) / (2 * a);
		double t2 = (-b + sqrtdelta) / (2 * a);

		if ((t1 > 0) && (t2 > 0))
		{
			return t1;
		}

		else if ((t1 < 0)&&(t2<0))
		{
			return NULL; //all the intersections are behind the starting point
		}

		else if (t1 == t2)
		{
			return t1;
		}

		else if ((t1 <= 0) && (t2 >= 0))
		{
			return t2;
		}
		else
		{
			cout << "error" << endl;
			return 0;
		}
			
	}

	Ray reflection(Vector& i, Vector P) //incident ray and intersection point as inputs
	{
		Vector n = P - _O;
		n.normalize();
		Vector r = i - n * (2 * i.dot(n));
		r.normalize();
		return Ray(P+n * 0.00001, r);
	}

	Ray refraction(Vector& i, Vector P) //incident ray and intersection point as inputs
	{
		i.normalize();
		Vector n = P - _O;
		n.normalize();

		if (i.dot(n)>=0) //if refraction is from inside to outisde
		{
			n = n * (-1);
			Optic_indice = 1 / Optic_indice;
		}

		double out1 = 1 - (1 / (Optic_indice * Optic_indice)) * (1 - i.dot(n)*i.dot(n));
		if (out1<0)
		{
			out1 = 0;
		}

		Vector vout = i * (1 / Optic_indice) - n * ((1 / Optic_indice)*i.dot(n) + sqrt(out1));
		Ray out = Ray(P, vout);
		return out;
	}

};





