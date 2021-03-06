//File: Resource.cpp



#include "Resource.h"
#include "Engine.h"

#include <fstream>
#include <sstream>
#include "Shader.h"
#include "Utility.h"

#include "TGA.h"

#include "GL_31.h"

namespace TE
{



	using namespace std;

	// |------- class Resource -------|



	CResource::CResource(void)
	{
		Module_SetName("Resource");
	}
	CResource::~CResource(void){}


	GL_31::Model* CResource::LoadModel(string Path)
	{
		auto Find = Models.find(Path);

		if (Find == Models.end())
		{
			Utility::Log("CResource: Allocating new [Model] CResource: '"+Path+"'");

			Collada_Loader C;

			Models.insert(Models.end(),pair<string,GL_31::Model>(Path,C.Load(Path)));
			return LoadModel(Path);
		}
		else
		{
			return &Find->second;
		}
	}
	GLuint CResource::LoadShader(std::string Path, GLenum ShaderType)
	{
		auto Find = Shaders.find(Path);
		if (Find == Shaders.end())
		{
			Utility::Log("CResource: Allocating new [Shader] CResource: '"+Path+"'");

			GLuint	Shader		=	Shader::LoadShader(Path,ShaderType);
			pair<string,GLuint>		Pair(Path,Shader);
			Shaders.insert(Shaders.end(),Pair);

			return LoadShader(Path,ShaderType);
		}
		else
		{
			return Find->second;
		}
	}
	GLuint CResource::LoadShader_Program(std::string VertexFile, std::string FragmentFile)
	{
		// We have to make a name that is unique but avoids loading the same files over and over.
		// To do this, we just combine the two file names, making a unique-only-to-those-files name.

		string	Name = VertexFile+FragmentFile;
		auto	Find = Shaders.find(Name);

		if (Find == Shaders.end())
		{
			GLuint	VertexShader	= CResource::LoadShader		(VertexFile	 ,	GL_VERTEX_SHADER);
			GLuint	FragmentShader	= CResource::LoadShader		(FragmentFile,	GL_FRAGMENT_SHADER);
			GLuint	Program			= Shader::AttachToProgram	(VertexShader,	FragmentShader);

			if (Program == NULL)
			{
				Utility::Log("Resource: LoadShader_Program() Failed, Shader::AttachToProgram() returned NULL program.");
				return (GLuint) NULL;
			}
			pair<string,GLuint>		Pair(Name,Program);
			Shaders.insert(Shaders.end(),Pair);
			return Program;
		}
		else
		{
			return Find->second;
		}

	}
	GLuint CResource::LoadTexture(std::string Name)
	{		

		Module_Log("Texture loading is currently unavailable!");

		return -1;

		/*
		//std::cout << "Path: " << AliasGet(Path::Materials) << "\n\n\n\n\n";
		std::string Path = AliasAdd(Path::Materials,Name);
		auto Find = Textures.find(Path);
		if (Find == Textures.end())
		{		
			GLuint  m_TexID;
			//CTGA TGA;
			//if (!TGA.Load((char*)Path.c_str()))

			if (glfwLoadTexture2D(Path.c_str(),GLFW_BUILD_MIPMAPS_BIT))
			{
				Module_Log("Failed to load texture ["+Name+"], ignoring.");
				return 0;
			}
			else
			{
				Module_Print("Loaded texture ["+Name+"] successfully!");
			}


			glGenTextures(1, &m_TexID);
			glBindTexture(GL_TEXTURE_2D, m_TexID);
			
			
			

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TGA.imageWidth, TGA.imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TGA.imageData);

			

			Module_Log("Allocating new [Texture] resource: "+Path);

			Textures.insert(Textures.end(),pair<string,GLuint>(Path,m_TexID));

			return m_TexID;
		}
		else
		{
			return Find->second;
		}

		*/
	}

	/*
	Node CResource::LoadConfig(string FileName)
	{

	return Collada::Load(AliasAdd(Path::Config,FileName));
	}
	*/

	// |---------- Resource::FileSystem ----------|

	void	CResource::Setup_FileSystem(char* Root)
	{	
		std::string directory = Root;

		Keys[Path::Root]      = directory.substr(0,directory.find_last_of("/\\"));
		Keys[Path::Data]      = AliasAdd(Path::Root,Path::Data);
		Keys[Path::Sound]     = AliasAdd(Path::Data,Path::Sound);
		Keys[Path::Config]    = AliasAdd(Path::Data,Path::Config);
		Keys[Path::Models]    = AliasAdd(Path::Data,Path::Models);
		Keys[Path::Shaders]   = AliasAdd(Path::Data,Path::Shaders);
		Keys[Path::Materials] = AliasAdd(Path::Data,Path::Materials);

		//cout << (Paths[Root] += "Test") << endl;
	}

	void	CResource::AliasCreate(string Alias, string Path)
	{
		Keys[Alias] = Path; // += Key(Alias,Path);
	}
	string	CResource::AliasGet(string Alias)
	{
		return Keys[Alias];
	}
	string	CResource::AliasAdd(string Alias, string Path)
	{
		return DirFix(AliasGet(Alias)+"/"+Path);
	}

	string  CResource::DirFix(string Path) 
	{
		// Sometimes when merging paths, back/forward slashes will be duplicated or mixed; this function formats them.

		string Token1 = "\\"; // Escape Char 
		string Token2 = "//"; // Normal
		unsigned int Pos1, Pos2;

		do
		{
			Pos1 = Path.find(Token1);
			if (Pos1!=-1)  {Path.replace(Pos1, Token1.length(), "/");}

			Pos2 = Path.find(Token2);
			if (Pos2!=-1)  {Path.replace(Pos2, Token2.length(), "/");}
		}
		while( ((Pos1 != string::npos) || (Pos2 != string::npos) ));


		return Path;

	}

	bool CResource::File_Exists(string Alias,string FileName)
	{
		string Path = AliasAdd(Alias,FileName);

		ifstream File;
		File.open(Path);
		if (File.is_open())
		{	
			File.close();
			return true;
		}
		else
		{
			return false;
		}
	}
	bool CResource::File_Exists(string Path)
	{
		ifstream File;
		File.open(Path);
		if (File.is_open())
		{	
			File.close();
			return true;
		}
		else
		{
			return false;
		}
	}
	bool CResource::File_Load(string Path,string &Buffer )
	{
		if (File_Exists(Path) == true)
		{

			ifstream File;
			File.open(Path);
			if(!File.is_open())
			{
				Utility::Log("FileSystem: Failed to read file ["+Path+"]");
				return false;
			}

			stringstream Data;
			Data << File.rdbuf();
			File.close();
			Buffer = Data.str();
			return true;
		}
		else
		{
			Utility::Log("FileSystem: File ["+Path+"] does not exist.");
			return false;
		}
	}


};