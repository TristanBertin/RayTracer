class Object
{
public:
	Object(Vector color, bool ismirror, double optic_indice) : Color(color), isMirror(ismirror), Optic_indice(optic_indice) {}

    // Attributes that are shared with all the objects
	Vector Color;
	bool isMirror; 
	double Optic_indice; //if = NULL, surface is not transparent, else optical indice

	// Virtuales methods that refer to the attributes of the spheres 
	virtual Vector O() = 0;
	virtual double R() = 0;
	virtual double intersect(Ray& r) = 0; // = 0 means that it will consider the method of the considered type of object
	virtual Ray reflection(Vector& i, Vector P) = 0;
	virtual Ray refraction(Vector& i, Vector P) = 0;
};

