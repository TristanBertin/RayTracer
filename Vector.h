
class Vector

{
public:

	double coords[3];

	Vector(double x = 0, double y = 0, double z = 0)
	{
		coords[0] = x;
		coords[1] = y;
		coords[2] = z;
	}

	double operator[](int i) const 
	{
		return coords[i];
	}

	void normalize()
	{
		double norm = sqrt(coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2]);
		coords[0] = coords[0] / norm;
		coords[1] = coords[1] / norm;
		coords[2] = coords[2] / norm;
	}

	double& operator[](int i)
	{
		return coords[i];
	}

	double getNorm2() const
	{
		return coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2];
	}

	double dot(Vector& B)
	{
		return coords[0] * B[0] + coords[1] * B[1] + coords[2] * B[2];
	}

	Vector operator+(Vector& B)
	{
		return Vector(coords[0] + B[0], coords[1] + B[1], coords[2] + B[2]);
	}

	Vector operator-(Vector& B)
	{
		return Vector(coords[0] - B[0], coords[1] - B[1], coords[2] - B[2]);
	}

	Vector operator*(double a)
	{
		return Vector(coords[0] * a, coords[1] * a, coords[2] * a);
	}

	Vector operator^(Vector a)
	{
		return Vector(coords[1] * a[2] - coords[2] * a[1],
			coords[2] * a[0] - coords[0] * a[2],
			coords[0] * a[1] - coords[1] * a[0]);
	}

	void operator=(const Vector& B)
	{
		coords[0] = B[0];
		coords[1] = B[1];
		coords[2] = B[2]; 
	}

};
