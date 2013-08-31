//File:	Collada.cpp
#include "Collada.h"
#include "GL\glew.h"
#include "GL\glfw.h"
#include <list>
#include <vector>
#include "Utility.h"

#include "Engine.h"
#include "Resource.h"

#include "glm\glm.hpp"

using namespace std;



namespace TE
{

	// |---------- class Node ----------|
	Node::Node() 
	{
		Create();
	}
	Node::Node(std::string _Name)
	{
		Create(_Name);
	}


	bool Node::IsRoot() {return  (m_Root == (Node*)this) ? true : false; }


	std::pair<bool,Node*> Node::ID_Find(std::string Name)
	{
		if (!m_Root) 
		{
			Utility::Log("Node: m_Root == NULL! Failed to add ID lookup."); 
			return std::pair<bool,Node*>(false,NULL);
		}
		
		for (auto it : m_Root->m_IDLookup)
		{
			if( Utility::CompareString(it.first,Name) )
			{
				return std::pair<bool,Node*>(true,it.second);
			}
		}
		return std::pair<bool,Node*>(false,NULL);


	}
	void	Node::ID_Add(std::string Name, Node* Ptr)
	{
		if (!m_Root)
		{
			Utility::Log("Node: m_Root == NULL! Failed to add ID lookup.");
			return;
		}
		if (ID_Find(Name).first)
		{
			Utility::Log("[WARNING] (Node::ID_Add) m_IDLookup already contains ID "+Name);
		}
		m_Root->m_IDLookup.push_back(std::pair<std::string,Node*>(Name,Ptr));
	}
	Node*	Node::ID_Get(std::string Name)
	{
		if (!m_Root)
		{
			Utility::Log("Node: m_Root == NULL! Failed to add ID lookup.");
			return NULL;
		}

		std::pair<bool,Node*> Find  = ID_Find(Name);
		if (Find.first)
		{
			return Find.second;
		}
		else
		{
			Utility::Log("ID_Get: Could not find ID "+Name);
			return NULL;
		}
	}


	void Node::Create(std::string _Name)
	{
		m_Name = _Name;
		m_Parent = NULL;
		m_Nodes = std::list<Node>(0);
		m_Keys = std::vector<Key>(0);
		m_Parent = NULL;
		m_Next = NULL;
		m_Prev = NULL;

		m_Root = NULL;
	}

	Node* Node::CreateSubNode()
	{
		// If I have no root node, I become the root node.
		// My babies gotta eat.
		if (m_Root == NULL)
		{
			m_Root = this;
		}

		Node *SecondLast = NULL;
		if(m_Nodes.size() > 0)
		{
			SecondLast = &m_Nodes.back();
		}

		m_Nodes.push_back(Node());
		Node *RequestedNode = &m_Nodes.back();
		RequestedNode->m_Parent = this;
		RequestedNode->m_Next = NULL;
		RequestedNode->m_Root = m_Root;

		if (SecondLast != NULL)
		{
			SecondLast->m_Next = RequestedNode;
			RequestedNode->m_Prev = SecondLast;
		}

		return RequestedNode;
	}

	void Node::AddKey(std::string Name,std::string Value)
	{
		m_Keys.push_back(Key(Name,Value));
	}
	void Node::AddKey(Key key)
	{
		m_Keys.push_back(key);
	}

	Node* Node::GetNode(std::string Name)
	{
		for (auto it = m_Nodes.begin();it != m_Nodes.end();it++)
		{
			if ( Utility::CompareString(it->m_Name,Name) )
			{
				return &(*it);
			}
		}
		Utility::Log("Node: Query Failed, no immediate nodes named "+Name);
		return NULL;
	}
	Node* Node::GetAnyNode(std::string Name, bool Subprocess)
	{
		for (auto it = m_Nodes.begin();it != m_Nodes.end();it++)
		{
			if ( Utility::CompareString(it->m_Name,Name) )
			{
				return &(*it);
			}


			Node* Find = it->GetAnyNode(Name,true);
			if (Find != NULL)
			{
				return Find;
			}
		}

		// Prevent spitting out errors if the method started itself
		if (Subprocess = false)
		{
			Utility::Log("Node: Query Failed, no child nodes named "+Name);
		}
		return NULL;
	}
	Node* Node::GetSimilar()
	{
		Node *N = m_Next;
		while (N != NULL)
		{
			if (Utility::CompareString(N->m_Name,m_Name))
			{
				return N;
			}
			N = N->m_Next;
		}
		return NULL;
	}

	Node::Key* Node::GetKey(std::string Name)
	{
		for (auto it = m_Keys.begin();it != m_Keys.end();it++)
		{
			if (Utility::CompareString(it->m_Name,Name))
			{
				return &(*it);
			}
		}
		std::cout << "GetKey: Attribute Query Failed: could not find " << Name.c_str() << " within " << this->m_Name <<"\n";
		return NULL;
	}
	Node::Key* Node::GetAnyKey(std::string Name, bool Subprocess)
	{
		for (auto it = m_Nodes.begin();it != m_Nodes.end();it++)
		{				
			for (auto k = it->m_Keys.begin(); k != it->m_Keys.end(); k++)
			{
				if (Utility::CompareString(k->m_Name,Name) )
				{
					return (Key*) &(*k);
				}
			}

			Key* Find = (Key*) it->GetAnyKey(Name,true);
			if (Find != NULL)
			{
				return Find;
			}
		}

		// Prevent spitting out errors if the method started itself
		if (Subprocess = false)
		{
			std::cout << "GetAnyKey: Attribute Any Query Failed: No child keys named "<<Name.c_str() <<std::endl;
		}
		return NULL;
	}



	void Node::MapKeys()
	{
		for ( auto it = m_Keys.begin(); it != m_Keys.end(); it++)
		{
			std::cout << "Key: " << it->m_Name << std::endl;
			std::cout << "Value: "<< it->m_Value << std::endl;
		}
	}
	void Node::MapNodes()
	{
		for ( auto it = m_Nodes.begin(); it != m_Nodes.end(); it++)
		{
			std::cout << "---Node---\n";
			std::cout << "Name: " << it->m_Name << std::endl;

			it->MapKeys();
			it->MapNodes();
			std::cout <<"\n";
		}
	}
	void Node::PrintMap()
	{
		std::cout << "---Start Node---\n"<< "Name: " << m_Name << std::endl;
		MapKeys();
		std::cout <<"\n";
		MapNodes();
	}



	// |---------- class Collada ----------|
	Collada::Collada(void) {}

	void Collada::ParseKeys(XMLElement* XML, Node* N)
	{		

		if (XML != NULL)
		{
			if (XML->GetText() != NULL)
			{
				N->AddKey("text",(char*) XML->GetText());
			}

			tinyxml2::XMLAttribute* Attrib = (tinyxml2::XMLAttribute*) XML->FirstAttribute();

			N->m_Name = (char*) XML->Name();
			while (Attrib != NULL)
			{
				
				Node::Key TempKey((char*)Attrib->Name(),(char*)Attrib->Value());


				if( Utility::CompareString(Attrib->Name(),"id"))
				{
					// Add to master ID lookup table.
					N->ID_Add(Attrib->Value(),N);
				}
				else if ( Utility::CompareString(Attrib->Name(),"source") || Utility::CompareString(Attrib->Name(),"url"))
				{
					// Remove '#' symbol.
					std::string Str = Attrib->Value();
					Str = Str.erase(0,1);
					TempKey.m_ptr = N->ID_Get(Str);
				}

				N->AddKey(TempKey);
				Attrib = (tinyxml2::XMLAttribute*) Attrib->Next();
			}
		}
	}
	void Collada::ParseSibling(XMLElement* XML, Node* Parent)
	{
		if (XML != NULL)
		{
			XML = XML->FirstChildElement();

			while(XML != NULL)
			{
				Node *N = Parent->CreateSubNode();
				ParseKeys(XML,N);
				ParseSibling(XML,N);

				//std::cout << "Completed Node Processing: "<<N->Name<<std::endl;
				XML = XML->NextSiblingElement();
			}
		}

	}

	Node Collada::Load(std::string Path)
	{
		XMLDocument Doc;
		if (Doc.LoadFile(Path.c_str()) == XML_SUCCESS)
		{
			Utility::Log("Collada: Loadeded ["+Path+"] successfully.");
		}
		else
		{
			Utility::Log("Collada: Failed to open file ["+Path+"]");
		}

		XMLElement *XMLMain = Doc.RootElement();

		Node Root;
		ParseKeys(XMLMain,&Root);
		ParseSibling(XMLMain,&Root);


		//Root.PrintMap();

		return Root;
	}

	std::vector<glm::vec3> Vectorize_Tokens(const char* Text,bool _3D = true)
	{
		std::vector<glm::vec3>	ReturnVector;
		std::vector<GLfloat>	Vec  = Utility::Vector_SplitString<GLfloat>((char *)Text);


		for (size_t Verticies = 0; Verticies < Vec.size()/3; Verticies++)
		{
			if (_3D)
			{
				ReturnVector.push_back(glm::vec3(Vec[Verticies*3],Vec[Verticies*3+1],Vec[Verticies*3+2]));
			}
			else
			{
				glm::vec3 V;
				V.s = Vec[Verticies*2];
				V.t = Vec[(Verticies*2)+1];
				ReturnVector.push_back(V);
			}
		}

		std::cout << "Printing Complete Vector Data...\n\n";
		for (auto it : ReturnVector)
		{
			if (_3D)
			{
				std::cout << it.x << ","<<it.y<<","<<it.z<<"\n";
			}
			else
			{
				std::cout << it.s << ","<<it.t <<"\n";
			}
		}
		std::cout << "\nComplete Vector Data. Print Complete.\n\n";

		return ReturnVector;
	}

	Model Collada::LoadModel(string path)
	{

		Col_Library_Geometries C_LibGeo = Collada::Load_LibGeometries(Engine.Resource->AliasAdd(Path::Models,path));

		Node Root = Collada::Load(Engine.Resource->AliasAdd(Path::Models,path));
		Model ReturnModel;

		Mesh M;	
		Node *Source = Root.GetAnyNode("source");

		// Position, Normal, Texcoord = PMT
		std::vector<glm::vec3> PNT[3];


		for (int i = 0;i<3;i++)
		{
			if (i == 2)
			{
				PNT[i]  = Vectorize_Tokens(Source->GetAnyKey("text")->m_Value.c_str(),false);
			}
			else
			{
				PNT[i]  = Vectorize_Tokens(Source->GetAnyKey("text")->m_Value.c_str());
			}
			Source = Source->m_Next;
		}

		vector<GLushort> Index = Utility::Vector_SplitString<GLushort>((char *)Root.GetAnyNode("triangles")->GetAnyNode("p")->GetKey("text")->m_Value.c_str());;


		std::vector<glm::vec3> Expanded_Vertex;


		for (unsigned int i = 0; i < Index.size()/3; i++)
		{


			glm::vec3 ExpandedVert	= PNT[0][Index[i*3]];
			glm::vec3 Norm			= PNT[1][Index[i*3+1]];
			glm::vec3 Tex			= PNT[1][Index[i*3+1]];

			ExpandedVert.r = Norm.x;
			ExpandedVert.g = Norm.y;
			ExpandedVert.b = Norm.z;

			ExpandedVert.s = Tex.s;
			ExpandedVert.t = Tex.t;


			Expanded_Vertex.push_back(ExpandedVert);

		}


		ReturnModel.m_Mesh.push_back(M);
		return ReturnModel;

	}




	Col_Library_Geometries Collada::Load_LibGeometries(std::string path)
	{

		Node Root		= Collada::Load(Engine.Resource->AliasAdd(Path::Models,path));
		Node *N_LibGeo	= Root.GetAnyNode("library_geometries");
		Col_Library_Geometries C_LibGeo;

		//Step into Geometry
		Node* N_Geometry = N_LibGeo->GetNode("geometry");
		while(N_Geometry != NULL)
		{
			Col_Geometry C_Geometry;
			C_Geometry.m_Id		= N_Geometry->GetKey("id")->m_Value;
			C_Geometry.m_Name	= N_Geometry->GetAnyKey("name")->m_Value;

			//Step into Mesh
			Node *N_Mesh		= N_Geometry->GetNode("mesh");
			while (N_Mesh != NULL)
			{
				Col_Mesh C_Mesh;

				//Step into Source
				Node* N_Source = N_Mesh->GetNode("source");
				while(N_Source != NULL)
				{
					Col_Source C_Source;
					C_Source.m_Id		= N_Source->GetKey("id")->m_Value;
					C_Source.m_Data		= N_Source->GetNode("float_array")->GetKey("text")->m_Value;
					C_Source.m_Stride	= Utility::ToUInt(N_Source->GetNode("technique_common")->GetNode("accessor")->GetKey("stride")->m_Value);

					C_Mesh.m_Source.push_back(C_Source);
					N_Source = N_Source->GetSimilar();
				}

				
				
				Node* N_Vert = N_Mesh->GetNode("vertices");
				if (N_Vert != NULL)
				{
					Col_Vertices C_Vert;
					C_Vert.m_id = N_Vert->GetKey("id")->m_Value;

					Node* N_Input = N_Vert->GetNode("input");
					C_Vert.m_input.m_Semantic = N_Input->GetKey("semantic")->m_Value;
					C_Vert.m_input.m_SourceID = N_Input->GetKey("source")->m_Value;
					C_Mesh.m_Vertices = C_Vert;
				}
				else
				{
					Utility::Log("Collada::Load: Failed to load vertices into C_Mesh!!!");
				}

				//Step into Triangle
				Node *N_Triangles = N_Mesh->GetNode("triangles");
				while (N_Triangles != NULL)
				{
					Col_Triangles C_Triangle;
					C_Triangle.m_Count = Utility::ToUInt(N_Triangles->GetKey("count")->m_Name);


					Node* N_Input = N_Triangles->GetNode("input");
					while(N_Input != NULL)
					{
						Col_Input C_Input;

						C_Input.m_Semantic	= N_Input->GetKey("semantic")->m_Value;
						C_Input.m_Offset	= Utility::ToUInt(N_Input->GetKey("offset")->m_Value);
						C_Input.m_SourceID	= N_Input->GetKey("source")->m_Value.erase(0,1);
						std::cout <<"Input SourceID: "<< C_Input.m_SourceID << "\n";

						C_Triangle.m_Input.push_back(C_Input);
						N_Input = N_Input->GetSimilar();
					}

					C_Triangle.m_P = Utility::Vector_SplitString<GLushort>((char*) N_Triangles->GetNode("p")->GetKey("text")->m_Value.c_str());
					C_Mesh.m_Triangles.push_back(C_Triangle);

					N_Triangles = N_Triangles->GetSimilar();
				}

				C_Geometry.m_Mesh.push_back(C_Mesh);

				N_Mesh = N_Mesh->GetSimilar();
			}

			C_LibGeo.m_Geometry.push_back(C_Geometry);
			N_Geometry = N_Geometry->GetSimilar();
		}


		return C_LibGeo;

	}

};