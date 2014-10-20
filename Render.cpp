#include "Render.h"




#include "Engine.h"
#include "Resource.h"


#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



#include "Utility.h"

#include "GL_31.h"

#include "ShitCamera.h"


namespace TE
{

	CRender::CRender() 
	{
		Module_SetName("Render");
	};

	CRender::~CRender() {};


	void CRender::Module_Poll()
	{
		//Module_Print("Polling!");
		PreDraw();
		Draw();

	}

	void CRender::Window_Begin()
	{
		if (!glfwInit())
		{
			Utility::Log("Render::Window_Begin() - GLFW: Failed to initialize.\n");
			Window_End();
			return;
		}
		Engine.Window = glfwCreateWindow(500, 500, "TE", NULL, NULL);


		if( !Engine.Window )
			glfwTerminate();

		glfwMakeContextCurrent(Engine.Window);

		GLenum GLEW_Error = glewInit(); 

		if (GLEW_Error != GLEW_OK) 
		{
			Utility::Log("Render::Window_Begin() - GLEW: Failed to initialize, error log follows:\n");
			Utility::Log((char*)glewGetErrorString(GLEW_Error));
			Window_End();
			return;
		}

		Utility::Log("Render: OpenGL version: "+(std::string)(char*)glGetString(GL_VERSION));

		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);

		glClearDepth(1.0f);
		glClearColor(0.f, 0.5f, 0.7f, 0.f);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);        
		glDepthFunc(GL_LEQUAL);                       
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(90.f, 1.f, 1.f, 500.f);
	}
	void CRender::Window_End()
	{
		glfwTerminate();
		Utility::Log("Render: OpenGL Context terminated.\n");
	}

	void CRender::UseShader(GLuint Shader)
	{
		if (Shader != m_CurrentShader)
		{
			m_CurrentShader = Shader;
			glUseProgram(Shader);
		}
	}

	


	void CRender::PreDraw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glMatrixMode(GL_MODELVIEW);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLuint Shader = Engine.Resource->LoadShader_Program("Shader.Vertex","Shader.Fragment");
		if (Shader == 0)
		{
			Engine.MainLoop_Stop();
			return;
		}
		UseShader(Shader);
		GLuint LocationMVP = glGetUniformLocation(Shader, "MVP");
		//GLuint LocationTex = glGetUniformLocation(Shader, "texture");



		/*
		Mat4 Proj;
		Proj.Perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

		Mat4 V;
		Mat4 M;
		V.Identity();
		M.Identity();
		Mat4 MVP = Proj * V * M;

		glUniformMatrix4fv(LocationMVP, 1,GL_FALSE,MVP.SetMat16());
		*/

		ShitCam.Update();

		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
		glm::mat4 View = ShitCam.getCamMatrix();
		glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		glm::mat4 MVP = Projection * View * Model;

		//glUniform1i(LocationTex,Engine.Resource->LoadTexture("TestT16.TGA"));
		glUniformMatrix4fv(LocationMVP, 1,GL_FALSE,glm::value_ptr(MVP) );

	}

	void CRender::Draw()
	{
		PreDraw();

		GL_31::Model *M = Engine.Resource->LoadModel("box.dae");
		M->Draw();

		//std::cout << "Draw!\n";
		/*
		for(auto it = Resource::DrawList.begin(); it != Resource::DrawList.end();it++)
		{
		DrawModel(it->m_Model);
		}
		*/
//glBindTexture(GL_TEXTURE_2D,Engine.Resource->LoadTexture("TestT16.TGA"));

		
		//GL_31::Square S;
		//S.SetSize(10);
		//S.Draw();


		glfwSwapBuffers(Engine.Window);
		glfwPollEvents();



	}

};