class Face
{
public:

	int _v[3] = { -1,-1,-1 };

	Vector N;
	Face() {};
	Face(int v1, int v2, int v3) {
		_v[0] = v1; _v[1] = v2; _v[2] = v3;
	}
	// get
	double v1() const { return _v[0]; }
	double v2() const { return _v[1]; }
	double v3() const { return _v[2]; }

	friend ostream& operator<<(ostream& os, const Face& dt);
};

class Mesh : public Object {
public:
	Material mat;
	Bbox bbox;
	std::vector<Face> faces;
	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Vector> normalsVert;
	int Nv, Nf;
	int id_face; // id of the current intersected triangle

	Mesh() {};
	Mesh(const char* obj, double scaling, const Vector offset, Material mat) : mat(mat), Object(mat) {

		readOBJ(obj);
		computeFacesNormal();
		computeVertNormal();

		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = vertices[i] * scaling + offset;
		}
		create_BBox();
	}

	void create_BBox() {

		Vector c_min = vertices[0];
		Vector c_max = vertices[0];
		for (int i = 1; i < vertices.size(); ++i)
		{
			c_min[0] = fmin(c_min.x, vertices[i].x);
			c_max[0] = fmax(c_max.x, vertices[i].x);

			c_min[= fmin(c_min.y, vertices[i].y);
			c_max.y = fmax(c_max.y, vertices[i].y);

			c_min.z = fmin(c_min.z, vertices[i].z);
			c_max.z = fmax(c_max.z, vertices[i].z);
		}
		bbox = BBox(c_min, c_max);
	};

	double intersect(const Ray& r, int& total_ray_casted, InterStruct &interStruct) {

		if (!bbox.intersect(r))
			return -1;

		double min_t = 1E99;
		InterStruct interStruct_tri;
		for (int i = 0; i < faces.size(); i++) // faces.size()
		{
			const Face& face = faces[i];
			const Vector3d v1 = vertices[face.v1()];
			const Vector3d v2 = vertices[face.v2()];
			const Vector3d v3 = vertices[face.v3()];

			Triangle tri = Triangle(v1, v2, v3, mat, false, normals[i]);
			const double t = tri.intersect(r, total_ray_casted, interStruct_tri);

			if (t > 0 && t < min_t) {
				min_t = t;
				id_face = i;
				interStruct = interStruct_tri;
			}
		}
		return min_t;
	}

	Vector3d getNormalAt(const Vector3d& P, bool normalize = true) const {

		return normals[id_face];
		// if(normalsVert.size()>0){
		//     // amélioration en interpolant les normales des 3 sommets de la face avec les coordonnées  barycentrique de P
		//     const Face& face = faces[id_face];
		//     const Vector3d v1 = vertices[face.v1()];
		//     const Vector3d v2 = vertices[face.v2()];
		//     const Vector3d v3 = vertices[face.v3()];
		//     Triangle tri = Triangle(v1, v2, v3, mat, false, normals[id_face]);
		//     double  alpha, beta;
		//     tri.computeBarycentricCoord(alpha, beta, P);
		//     return ...
		// } else 
		// {
		//     return normals[id_face];
		// }    
	}

	void computeFacesNormal() {
		normals.clear();
		normals.reserve(Nf);
		for (int i = 0; i < Nf; i++)
		{
			Face &f = faces[i];

			Vector3d A = vertices[f.v2()] - vertices[f.v1()];
			Vector3d B = vertices[f.v3()] - vertices[f.v1()];
			normals.push_back(A.cross(B).getNormalized());
		}
	}
	void computeVertNormal() {
		normalsVert.clear();
		normalsVert.reserve(Nv);
		for (int i = 0; i < Nv; i++) {
			normalsVert.push_back(Vector3d(0));
		}
		for (int i = 0; i < Nf; i++)
		{
			Face &f = faces[i];
			normalsVert[f.v1()] += normals[i];
			normalsVert[f.v2()] += normals[i];
			normalsVert[f.v3()] += normals[i];
		}
		for (int i = 0; i < Nv; i++) {
			normalsVert[0].normalize();
		}
	}

	void readOBJ(const char file_name[]) {
		FILE *fp;
		fp = fopen(file_name, "r");  //Ouverture d'un fichier en lecture
		if (fp == NULL)
		{
			cout << "Error opening file: " << file_name << endl;
			//exit(1);
			return;
		}
		cout << "Opening " << file_name << endl;

		vertices.clear();
		faces.clear();
		int nb_edge = 0;
		fscanf(fp, "%d %d %d\n", &Nv, &Nf, &nb_edge);
		cout << "nb vertices: " << Nv << ", nb faces: " << Nf << endl;

		vertices.reserve(Nv);
		faces.reserve(Nf);

		double x, y, z;
		for (int i_vertex = 0; i_vertex < Nv; i_vertex++)
		{
			fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
			vertices.push_back(Vector3d(x, z, y));
		}

		int n_face, v1, v2, v3;
		for (int i_triangle = 0; i_triangle < Nf; i_triangle++)
		{
			fscanf(fp, "%d %d %d %d\n", &n_face, &v1, &v2, &v3);
			faces.push_back(Face(v1, v2, v3));
		}
		fclose(fp);
	}

};

#endif //MESH_H