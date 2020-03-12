class Triangle:public Object
{
public:
	Triangle(Material material_, Vector A, Vector B, Vector C): Object(material_), A(A), B(B), C(C) {
	};
	
	Vector &A, &B, &C;

	object_out intersect(Ray& r)
	{

		object_out result = {NULL, Vector(NULL,NULL,NULL), Vector(NULL,NULL,NULL) };

		Vector N = (B - A) ^ (C - A);
		// N = N * (-1.0);
		
		N.normalize();

		double t = (C - r.C).dot(N) / ((r.u).dot(N));

		if (t < 0) return result; //t = NULL

		Vector P = r.C + r.u * t;
		Vector u = B - A;
		Vector v = C - A;
		Vector w = P - A;

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
		if (alpha < 0 || alpha> 1) return result; //t = NULL no intersection
		if (beta < 0 || beta>1) return result; //t = NULL
		if (gamma < 0 || gamma>1) return result; //t = NULL

		result.t = t;
		result.N = N;
		result.P = P;

		//cout << "nn" << N << endl;
		
		return result;
	}


	Vector getNormal(Vector P)
	{
		Vector n = (B - A) ^ (C - A);
		n.normalize();
		return n;
	}
	
};