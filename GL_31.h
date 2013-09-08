// File: GL_31.h
//Purpose: To create a class wrapper for needed OpenGL functions. VertexBuffer ... etc.
// Programmer: Daniel Martin 8/24/2013

#include "GL\glew.h"
#include <iostream>
#include <vector>
#include "glm\glm.hpp"

#include "Utility.h"
//using namespace glm;

namespace TE
{
	namespace GL_31
	{
#define BUFFER_OFFSET(i) ((void*)(i))

		class Triangle
		{
		public:
			glm::vec3 m_Vertices[3];

			Triangle(glm::vec3 V1,glm::vec3 V2,glm::vec3 V3)
			{
				m_Vertices[0] = V1;
				m_Vertices[1] = V2;
				m_Vertices[2] = V3;
			}

			Triangle() {};
		};


		class Square
		{
		public:

			Triangle m_Triangles[2];
			glm::vec3 m_Pos;

			void SetSize(float Size)
			{

				Triangle Left;
				//Upper Left A
				Left.m_Vertices[0] = glm::vec3(0,Size,0);
				Left.m_Vertices[0].s = 0;
				Left.m_Vertices[0].t = 1;

				//Lower Left B
				Left.m_Vertices[1] = glm::vec3(0,0,0);
				Left.m_Vertices[1].s = 0;
				Left.m_Vertices[1].t = 0;

				//  Left Far right C
				Left.m_Vertices[2] = glm::vec3(Size,0,0);
				Left.m_Vertices[2].s = 1;
				Left.m_Vertices[2].t = 0;

				m_Triangles[0] = Left;



				Triangle Right;				

				//  Left Far right C
				Right.m_Vertices[0] = glm::vec3(0,Size,0);
				Right.m_Vertices[0].s = 0;
				Right.m_Vertices[0].t = 1;

				//Lower Left B
				Right.m_Vertices[1] = glm::vec3(Size,0,0);
				Right.m_Vertices[1].s = 1;
				Right.m_Vertices[1].t = 0;

				//Upper Left A
				Right.m_Vertices[2] = glm::vec3(Size,Size,0);
				Right.m_Vertices[2].s = 1;
				Right.m_Vertices[2].t = 1;

				m_Triangles[1] = Right;
			}

			void Draw()
			{
				//SetSize(5);

				glBegin(GL_TRIANGLES);
				for (int t = 0; t < 2; t++)
				{
					for (int v = 0; v < 3; v++)
					{glTexCoord2f(m_Triangles[t].m_Vertices[v].s,m_Triangles[t].m_Vertices[v].t);
						glVertex3f(m_Triangles[t].m_Vertices[v].x,m_Triangles[t].m_Vertices[v].y,m_Triangles[t].m_Vertices[v].z);
						
					}
				}
				glEnd();

			}

		};


		class Sprite
		{
		public:
			Triangle m_Triangles[2];
			glm::vec3 m_Pos;

			Sprite()
			{

			}
			Sprite(glm::vec3 Pos, float  size)
			{
				m_Pos = Pos;

			}

			std::vector<Triangle> CreateSquare(float size)
			{

			}

		};


		class VertexBuffer
		{
		public:
		;

			GLuint m_nVBO,m_nUBO;
			unsigned int m_VertexCount;

			VertexBuffer()
			{
				m_nVBO = 0;
			}


			void Upload(std::vector<glm::vec3> m_Buffer)
			{
				glGenBuffers(1, &m_nVBO);
				glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*m_Buffer.size(), &m_Buffer[0].x, GL_STATIC_DRAW);
			}


			void Draw()
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, sizeof(glm::vec3), 0,0);
				glDisableClientState(GL_VERTEX_ARRAY);

				glDrawElements(GL_TRIANGLES, m_VertexCount, GL_UNSIGNED_BYTE, 0);
			}



			void Draw_VBO()
			{
				//glEnableClientState(GL_VERTEX_ARRAY);
				//glBindBuffer( GL_ARRAY_BUFFER_ARB, m_nVBO );
				//glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
				//glDrawArrays( GL_TRIANGLES, 0, m_Position.size() );  
				//glDisableClientState( GL_VERTEX_ARRAY );

				glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3),0); 
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, sizeof(glm::vec3), BUFFER_OFFSET(12));
				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(glm::vec3), BUFFER_OFFSET(24));

				//glDrawArrays( GL_TRIANGLES, 0, m_Buffer.size() );  

			}

		};


		static void TestChop(std::string RawData)
		{
			std::vector<glm::vec3> Position_Vectors;

			//Refined data is the sequence string of "1 2 3 4 ..." to a vector of type GLfloat of [1,2,3,4,...]
			std::vector<GLfloat> RefinedData = Utility::Vector_Strtok<GLfloat>((char*) RawData.c_str());


			for (size_t Vec3_Elements = 0; Vec3_Elements < RefinedData.size()/3;Vec3_Elements++)
			{
				Position_Vectors.push_back(glm::vec3(RefinedData[Vec3_Elements*3],RefinedData[Vec3_Elements*3+1],RefinedData[Vec3_Elements*3+2]));
			}

			std::cout << "Printing Complete Vector Data...\n\n";
			for (auto it : Position_Vectors)
			{
				std::cout << it.x << ","<<it.y<<","<<it.z<<"\n";
			}
			std::cout << "\nComplete Vector Data. Print Complete.\n\n";

		}







	};
};