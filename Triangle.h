class Triangle:public Object
{
public:
	Triangle(Vector Color, bool isMirror, double Optic_indice, const Vector A, const Vector B, const Vector C): Object(Color, isMirror, Optic_indice), _A(A), _B(B), _C(C) {};
	Vector _A, _B, _C;

	virtual Vector A() { return _A; };
	virtual Vector B() { return _B; };
	virtual Vector C() { return _C; };

	double intersect(Ray& r)
	{
		Vector N = (_B - _A) ^ (_C - _A);
		N.normalize();
		double t = -(_C - r.C).dot(N) / ((r.u).dot(N));

		if (t < 0) return NULL;

		Vector P = r.C + r.u * t;
		Vector u = _B - _A;
		Vector v = _C - _A;
		Vector w = P - _A;

		double m11 = u.getNorm2();
		double m12 = u.dot(v);
		double m22 = v.getNorm2();
		double detm = m11 * m22 - m12 * m12;

		double b11 = w.dot(u);
		double b21 = w.dot(v);
		double detb = b11 * m22 - m12 * b21;
		double beta = detb / detm;  //coordonnées barycentriques par rapport à B

		double g12 = b11;
		double g22 = b21;
		double detg = m11 * g22 - g12 * m12;
		double gamma = detg / detm;  //coordonnées barycentriques par rapport à C

		double alpha = 1 - beta - gamma;
		if (alpha < 0 || alpha> 1) return NULL;
		if (beta < 0 || beta>1) return NULL;
		if (gamma < 0 || gamma>1) return NULL;

		return t;
	}






		

};