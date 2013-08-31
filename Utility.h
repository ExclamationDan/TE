//File:			Utility.h
//Purpose:		To provide a simple set of utility methods via a 'static' class, for use within the Engine
//Programmer:	Daniel Martin 4/22/2013

#pragma once
#include <iostream>
#include <Vector>
#include <sstream>
#include <algorithm>


namespace TE
{

	class Utility
	{

	public:

		static void			Log(std::string str, bool WriteToLogFile = true);

		static int			ToInt(std::string Str);
		static float		ToFloat(std::string Str);
		static unsigned int ToUInt(std::string Str);

		static std::string	ToString(int Type);
		static std::string	ToString(float Type);
		static std::string	ToString(std::string Type);
		static std::string	ToUpper(std::string str);

		static bool			CompareString(std::string Str1,std::string Str2);

		static std::string Trim_Whitespace(std::string Str);

		static std::vector<std::string>	Vector_SplitToString(char* Text, int VectorSize = 0);


		template <typename T>
		static std::vector<T>	Vector_SplitString(char* Text, int VectorSize = 0)
		{
			if (Text == NULL)		{return std::vector<T>(0);}
			std::vector<T> Ret;
			if (VectorSize != 0)	{Ret.reserve(VectorSize);}


			std::string str(Text);
			for (unsigned int i = 0; i < str.length();i++) 
			{
				if (str[i] == '\n')
				{
					str[i] = ' ';
				}
			}


			std::string token; 
			std::istringstream iss(str);

			while(getline(iss, token, ' '))
			{
				token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
				if (token != "")
				{
					Ret.push_back(ToFloat(token));
				}
			}

			Ret.shrink_to_fit();
			return Ret;
		}

		template <typename T>
		static void	PrintVector(std::vector<T> V)
		{
			for (auto it = V.begin(); it != V.end();it++)
			{
				std::cout << *it << " ";
			}
			std::cout << endl;
		}

		template <typename T>
		static std::vector<T> CombineVector(std::vector<T> V1,std::vector<T> V2)
		{
			V1.insert(V1.end(),V2.begin(),V2.end());
			return V1;
		}


	};


};