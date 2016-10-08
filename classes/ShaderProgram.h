#pragma once
#include <stdio.h>
#include "Vector.hpp"
#include "RGBImage.hpp"
#include "Color.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "Matrix.h"
#include <vector>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>


class ShaderProgram{
    
public:
    ShaderProgram();
    ShaderProgram(const std::string& pFilename);
	~ShaderProgram();
    bool load(const char* VertexShader, const char* FragmentShader);
	bool loadVertexShader(const char* VertexShader);
	bool loadFragmentShader(const char* FragmentShader);
    bool compile(std::string* CompileErrors = NULL);
    bool compileShader(GLuint shader, std::string* CompileErrors);

	GLint getParameterID(const char* ParameterName) const;

	void setParameter(GLint ID, float Param);
	void setParameter(GLint ID, int Param);
	void setParameter(GLint ID, const Vector& Param);
	void setParameter(GLint ID, const Color& Param);
	void setParameter(GLint ID, const Matrix& Param);
    GLuint createShader(const std::string& text, GLenum shadeType);
    

	void activate() const;
	void deactivate() const;

private:
    const char* readFile(const char *fileName);
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_ShaderProgram;
    ShaderProgram(ShaderProgram& pShaderProgramm){ }
    void operator=(const ShaderProgram& other){ }
};

