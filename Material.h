#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
public:
	Vector Color;
	bool isMirror;
	double Optic_indice;

	Material() {};
    Material(Vector color, bool ismirror, double optic_indice) 
	{
		Color = color;
		isMirror =  ismirror;
		Optic_indice = optic_indice;
    }; 

};

#endif // MATERIAL_H
