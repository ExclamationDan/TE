#pragma once

#include <iostream>
#include "GL_31.h"

#include <vector>

namespace TE
{


	class CWorld
	{
	public:
		CWorld(void);
		~CWorld(void);

		std::vector<GL_31::Model> Models;
		void World_Load(std::string Name);
	};

};