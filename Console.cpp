#include "Console.h"
#include <sstream>
#include <future>
#include <algorithm>

#include "Utility.h"

#include "Engine.h"

namespace TE
{


	// |------- class CConsoleCommand -------| 

	namespace ConCommands
	{

		void ListCommands(CConCommand Info)
		{
			std::cout << "Listing commands...\n\n";
			for ( auto it : Engine.Console->m_Commands)
			{
				std::cout << it.m_Name << " | " << it.m_Description << "\n";
			}
			std::cout << "\nCommand list complete...\n\n";
		}

		void Help(CConCommand Info)
		{
			ListCommands(Info);
		}



	};
	CConCommand::CConCommand()
	{
		m_Description = "N\\A";
		m_Func = NULL;
		m_Name = "INVALID";
	}
	CConCommand::CConCommand(CmdCallback Callback,std::string Name,std::string Description)
	{
		m_Func = Callback;
		m_Name = Name;
		m_Description = Description;
	}

	void CConCommand::CallCommand()
	{
		if (m_Func != NULL)
		{
			m_Func(*this);
		}
		else
		{
			Engine.Console->Module_Log("Command \""+m_Name+"\" has invalid function pointer!");
		}
	}

	// --------------------------------------


	// |------- class CConsole -------| 

	CConsole::CConsole(void)
	{
		Module_SetName("Console");
		m_Loop = true;
		Command_Reg(ConCommands::ListCommands,"ListCommands","Lists all registered commands.");
		Command_Reg(ConCommands::Help,"Help","Lists all registered commands.");
	}


	CConsole::~CConsole(void)
	{
		Stop();
	}

	void CConsole::Start()
	{
		m_Loop = true;
		std::async(std::launch::async,&CConsole::GetInput, this);
	}
	void CConsole::Stop()
	{
		m_Loop = false;
	}

	void CConsole::GetInput()
	{
		while(m_Loop)
		{
			std::string RawInput;
			std::getline(std::cin,RawInput);


			Module_Print(RawInput);
			Command_ParseString(RawInput);

			std::async(std::launch::async,&CConsole::GetInput, this);
		}
	}

	bool CConsole::Command_Exists(std::string Cmd)
	{
		for (auto it : m_Commands)
		{
			if (Command_Compare(Cmd,it.m_Name))
			{
				return true;
			}
		}

		return false;

	}
	void CConsole::Command_Reg(CConCommand Cmd)
	{
		Cmd.m_Name = Utility::Trim_Whitespace(Cmd.m_Name);

		if (Command_Exists(Cmd.m_Name))
		{
			Module_Print("Command /"+Cmd.m_Name+"\" already exists.");
			return;
		}

		m_Commands.push_back(Cmd);
	}
	void CConsole::Command_Reg(CConCommand::CmdCallback Callback,std::string Name,std::string Description)
	{
		Name = Utility::Trim_Whitespace(Name);

		if (Command_Exists(Name))
		{
			Module_Print("Command /"+Name+"\" already exists.");
			return;
		}

		m_Commands.push_back(CConCommand(Callback,Name,Description));
	}

	CConCommand CConsole::Command_GetConCommand	(std::string Cmd)
	{
		for (auto it : m_Commands)
		{
			if  (Command_Compare(Cmd,it.m_Name))
			{
				return it;
			}
		}
		Module_Log("Failed to GetConCommand(\""+Cmd+"\")!");
		return CConCommand();
	}
	void		CConsole::Command_ParseString	(std::string Command)
	{
		std::vector<std::string> Args = Utility::Vector_SplitToString((char*) Command.c_str());

		if(!Command_Exists(Args[0]))
		{
			Module_Print("Command \""+Args[0]+"\" does not exist.");
			return;
		}

		CConCommand Cmd = Command_GetConCommand(Args[0]);
		Cmd.m_Args = Args;
		Cmd.CallCommand();
	}
	bool		CConsole::Command_Compare		(std::string Str1, std::string Str2)
	{
		Str1 = Utility::ToUpper(Utility::Trim_Whitespace(Str1));
		Str2 = Utility::ToUpper(Utility::Trim_Whitespace(Str2));

		if (Str1.compare(Str2) == 0)
		{
			return true;
		}
		return false;

	}

};