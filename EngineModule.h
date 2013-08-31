// File:		EngineModule.h
// Purpose:		The base class for all CEngine modules. To provide a standard minimum requirement list of all CEngine modules.
// Programmer:	Daniel Martin 8/21/2013
#pragma once
#include <iostream>

namespace TE
{

	class CEngineModule
	{

	public:

		std::string m_Name;
		bool		m_Poll;

		virtual void	Module_Poll		();
		virtual void	Module_Print	(std::string Text);
		virtual void	Module_Log		(std::string Text);
		void			Module_SetName	(std::string Name);

		CEngineModule(void);
		~CEngineModule(void);
	};

};