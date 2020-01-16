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

class Sphere
{
public:
	Sphere(const Vector& O, double R, const Vector& Color, bool Specu, double optic_indice) : O(O), R(R), Color(Color), Specu(Specu), optic_indice(optic_indice){};
	Vector O;
	double R;
	Vector Color; //Color as an attribute of the Sphere class
	bool Specu; //if is Mirror
	double optic_indice; //if = NULL, surface is not transparent, else optical indice

	double intersect(Ray& r)
	{
		double a = 1;
		double b = 2 * r.u.dot(r.C-O);
		double c = (r.C - O).getNorm2() - R * R;

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
			cout << "errrorororororor    t1" << t1 << "     t2  " << t2 << endl;

			return 0;
		}
		
	}

	Ray reflection(Vector& i, Vector P) //incident ray and intersection point as inputs
	{
		Vector n = P - O;
		n.normalize();
		Vector r = i - n * (2 * i.dot(n));
		r.normalize();
		return Ray(P, r);
	}

	Ray refraction(Vector& i, Vector P) //incident ray and intersection point as inputs
	{
		i.normalize();
		Vector n = P - O;
		n.normalize();

		if (i.dot(n)>=0) //if refraction is from inside to outisde
		{
			n = n * (-1);
			optic_indice = 1 / optic_indice;
		}

		double out1 = 1 - (1 / (optic_indice * optic_indice)) * (1 - i.dot(n)*i.dot(n));
		if (out1<0)
		{
			out1 = 0;
		}
		Vector vout = i * (1 / optic_indice) - n * ((1 / optic_indice)*i.dot(n) + sqrt(out1));
		Ray out = Ray(P, vout);
		return out;
	}

};





