//File:			Shader.h
//Purpose:		To handle binding, compiling, and using OpenGL-GLSL shader programs
//Programmer:	Daniel Martin 4/20/2013

#pragma once

#include <iostream>
#include "GL_31.h"

namespace TE
{

	class Shader
	{
	public:

		static GLuint LoadShader(std::string FilePath,GLenum Shader_Type);
		static GLuint LoadProgram(std::string Vertex_FilePath,std::string Fragment_FilePath);

		static GLuint AttachToProgram(GLuint Vertex,GLuint Fragment);

		Shader(void);
		~Shader(void);
	};

};