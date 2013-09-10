#include "Engine.h"

#include "Render.h"
#include "Resource.h"
#include "Console.h"



namespace TE
{

		namespace ConCommands
		{
			void Engine_Stop(CConCommand Info)
			{
				Engine.MainLoop_Stop();
			}
		}



	CEngine::CEngine(void)
	{
		Module_SetName("Engine");
		m_MainLoop_Run = true;

		Render =  new CRender();
		Resource =  new CResource();
		Console =  new CConsole();

		
		Console->Command_Reg(ConCommands::Engine_Stop,"Engine_Stop","Stop the engine's mainloop.");
		Console->Command_Reg(ConCommands::Engine_Stop,"Stop","Stop the engine's mainloop.");
	}


	CEngine::~CEngine(void)
	{
		if (Render)		{delete Render;}
		if (Resource)	{delete Resource;}
		if (Console)	{delete Console;}
	}

	bool CEngine::InMainLoop()
	{
		return m_MainLoop_Run;
	}
	void CEngine::MainLoop_Start()
	{
		Module_Log("Mainloop started.");
		m_MainLoop_Run = true;

		Console->Start();
		Render->Window_Begin();
		MainLoop();
	}
	void CEngine::MainLoop_Stop()
	{
		Console->Stop();

		m_MainLoop_Run = false;
		Module_Log("Mainloop stopped.");
	}
	void CEngine::MainLoop()
	{
		while(m_MainLoop_Run)
		{
			Resource->Module_Poll();
			Render->Module_Poll();
		}
	}




	CEngine Engine;
};