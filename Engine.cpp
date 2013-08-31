#include "Engine.h"

#include "Render.h"
#include "Resource.h"
#include "Console.h"



namespace TE
{

	CEngine::CEngine(void)
	{
		Module_SetName("Engine");
		m_MainLoop_Run = true;

		Render =  new CRender();
		Resource =  new CResource();
		Console =  new CConsole();
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