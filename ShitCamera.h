#pragma once


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Engine.h"



namespace TE
{

	class ShittyCamera
	{
	public:
		glm::vec3 m_Pos;
		glm::vec3 m_Angles;

		glm::vec2 m_LastMousePos;
		bool m_MouseHeld;

		ShittyCamera()
		{
			m_MouseHeld = false;
			m_LastMousePos = glm::vec2(0, 0);
			m_Pos = glm::vec3(0, 0, 0);
			m_Angles = glm::vec3(0, 0, 0);

		}

		inline void Update()
		{

			

			if (glfwGetMouseButton(Engine.Window,GLFW_MOUSE_BUTTON_1))
			{
				double mouseX, mouseY;

				glfwGetCursorPos(Engine.Window,&mouseX, &mouseY);
				glm::vec2 currentMousePos = glm::vec2(mouseX, mouseY);

				if (m_MouseHeld)
				{
					glm::vec2 mouseDiff = currentMousePos - m_LastMousePos;
					m_Angles += glm::vec3(mouseDiff.y / 2, mouseDiff.x / 2, 0);
				}

				m_LastMousePos = currentMousePos;
				m_MouseHeld = true;

			}
			else
			{
				m_MouseHeld = false;
			}



			bool KeyPressed = false;
			int	 Modifier = 1;

			if (glfwGetKey(Engine.Window, GLFW_KEY_W))
			{
				KeyPressed = true;
			}
			if (glfwGetKey(Engine.Window, GLFW_KEY_S))
			{
				KeyPressed = true;
				Modifier = -1;
			}



			if (KeyPressed)
			{
				static const double rads = (3.14 / 180);
				double rady = m_Angles.y * rads;
				double radx = m_Angles.x * rads;

				m_Pos.x += Modifier * -sinf(rady) * cosf(radx);
				m_Pos.y += Modifier * sinf(radx);
				m_Pos.z += Modifier * cosf(rady) * cosf(radx);
			}


		}

		inline glm::mat4 getCamMatrix()
		{
			glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			ViewTranslate = glm::rotate(ViewTranslate, m_Angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
			ViewTranslate = glm::rotate(ViewTranslate, m_Angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
			ViewTranslate = glm::rotate(ViewTranslate, m_Angles.z, glm::vec3(0.0f, 0.0f, 1.0f));

			ViewTranslate = glm::translate(ViewTranslate, m_Pos);
			return ViewTranslate;

		}

	};

	ShittyCamera ShitCam;

}