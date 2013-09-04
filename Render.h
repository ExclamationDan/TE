#pragma once
#include <iostream>
#include "EngineModule.h"
#include "Shader.h"

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
		void PreDraw();
		void Draw();

		void Module_Poll();
	};

};