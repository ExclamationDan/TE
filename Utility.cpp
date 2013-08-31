//File:	Utility.cpp
#include "Utility.h"
#include <fstream>
#include <time.h>
#include <cctype>

namespace TE
{


	// |------- class Utility -------|
	void Utility::Log(std::string str, bool WriteToLogFile)
	{
		std::cout << str << std::endl;

		if (WriteToLogFile)
		{
			time_t rawtime;
			struct tm * timeinfo;
			char buffer [50];

			time (&rawtime);
			timeinfo = localtime (&rawtime);
			strftime (buffer,80,"[%b|%d - %I:%M%p] ",timeinfo);


			std::ofstream File;
			File.open("Log.txt",std::ios::app);
			if (File.is_open())
			{
				File <<buffer << str << std::endl;
			}
			File.close();
		}
	}

	int				Utility::ToInt(std::string Str)
	{
		return atoi(Str.c_str());
	}
	float			Utility::ToFloat(std::string Str) 
	{
		return (float) atof(Str.c_str());
	}
	unsigned int	Utility::ToUInt(std::string Str)
	{
		return (unsigned int) atoi(Str.c_str());
	}

	std::string		Utility::ToString(int Type)
	{
		std::string s;
		std::stringstream str;
		str << Type;
		str >> s;
		return s;
	}
	std::string		Utility::ToString(float Type)
	{
		std::string s;
		std::stringstream str;
		str << Type;
		str >> s;
		return s;
	}
	std::string		Utility::ToString(std::string Type)
	{
		return Type;
	}
	std::string		Utility::ToUpper(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::toupper);
		return str;
	}

	bool		Utility::CompareString(std::string Str1,std::string Str2)
	{
		Str1 = ToUpper(Str1);
		Str2 = ToUpper(Str2);
		return  Str1.compare(Str2) == 0 ? true : false;
	}

	std::string Utility::Trim_Whitespace(std::string Str)
	{
		Str.erase(std::remove_if(Str.begin(),Str.end(),isspace));
		return Str;
	}

	std::vector<std::string>	Utility::Vector_SplitToString(char* Text, int VectorSize)
	{
		if (Text == NULL) {return std::vector<std::string>(0);}

		std::vector<std::string> Ret;
		if (VectorSize != 0) 
		{
			Ret.reserve(VectorSize);
		}

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
				Ret.push_back(token);
			}
		}

		Ret.shrink_to_fit();
		return Ret;
	}
};