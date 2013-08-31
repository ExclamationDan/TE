//File:	Model.cpp
#include "Model.h"
#include "Utility.h"

using namespace std;

namespace TE
{

// |---------- class SimpleVertex ----------|
SimpleVertex::SimpleVertex(GLfloat X,GLfloat Y,GLfloat Z)
{
	x = X;
	y = Y;
	z = Z;
}

void SimpleVertex::Print()
{
	cout << "Vertex: ( "<<x<<", "<<y<<", "<<y<<" )\n";
}


// |---------- class Vertex ----------|
Vertex::Vertex()
{
	X = Y = Z = Nx = Ny = Nz = S = T = 0;
}

Vertex ::Vertex(float x, float y,float z,float nx,float ny,float nz, float s, float t)
{
	X = x;
	Y = y;
	Z = z;

	Nx = nx;
	Ny = ny;
	Nz = nz;

	S = s;
	T = t;
}

void Vertex ::PrintVertex()
{
	cout << "Vertex: ("<<X<<", "<<Y<<", "<<Y<<" | "<<Nx<<", "<<Ny<<", "<<Nz<<" | "<<S<<", "<< T <<" )\n";
}

SimpleVertex Vertex::ToSimpleVertex(int index)
{
	if (index == 0)
	{
		return SimpleVertex(X,Y,X);
	}

	else if (index == 1)
	{
		return SimpleVertex(Nx,Ny,Nz);
	}

	else 
	{
		return SimpleVertex(S,T,0);
	}



}



// |---------- class Mesh ----------|
Mesh::Mesh()
{
	m_nVBO = 0;
	m_TBO = 0;
}
Mesh::~Mesh() 
{

}

void Mesh::Upload()
{
	//glGenBuffers(1, &m_nVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_Interleaved.size(), &m_Interleaved[0].X, GL_STATIC_DRAW);
}

void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, sizeof(SimpleVertex), 0, &m_Interleaved[0].x);


	for (auto it = m_Interleaved.begin(); it != m_Interleaved.end(); it++)
	{
		it->Print();
	}


	//glDrawArrays(GL_TRIANGLES,0,6);
	glDisableClientState(GL_VERTEX_ARRAY);


	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &Position[0]);
	glNormalPointer(GL_FLOAT, 0, &Normal[0]);
	glTexCoordPointer(2,GL_FLOAT,0,&Texcoord[0]);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_SHORT, &Indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	*/

}
void Mesh::Draw_VBO()
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer( GL_ARRAY_BUFFER_ARB, m_nVBO );
	//glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
	//glDrawArrays( GL_TRIANGLES, 0, m_Position.size() );  
	//glDisableClientState( GL_VERTEX_ARRAY );


	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); 
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(24));

	glDrawArrays( GL_TRIANGLES, 0, m_Interleaved.size() );  

}


// |---------- class Model ----------|
Model::Model()
{
	m_TBO = 0;
	m_Mesh = std::vector<Mesh>(0);
}
Model::~Model()
{

}

void Model::Draw()
{
	glBindTexture(GL_TEXTURE_2D, m_TBO);

	if (m_Mesh.size() > 0)
	{
		for (auto it = m_Mesh.begin();it != m_Mesh.end();it++)
		{
			//it->Draw_VBO();
			it->Draw();
		}
	}
	else
	{
		cout << "Model: Mesh list empty, draw failure!\n";
	}
}
};