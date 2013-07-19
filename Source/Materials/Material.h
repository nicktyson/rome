#ifndef MATERIAL_H
#define MATERIAL_H

#include <gl\glew.h>

class ShaderProgram;
class MaterialProperties;

class Material {
public:
	Material();
	virtual void use() = 0;
	virtual void unuse() = 0;
	virtual void setUniforms(MaterialProperties* properties) = 0;
	virtual bool getHasTransparency();
	void setTransparencyUniforms(GLuint opaqueDepthBuffer, GLuint peelDepthBuffer, int passNumber);
protected:
	ShaderProgram* shader;
	bool hasTransparency;
};

#endif