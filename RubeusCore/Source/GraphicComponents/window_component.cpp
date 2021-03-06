/**
 * @file		Source/window_component.cpp.
 *
 * @brief	Implements the RWindowComponent class
 */
#include <window_component.h>

#include <any>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Rubeus
{
	namespace GraphicComponents
	{
		int RWindowComponent::MouseX;
		int RWindowComponent::Y;

		void getGLFWErrorLog(int error, const char * description)
		{
			ERRORLOG(description);
		}

		void windowCloseCallback(GLFWwindow * window)
		{
			LOG("Window close callback was evoked");
		}

		void windowResizeCallback(GLFWwindow * window, int width, int height)
		{
			LOG("Window resize callback was evoked");
			GLCall(glViewport(0, 0, width, height));
		}

		RWindowComponent::RWindowComponent(const char *title, int width, int height, EWindowParameters windowMode, EWindowParameters windowType, int setFPS)
		{
			if (!initWindow(title, width, height, windowMode, windowType))
			{
				ERRORLOG("WindowComponent Initialisation failed");
				glfwTerminate();
			}

			SUCCESS("Window initialisation successful");

			//glfwSwapInterval(setFPS);
			ASSERT("FPS set to " + std::to_string((1.0f / ((float)setFPS)) * 60.0f));

			ASSERT((const char *)glGetString(GL_VENDOR));

			m_Height = height;
			m_Width = width;
			m_Title = title;

			// TODO: Add customisable resolutions
		}

		bool RWindowComponent::closed()
		{
			return glfwWindowShouldClose(m_GLFWWindow);
		}

		void RWindowComponent::setWindowTitle(const char * title)
		{
			glfwSetWindowTitle(m_GLFWWindow, title);
			ASSERT("Window title set to: " + (std::string) title);
		}

		void RWindowComponent::setWindowIcon(RWindowComponent GameWindow, std::string names[])
		{
			//GLFWimage * images = new GLFWimage[names->length];

			//for(int i = 0; i < names->length; i++)
			//{
			//	images[i] = LoaderComponent::LoadImageWindows(names[i]);
			//}

			// TODO: Remove this when LoaderComponent::LoadImageWindows() is completed
			// TODO: Remove this when LoaderComponent::LoadImageWindows() is completed
			ERRORLOG("ABORT! Incomplete code used");

			//delete[] images;
		}

		RWindowComponent::~RWindowComponent()
		{
			ASSERT("Terminating GLFW Window");
			glfwTerminate();
		}

		void RWindowComponent::clearWindow()
		{
			// No GLCall() used for maintaining performance
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void RWindowComponent::updateWindow()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_GLFWWindow);
		}

		bool RWindowComponent::initWindow(const char * title, int width, int height, EWindowParameters windowMode, EWindowParameters windowType)
		{
			if (!glfwInit())
			{
				ERRORLOG("Error: GLFW initialisation failed");
			}

			SUCCESS("GLFW initialisation successful");

			// Set window hints if any
			if (windowType == EWindowParameters::RESIZABLE_WINDOW)
			{
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			}
			else
			{
				if (windowType == EWindowParameters::NON_RESIZABLE_WINDOW)
				{
					glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
				}
				else
				{
					ERRORLOG("Semantics error: Use valid Enum values");
				}
			}

			glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Create a window of specified mode, title, width and height
			if (windowMode == EWindowParameters::WINDOWED_MODE)
			{
				m_GLFWWindow = glfwCreateWindow(width, height, title, NULL, NULL);
			}
			else
			{
				if (windowMode == EWindowParameters::FULLSCREEN_MODE)
				{
					m_GLFWWindow = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
				}
				else
				{
					ERRORLOG("Semantics error: Use valid Enum values");
				}
			}

			if (!m_GLFWWindow)
			{
				glfwTerminate();
				ERRORLOG("Failed to create window");

				return false;
			}


			// Set the new window as the current context
			glfwMakeContextCurrent(m_GLFWWindow);
			glfwSetWindowUserPointer(m_GLFWWindow, this);

			SUCCESS("Window creation successful");

			glfwSetErrorCallback(getGLFWErrorLog);
			glfwSetWindowCloseCallback(m_GLFWWindow, windowCloseCallback);
			glfwSetFramebufferSizeCallback(m_GLFWWindow, windowResizeCallback);


			glfwSetCursorPosCallback(m_GLFWWindow, cursorPositionCallback);
			glfwSetMouseButtonCallback(m_GLFWWindow, mouseButtonCallback);
			glfwSetInputMode(m_GLFWWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
			glfwSetScrollCallback(m_GLFWWindow, scrollCallback);

			glfwSetKeyCallback(m_GLFWWindow, keyCallback);
			glfwSetInputMode(m_GLFWWindow, GLFW_STICKY_KEYS, 1);


			if (glewInit() != GLEW_OK)
			{
				ERRORLOG("GLEW initialisation failed");

				return false;
			}

			SUCCESS("GLEW initialisation successful");
			ASSERT("OpenGL Driver software: " + std::string((const char *)glGetString(GL_VERSION)));

			return true;
		}

		void RWindowComponent::change_window_title(var data)
		{
			setWindowTitle(std::any_cast<const char *>(data));
		}
				
		void RWindowComponent::get_loaded_image(var data )
		{
			ASSERT("Image received");
			Image & image = std::any_cast<Image &>(data);
		}
				
	}
}
