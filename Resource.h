//File:			Resource.h
//Purpose:		Prevent loading of resources already available in memory - handle loading and unloading of assets.
//Programmer:	Daniel Martin 4/21/2013

#pragma once
#include <iostream>
#include <map>
#include <list>
//#include "Model.h"
#include "Collada.h"
#include "EngineModule.h"
#include "GL_31.h"

#include "World.h"

namespace TE 
{



	namespace Path
	{
		const char Root[] = "Root";
		const char Data[] = "Data";
		const char Config[] = "Config";
		const char Models[] = "Models";
		const char Shaders[] = "Shaders";
		const char Materials[] = "Materials";
		const char Sound[] = "Sound";
	}

	class CResource : public CEngineModule
	{
	public:
		std::map<std::string,GL_31::Model> Models;
		std::map<std::string,GLuint> Textures;

		std::map<std::string,GLuint> Shaders;
		std::map<std::string,GLuint> Shader_Programs;

		CWorld m_World;

		//static Node Config;

		CResource(void);
		~CResource(void);

		GL_31::Model* LoadModel(std::string Path);
		GLuint LoadTexture(std::string Path);
		GLuint LoadShader(std::string FilePath, GLenum ShaderType);
		GLuint LoadShader_Program(std::string VertexFile, std::string FragmentFile);
		//Node   LoadConfig(std::string FileName);



		// FileSystem
		std::map<std::string,std::string> Keys;

		// Get the root, setup FileSystem defaults [Read from config file?]
		void	Setup_FileSystem(char* Root);

		std::string	AliasGet(std::string Alias); 
		std::string	AliasAdd(std::string Alias,std::string Path);
		void	AliasCreate(std::string Alias,std::string Path); // Make a new alias

		//FileSystem Utility
		std::string	DirFix(std::string Dir);				// Fix the backslash characters.
		bool	File_Exists(std::string Alias,std::string FileName);
		bool	File_Exists(std::string Path);
		bool	File_Load(std::string Path,std::string &Buffer );


	};

};
