//File: Shader.cpp
#include "Shader.h"
#include <iostream>



#include "Engine.h"
#include "Resource.h"
#include "Utility.h"

using namespace std;

namespace TE
{

	// |---------- class Shader ----------|
	Shader::Shader(void)
	{
	}
	Shader::~Shader(void)
	{
	}

	GLuint Shader::LoadShader(string File,GLenum Type)
	{
		GLuint	Shader;
		GLint	NoError;
		string	Path;
		string	Data;

		Path = Engine.Resource->AliasAdd(Path::Shaders,File);
		if (!Engine.Resource->File_Load(Path,Data))
		{
			Utility::Log("Shader: Failed to load ["+Path+"] from file.");
			return (GLuint) NULL;
		}

		Shader =	glCreateShader(Type);
		GLint		ShaderLength = Data.size();
		const char* ShaderSource = Data.c_str();

		glShaderSource(Shader,1,&ShaderSource,(GLint*) &ShaderLength);
		glCompileShader(Shader);

		glGetShaderiv(Shader,GL_COMPILE_STATUS,&NoError);
		if (NoError == GL_FALSE)
		{
			char Log[1024];
			glGetShaderInfoLog(Shader,1024,NULL,Log);
			Utility::Log("Shader: Failed to compile with the following error(s).\n"+(string)Log);
			glDeleteShader(Shader);
			return (GLuint) NULL;
		}

		Utility::Log("Shader: ["+File+"] Load complete.");
		return Shader;
	}
	GLuint Shader::LoadProgram(std::string Vertex_FilePath,std::string Fragment_FilePath)
	{
		GLuint	Vertex,Fragment,Program;
		GLint	NoError;

		Vertex		= Shader::LoadShader(Vertex_FilePath  ,GL_VERTEX_SHADER);
		Fragment	= Shader::LoadShader(Fragment_FilePath,GL_FRAGMENT_SHADER);

		if (Vertex == NULL || Fragment == NULL)
		{
			Utility::Log("Shader: Failed to create program, one or more shader files failed to load.");
			return (GLuint) NULL;
		}

		Utility::Log("Shader: Creating Program.");
		Program = glCreateProgram();

		glAttachShader(Program,Vertex);
		glAttachShader(Program,Fragment);
		glLinkProgram(Program);

		glGetProgramiv(Program,GL_LINK_STATUS,&NoError);
		if (NoError == GL_FALSE)
		{
			char Log[1024];
			glGetProgramInfoLog(Program,1024,NULL,Log);
			Utility::Log("Shader: Shader program link failed with error(s).\n"+(string)Log);
			glDeleteProgram(Program);
			return (GLuint) NULL;
		}
		Utility::Log("Shader: Program success.");
		return Program;
	}
	GLuint Shader::AttachToProgram(GLuint Vertex,GLuint Fragment)
	{
		GLint NoError;

		if (Vertex == NULL || Fragment == NULL)
		{
			Utility::Log("Shader: AttachToProgram() Failed, Vertex or Fragment equals NULL.");
			return (GLuint) NULL;
		}

		GLuint Program = glCreateProgram();
		glAttachShader(Program,Vertex);
		glAttachShader(Program,Fragment);
		glLinkProgram(Program);

		glGetProgramiv(Program,GL_LINK_STATUS,&NoError);
		if (NoError == GL_FALSE)
		{
			char Log[1024];
			glGetProgramInfoLog(Program,1024,NULL,Log);
			Utility::Log("Shader: Shader program link failed with error(s).\n"+(string)Log);
			glDeleteProgram(Program);
			return (GLuint) NULL;
		}
		Utility::Log("Shader: Program success.");
		return Program;
	}

};   