

class Geometry : public Object {

public:

	Geometry() {};
	Geometry(char* obj, double scaling, Vector& offset, Material material_) : Object(material_) {
		readOBJ(obj);
		
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = vertices[i] * scaling + offset;
		}
		Generate_box();


	}

	Bbox bbox;

	std::vector<TriangleIndices> indices;
	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Vector> uvs; // Vector en 3D mais on n'utilise que 2 composantes
	std::vector<Vector> vertexcolors;

	std::vector<std::vector<unsigned char> > textures;
	std::vector<int> w, h;


	Vector getNormal(Vector P)
	{
		cout << "be careful, getNormal in Geometry is used" << endl;
		Vector n = Vector(0, 0, 0);
		n.normalize();
		return n;
	}

	object_out intersect(Ray& r)
	{
		
		object_out result = { NULL, Vector(NULL, NULL, NULL), Vector(NULL, NULL, NULL)};

		if (!bbox.intersect(r))
		{
			return result;
		}

		double t = 1E99;
		double h = t;


		for (int i = 0; i < indices.size(); i++)
		{
			int i0 = indices[i].vtxi;
			int i1 = indices[i].vtxj;
			int i2 = indices[i].vtxk;
			Triangle tri(mat, vertices[i0], vertices[i1], vertices[i2]);

			object_out out = tri.intersect(r);
			double tt = out.t;

			if ((tt < t) && (tt != NULL)) // intersection closer than the min already founded
			{
				t = tt;
				result.P = out.P;
				result.N = out.N;
			}
		}

		if (t == h) return result; //NO intersection for any rectangle

		result.t = t;
		//cout << "intersecté !! " << result.t << endl;
		
		return result;
	}


	void readOBJ(const char* obj) {

		char matfile[255];
		char grp[255];

		FILE* f;
		f = fopen(obj, "r");
		
		std::map<std::string, int> groupNames;
		int curGroup = -1;

		while (!feof(f)) {
			char line[255];
			if (!fgets(line, 255, f)) break;

			std::string linetrim(line);
			linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
			strcpy(line, linetrim.c_str());

			if (line[0] == 'u' && line[1] == 's') {
				sscanf(line, "usemtl %[^\n]\n", grp);
				if (groupNames.find(std::string(grp)) != groupNames.end()) {
					curGroup = groupNames[std::string(grp)];
				}
				else {
					curGroup = groupNames.size();
					groupNames[std::string(grp)] = curGroup;
				}
			}
			if (line[0] == 'm' && line[1] == 't' && line[2] == 'l') {
				sscanf(line, "mtllib %[^\n]\n", matfile);
			}
			if (line[0] == 'v' && line[1] == ' ') {
				Vector vec;
				Vector col;
				if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[2], &vec[1], &col[0], &col[1], &col[2]) == 6) {
					vertices.push_back(vec);
					vertexcolors.push_back(col);
				}
				else {
					sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[2], &vec[1]);  // helmet
																				 //vec[2] = -vec[2]; //car2
					vertices.push_back(vec);
				}
			}
			if (line[0] == 'v' && line[1] == 'n') {
				Vector vec;
				sscanf_s(line, "vn %lf %lf %lf\n", &vec[0], &vec[2], &vec[1]); //girl
				normals.push_back(vec);
			}
			if (line[0] == 'v' && line[1] == 't') {
				Vector vec;
				sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
				uvs.push_back(vec);
			}
			if (line[0] == 'f') {
				TriangleIndices t;
				int i0, i1, i2, i3;
				int j0, j1, j2, j3;
				int k0, k1, k2, k3;
				int nn;

				char* consumedline = line + 1;
				int offset;
				t.faceGroup = curGroup;
				nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
				if (nn == 9) {
					if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
					if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
					if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
					if (j0 < 0) t.uvi = uvs.size() + j0; else   t.uvi = j0 - 1;
					if (j1 < 0) t.uvj = uvs.size() + j1; else   t.uvj = j1 - 1;
					if (j2 < 0) t.uvk = uvs.size() + j2; else   t.uvk = j2 - 1;
					if (k0 < 0) t.ni = normals.size() + k0; else    t.ni = k0 - 1;
					if (k1 < 0) t.nj = normals.size() + k1; else    t.nj = k1 - 1;
					if (k2 < 0) t.nk = normals.size() + k2; else    t.nk = k2 - 1;

					indices.push_back(t);
				}
				else {
					nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
					if (nn == 6) {
						if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
						if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
						if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
						if (j0 < 0) t.uvi = uvs.size() + j0; else   t.uvi = j0 - 1;
						if (j1 < 0) t.uvj = uvs.size() + j1; else   t.uvj = j1 - 1;
						if (j2 < 0) t.uvk = uvs.size() + j2; else   t.uvk = j2 - 1;
						indices.push_back(t);
					}
					else {
						nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
						if (nn == 3) {
							if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
							if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
							if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
							indices.push_back(t);
						}
						else {
							nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
							if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
							if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
							if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
							if (k0 < 0) t.ni = normals.size() + k0; else    t.ni = k0 - 1;
							if (k1 < 0) t.nj = normals.size() + k1; else    t.nj = k1 - 1;
							if (k2 < 0) t.nk = normals.size() + k2; else    t.nk = k2 - 1;
							indices.push_back(t);
						}
					}
				}

				
				consumedline = consumedline + offset;

				while (true) {
					if (consumedline[0] == '\n') break;
					if (consumedline[0] == '\0') break;
					nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
					TriangleIndices t2;
					t2.faceGroup = curGroup;
					if (nn == 3) {
						if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
						if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
						if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
						if (j0 < 0) t2.uvi = uvs.size() + j0; else  t2.uvi = j0 - 1;
						if (j2 < 0) t2.uvj = uvs.size() + j2; else  t2.uvj = j2 - 1;
						if (j3 < 0) t2.uvk = uvs.size() + j3; else  t2.uvk = j3 - 1;
						if (k0 < 0) t2.ni = normals.size() + k0; else   t2.ni = k0 - 1;
						if (k2 < 0) t2.nj = normals.size() + k2; else   t2.nj = k2 - 1;
						if (k3 < 0) t2.nk = normals.size() + k3; else   t2.nk = k3 - 1;
						indices.push_back(t2);
						consumedline = consumedline + offset;
						i2 = i3;
						j2 = j3;
						k2 = k3;
					}
					else {
						nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
						if (nn == 2) {
							if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
							if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
							if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
							if (j0 < 0) t2.uvi = uvs.size() + j0; else  t2.uvi = j0 - 1;
							if (j2 < 0) t2.uvj = uvs.size() + j2; else  t2.uvj = j2 - 1;
							if (j3 < 0) t2.uvk = uvs.size() + j3; else  t2.uvk = j3 - 1;
							consumedline = consumedline + offset;
							i2 = i3;
							j2 = j3;
							indices.push_back(t2);
						}
						else {
							nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
							if (nn == 2) {
								if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
								if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
								if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
								if (k0 < 0) t2.ni = normals.size() + k0; else   t2.ni = k0 - 1;
								if (k2 < 0) t2.nj = normals.size() + k2; else   t2.nj = k2 - 1;
								if (k3 < 0) t2.nk = normals.size() + k3; else   t2.nk = k3 - 1;
								consumedline = consumedline + offset;
								i2 = i3;
								k2 = k3;
								indices.push_back(t2);
							}
							else {
								nn = sscanf(consumedline, "%u%n", &i3, &offset);
								if (nn == 1) {
									if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
									if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
									if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
									consumedline = consumedline + offset;
									i2 = i3;
									indices.push_back(t2);
								}
								else {
									consumedline = consumedline + 1;
								}
							}
						}
					}
				}
				

			}


		}

		fclose(f);
	}

	void Generate_box()
	{
		bbox.bmin = vertices[0];
		bbox.bmax = vertices[0];

		for (int i = 0; i < vertices.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{

				bbox.bmin[j] = std::min(bbox.bmin[j], vertices[i][j]);
				bbox.bmax[j] = std::max(bbox.bmax[j], vertices[i][j]);
			}
		}
		cout << "bbox min" << bbox.bmin << endl;
		cout << "bbox max" << bbox.bmax << endl;
	}

	void add_texture(const char* filename) {

		textures.resize(textures.size() + 1);
		w.resize(w.size() + 1);
		h.resize(h.size() + 1);

		FILE* f;
		f = fopen(filename, "rb");
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

		w[w.size() - 1] = *(int*)&info[18]; // extract image height and width from header
		h[h.size() - 1] = *(int*)&info[22];

		int size = 3 * w[w.size() - 1] * h[h.size() - 1];
		textures[textures.size() - 1].resize(size); // allocate 3 bytes per pixel
		fread(&textures[textures.size() - 1][0], sizeof(unsigned char), size, f); // read the rest of the data at once
		fclose(f);

		for (int i = 0; i < size; i += 3) {
			std::swap(textures[textures.size() - 1][i], textures[textures.size() - 1][i + 2]);
		}
	}

};