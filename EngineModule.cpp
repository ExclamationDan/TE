#include "EngineModule.h"
#include "Utility.h"

namespace TE
{

	CEngineModule::CEngineModule(void)
	{
		m_Poll = true;
		m_Name = "Unnamed CEngineModule";
	}
	CEngineModule::~CEngineModule(void)
	{
		Module_Log("Module destructed.");
	}

	void CEngineModule::Module_Poll()
	{
		if (m_Poll)
		{
			Utility::Log(m_Name +": Virtual Poll() function undefined!");
			m_Poll = false;
		}
	}

	void CEngineModule::Module_Print(std::string Text)
	{
		std::cout << m_Name << ": " << Text << std::endl;
	}

	void CEngineModule::Module_Log(std::string Text)
	{
		Utility::Log(m_Name +": "+Text+"\n");
	}

	void CEngineModule::Module_SetName(std::string Name)
	{
		m_Name = Name;
	}
};
