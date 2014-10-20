#pragma once


#include "GL_31.h"
#include "EngineModule.h"
#include "Shader.h"
#include <iostream>




namespace TE
{


	class CRender : public CEngineModule 
	{
	public:

		GLuint m_CurrentShader;

		CRender();
		~CRender();

		void Window_Begin();
		void Window_End();
		void UseShader(GLuint Shader);
		inline void PreDraw();
		inline void Draw();

		void Module_Poll();
	};

};