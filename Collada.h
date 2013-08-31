//File:			Collada.h
//Purpose:		Handle loading Collada .DAE model files in addition to loading basic XML files for general parsing using the Node Class
//Programmer:	Daniel Martin 3/19/2013 

#pragma once
#include <iostream>
#include <list>
#include "Model.h"
#include "TinyXML\tinyxml2.h"
using namespace tinyxml2;

namespace TE
{




	class Node
	{
		//Global scope node Lookup. If it has an ID, it goes here.		
		//ONLY Root can use m_IdLookup
		std::vector<std::pair<std::string,Node*>> m_IDLookup;		
		Node*		m_Root;
		bool		IsRoot(); 
	public:

		// Technically called attributes, but "keys" makes shorter looking methods
		struct Key
		{
			std::string m_Name;
			std::string m_Value;
			Node* m_ptr;

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
		std::list<Node>		m_Nodes;
		std::vector<Key>	m_Keys;
		Node* m_Parent, *m_Next, *m_Prev;


		Node();
		Node(std::string _Name);
		void Create(std::string _Name = "NULL");

		void					ID_Add(std::string ID,Node* Ptr);
		Node*					ID_Get(std::string Name);
		std::pair<bool,Node*>	ID_Find(std::string Name);

		Node* CreateSubNode();
		void AddKey(std::string Name,std::string Value);
		void AddKey(Key key);


		// Recursively parse all node children for matching name
		Node* GetAnyNode(std::string Name, bool Subprocess = false);

		// Recursively parse all node children keys and self
		Key* GetAnyKey(std::string Name, bool Subprocess = false);


		// Parse only immediate child nodes for matching name
		Node* GetNode(std::string Name);

		// Parse only immediate node children keys and self
		Key* GetKey(std::string Name);

		// Iterate Node->Next pointers until a node with a matching name is found.
		Node* GetSimilar();

		void MapKeys();
		void MapNodes();

		// Print a list of child nodes and attributes (keys)
		void PrintMap();
	};

	struct Col_Library_Geometries;

	class Collada
	{
	public:	
		Collada(void);

		static void ParseKeys(XMLElement* XML, Node* N);
		static void ParseSibling(XMLElement* XML, Node* Parent);

		static Node Load(std::string Path);
		static Model LoadModel(std::string path);
		static Col_Library_Geometries Load_LibGeometries(std::string path);
	};


	struct Col_Source
	{
		enum Type {Position = 0, Normal, Texcoord};

		Type			m_Type;
		std::string		m_Data;
		std::string		m_Id;
		unsigned  int	m_Stride;	
	};



	struct Col_Input
	{
		std::string m_Semantic,m_Offset, m_SourceID; // Remember to strip '#' from front of m_sourceID name
	};

	struct Col_Vertices
	{
		std::string	m_id;
		Col_Input	m_input;
	};

	struct Col_Triangles
	{
		unsigned int			m_Count;
		std::vector<Col_Input>	m_Input;
		std::vector<GLushort>	m_P;
	};


	struct Col_Mesh
	{
		std::vector<Col_Source> m_Source;
		std::vector<Col_Triangles> m_Triangles;
		Col_Vertices m_Vertices;
	};


	struct Col_Geometry
	{
		std::string m_Id,m_Name;
		std::vector<Col_Mesh> m_Mesh;
	};


	struct Col_Library_Geometries
	{
		std::vector<Col_Geometry> m_Geometry;
	};

};