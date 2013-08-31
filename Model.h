//Name:    Model.h
//Purpose:	Uniform system, independent of model loaders. Made as engine wide model format
//Creator:	Daniel Martin 10/9/2012

#pragma once
#include "GL\glew.h"
#include <iostream>
#include <vector>



#define BUFFER_OFFSET(i) ((void*)(i))

namespace TE
{

	class SimpleVertex
	{
	public:
		GLfloat x,y,z;

		SimpleVertex(GLfloat X,GLfloat Y,GLfloat Z);
		void Print();
	};

	class Vertex 
	{
	public:
		float X,Y,Z,Nx,Ny,Nz,S,T;

		Vertex();
		Vertex(float x, float y,float z,float nx,float ny,float nz, float s, float t);

		void PrintVertex();

		SimpleVertex ToSimpleVertex(int index);


	};

	class Mesh
	{
	public:
		std::vector<SimpleVertex> m_Interleaved;
		GLuint m_nVBO;
		GLuint m_TBO;

		//Fallback
		std::vector<GLfloat> Position,Normal,Texcoord;

		std::vector<GLushort> Indices;
		std::vector<GLfloat> RawVertex;

		Mesh();
		~Mesh();

		void Upload();

		void Draw();
		void Draw_VBO();

	};

	class Model
	{
	public:
		std::vector<Mesh> m_Mesh;
		GLuint m_TBO;

		Model();
		~Model();

		void Draw();

	};

}