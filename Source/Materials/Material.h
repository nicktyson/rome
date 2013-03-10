#ifndef MATERIAL_H
#define MATERIAL_H

class ShaderProgram;
class MaterialProperties;

class Material {
public:
	Material();
	virtual void use() = 0;
	virtual void unuse() = 0;
	virtual void setUniforms(MaterialProperties* properties) = 0;
protected:
	ShaderProgram* shader;
};

#endif