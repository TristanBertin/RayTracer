/*class Object
{
public:
	Object(Vector color, bool ismirror, double optic_indice) : Color(color), isMirror(ismirror), Optic_indice(optic_indice) {}

    // Attributes that are shared with all the objects
	Vector Color;
	bool isMirror; 
	double Optic_indice; //if = NULL, surface is not transparent, else optical indice

	//Mat material

	// Virtuales methods that refer to the attributes of the Spheres 
	virtual Vector O() = 0;
	virtual double R() = 0;
	virtual double intersect(Ray& r) = 0; // = 0 means that it will consider the method of the considered type of object
	virtual Ray reflection(Vector& i, Vector P) = 0;
	virtual Ray refraction(Vector& i, Vector P) = 0;

	// Virtuales methods that refer to the attributes of the Triangles 
	//virtual Vector A() = 0;
	//virtual Vector B() = 0;
	//virtual Vector C() = 0;


};*/


class Object
{
public:

	Material mat;

	Object() {};
	Object(Material mat) : mat(mat) { };

	std::vector<TriangleIndices> indices;
	std::vector<Vector> vertices;

	virtual Vector getNormal(Vector P) = 0;
	virtual object_out intersect(Ray& r) = 0; // = 0 means that it will consider the method of the considered type of object

	Ray reflection(Vector& i, Vector P, Vector N) //incident ray and intersection point as inputs
	{
		N.normalize();
		Vector r = i - N * (2 * i.dot(N));
		r.normalize();
		return Ray(P + N * 0.00001, r);
	}

	Ray refraction(Vector& i, Vector P, Vector N) //incident ray and intersection point as inputs
	{
		i.normalize();
		//Vector n = (B - A) ^ (C - A);
		N.normalize();

		if (i.dot(N) >= 0) //if refraction is from inside to outisde
		{
			//N = N * (-1);
			mat.Optic_indice = 1 / mat.Optic_indice;
		}

		double out1 = 1 - (1 / (mat.Optic_indice * mat.Optic_indice)) * (1 - i.dot(N)*i.dot(N));
		if (out1 < 0)
		{
			out1 = 0;
		}

		Vector vout = i * (1 / mat.Optic_indice) - N * ((1 / mat.Optic_indice)*i.dot(N) + sqrt(out1));
		vout.normalize();
		Ray out = Ray(P, vout);

		return out;
	}

};
