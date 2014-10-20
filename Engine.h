#pragma once


#include "EngineModule.h"
#include "GL_31.h"

namespace TE
{

	class CRender;
	class CResource;
	class CConsole;

	class CEngine : public CEngineModule
	{		
		bool m_MainLoop_Run;
	public:

		CRender		*Render;
		CResource	*Resource;
		CConsole	*Console;
		GLFWwindow	*Window;

		void MainLoop_Start	();
		void MainLoop_Stop	();
		void MainLoop		();
		bool InMainLoop		();

		CEngine(void);
		~CEngine(void);
	};

	extern CEngine Engine;
}