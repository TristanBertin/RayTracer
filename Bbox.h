
class Bbox {
public : 
	Bbox() {};
	Bbox(const Vector& bmin, const Vector& bmax) : bmin(bmin), bmax(bmax) {};
	Vector bmin, bmax;

	bool intersect(Ray& d)
	{
		double t_1_x = (bmin[0] - d.C[0]) * (1 / d.u[0]);
		double t_2_x = (bmax[0] - d.C[0]) * (1 / d.u[0]);
		double t_min_x = std::min(t_1_x, t_2_x);
		double t_max_x = std::max(t_1_x, t_2_x);

		double t_1_y = (bmin[1] - d.C[1]) * (1 / d.u[1]);
		double t_2_y = (bmax[1] - d.C[1]) * (1 / d.u[1]);
		double t_min_y = std::min(t_1_y, t_2_y);
		double t_max_y = std::max(t_1_y, t_2_y);

		double t_1_z = (bmin[2] - d.C[2]) * (1 / d.u[2]);
		double t_2_z = (bmax[2] - d.C[2]) * (1 / d.u[2]);
		double t_min_z = std::min(t_1_z, t_2_z);
		double t_max_z = std::max(t_1_z, t_2_z);


		if (std::min(std::min(t_max_x, t_max_y), t_max_z) > std::max(std::max(t_min_x, t_min_y), t_min_z))
		{
			return true; //intersection
		}

		else {
			return false;
		}
	}

};