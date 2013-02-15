#ifndef MATERIAL_H
#define MATERIAL_H

class ShaderProgram;

class Material {
public:
	Material();
	virtual void use() = 0;
	virtual void unuse() = 0;
protected:
	ShaderProgram* shader;
};

#endif