//File:			Collada.h
//Purpose:		Handle loading Collada .DAE model files in addition to loading basic XML files for general parsing using the Node Class
//Programmer:	Daniel Martin 3/19/2013 

#pragma once
#include <iostream>
#include <list>
//#include "Model.h"
#include "GL_31.h"

#include "TinyXML\tinyxml2.h"
using namespace tinyxml2;


namespace TE
{

		class Collada_Loader;

		class Collada_Node
		{
		public:
			Collada_Loader*		m_Root;

			// Technically called attributes, but "keys" makes shorter looking methods
			struct Key
			{
				std::string m_Name;
				std::string m_Value;
				Collada_Node* m_ptr;

				Key()
				{
					m_Name = "NULL";
					m_Value = "NULL";
					m_ptr = NULL;
				}
				Key(std::string _Name,std::string _Value)
				{
					m_Name = _Name;
					m_Value = _Value;
					m_ptr = NULL;
				}
			};	

			std::string			m_Name;
			std::list<Collada_Node>		m_Nodes;
			std::vector<Key>	m_Keys;
			Collada_Node* m_Parent, *m_Next, *m_Prev;

			Collada_Node();
			Collada_Node(std::string _Name);
			void Create(std::string _Name = "NULL");

			Collada_Node& CreateSubNode();
			void AddKey(std::string Name,std::string Value);
			void AddKey(Key key);


			// Recursively parse all node children for matching name
			Collada_Node* GetAnyNode(std::string Name, bool Subprocess = false);

			// Recursively parse all node children keys and self
			Key* GetAnyKey(std::string Name, bool Subprocess = false);


			// Parse only immediate child nodes for matching name
			Collada_Node* GetNode(std::string Name);

			std::vector<Collada_Node*> GetNodes(std::string Name);

			// Parse only immediate node children keys and self
			Key* GetKey(std::string Name);

			// Iterate Node->Next pointers until a node with a matching name as this->m_Name is found.
			Collada_Node* GetSimilar(std::string Name);

			void MapKeys();
			void MapNodes();

			// Print a list of child nodes and attributes (keys)
			void PrintMap();
		};

		class Collada_Loader : public Collada_Node
		{
		public:		


			//Global scope node Lookup. If it has an ID, it goes here.
			std::vector<std::pair<std::string,Collada_Node*>> m_IDLookup;		

			void					ID_Add(std::string ID,Collada_Node* Ptr);
			Collada_Node*					ID_Get(std::string Name);
			std::pair<bool,Collada_Node*>	ID_Find(std::string Name);


			Collada_Loader(void);

			static void ParseKeys(XMLElement* XML, Collada_Node* N);
			static void ParseSibling(XMLElement* XML, Collada_Node* Parent);

			GL_31::Model Load(std::string Path);
			GL_31::Model LoadModel(std::string path);

		};
}



