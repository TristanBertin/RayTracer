


class TriangleIndices {
public:
	TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk) {
	};
	int vtxi, vtxj, vtxk;
	int uvi, uvj, uvk;
	int ni, nj, nk;
	int faceGroup;
};

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

	friend ostream& operator<<(ostream& os, const Vector& dt);

};

ostream& operator<<(ostream& os, const Vector& V)
{
	os << V[0] << ",  " << V[1] << ",  " << V[2];
	return os;
}

struct object_out
{
	double t; //minimal distance
	Vector P; //intersection point
	Vector N; //normal at the intersection point
};