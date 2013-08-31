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


		class VertexBuffer
		{
		public:
			std::vector<glm::vec3> m_Buffer;

			GLuint m_nVBO;
			GLuint m_TBO;

			VertexBuffer()
			{
				m_nVBO = 0;
				m_TBO = 0;
			}


			void Upload()
			{
				glGenBuffers(1, &m_nVBO);
				glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*m_Buffer.size(), &m_Buffer[0].x, GL_STATIC_DRAW);
			}


			void Draw()
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, sizeof(glm::vec3), 0, &m_Buffer[0].x);
				glDisableClientState(GL_VERTEX_ARRAY);

				glDrawArrays( GL_TRIANGLES, 0, m_Buffer.size() );  
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

				glDrawArrays( GL_TRIANGLES, 0, m_Buffer.size() );  

			}

		};


		static void TestChop(std::string RawData)
		{
			std::vector<glm::vec3> Position_Vectors;

			//Refined data is the sequence string of "1 2 3 4 ..." to a vector of type GLfloat of [1,2,3,4,...]
			std::vector<GLfloat> RefinedData = Utility::Vector_SplitString<GLfloat>((char*) RawData.c_str());


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