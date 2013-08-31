#pragma once
#include "EngineModule.h"
#include <iostream>
#include <vector>

namespace TE
{

	class CConCommand
	{
	public:
		typedef void (*CmdCallback)(CConCommand);

		CConCommand();
		CConCommand(CmdCallback Callback,std::string Name,std::string Description = "N\\A");

		std::string m_Name;
		std::string m_Description;

		std::vector<std::string> m_Args;

		void (*m_Func)(CConCommand);
		void CallCommand();
	};

	class CConsole :  public CEngineModule
	{
	public:
		
		
		bool m_Loop;
		std::vector<CConCommand> m_Commands;


		void Start();
		void Stop();

		void GetInput();

		bool Command_Exists(std::string);
		void Command_Reg(CConCommand Cmd);
		void Command_Reg(CConCommand::CmdCallback Callback,std::string Name,std::string Description = "N\\A");
		void Command_ParseString(std::string Potential_Command);
		CConCommand Command_GetConCommand(std::string Command);

		bool Command_Compare(std::string Str1, std::string Str2);

		CConsole(void);
		~CConsole(void);
	};

};