
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, 
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;

/********************************************************************************
 Define an error callback
 ********************************************************************************/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

/********************************************************************************
 Define the key input callback
 ********************************************************************************/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/********************************************************************************
 Callback function when the window is resized
 ********************************************************************************/
void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

/********************************************************************************
 Get keyboard's key states
 ********************************************************************************/
bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

/********************************************************************************
 Get mouse updates
 ********************************************************************************/
bool Application::GetMouseUpdate()
{
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width - m_window_deadzone))
	{
		mouse_current_x = m_window_width >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height - m_window_deadzone))
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	BUTTON_LEFT = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
	BUTTON_MIDDLE = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE);
	BUTTON_RIGHT = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);

	theGSM->HandleEvents(mouse_current_x, mouse_current_y, BUTTON_LEFT, BUTTON_MIDDLE, BUTTON_RIGHT);
    return false;
}

/********************************************************************************
 Get keyboard updates
 ********************************************************************************/
bool Application::GetKeyboardUpdate()
{
	if (IsKeyPressed('A'))
	{
		theGSM->HandleEvents('a');
	}
	else
	{
		theGSM->HandleEvents('a', false);
	}
	if (IsKeyPressed('D'))
	{
		theGSM->HandleEvents('d');
	}
	else
	{
		theGSM->HandleEvents('d', false);
	}
	if (IsKeyPressed('W'))
	{
		theGSM->HandleEvents('w');
	}
	else
	{
		theGSM->HandleEvents('w', false);
	}
	if (IsKeyPressed('S'))
	{
		theGSM->HandleEvents('s');
	}
	else
	{
		theGSM->HandleEvents('s', false);
	}
	// Jump
	if (IsKeyPressed(32))
	{
		theGSM->HandleEvents(32);
	}
	// Rotate camera
	if (IsKeyPressed(VK_LEFT))
	{
		theGSM->HandleEvents(VK_LEFT);
	}
	else
	{
		theGSM->HandleEvents(VK_LEFT, false);
	}
	if (IsKeyPressed(VK_RIGHT))
	{
		theGSM->HandleEvents(VK_RIGHT);
	}
	else
	{
		theGSM->HandleEvents(VK_RIGHT, false);
	}
	if (IsKeyPressed(VK_UP))
	{
		theGSM->HandleEvents(VK_UP);
	}
	else
	{
		theGSM->HandleEvents(VK_UP, false);
	}
	if (IsKeyPressed(VK_DOWN))
	{
		theGSM->HandleEvents(VK_DOWN);
	}
	else
	{
		theGSM->HandleEvents(VK_DOWN, false);
	}
    return true;
}

/********************************************************************************
 Constructor
 ********************************************************************************/
Application::Application()
	: scene(NULL)
	, theGSM(NULL)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
Application::~Application()
{
	if (theGSM)
	{
		delete theGSM;
		theGSM = NULL;
	}
}

/********************************************************************************
 Initialise this program
 ********************************************************************************/
void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	lua_State *L = lua_open();
	luaL_openlibs(L);
	if (luaL_loadfile(L, "config.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	lua_getglobal(L, "SCREENHEIGHT");
	if (!lua_isnumber(L, -1))
	{
		printf("'SCREENHEIGHT' must be an integer!\n");
	}
	m_window_height = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "SCREENWIDTH");
	if (!lua_isnumber(L, -1))
	{
		printf("'SCREENWIDTH' must be an integer!\n");
	}
	m_window_width = (int)lua_tonumber(L, -1);

	lua_getglobal(L, "PROGRAM_NAME");
	if (!lua_isstring(L, -1))
	{
		printf("'PROGRAM_NAME' must be a string!\n");
	}
	string PROGRAM_NAME = lua_tostring(L, -1);

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, PROGRAM_NAME.c_str(), NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set these 2 variables to zero
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;

	theGSM = new CGameStateManager();
	theGSM->Init("DM2240 with Game State Management", 800, 600);
	theGSM->ChangeState(CSplashState::Instance());  // Run splash state first
}

/********************************************************************************
 Run this program
 ********************************************************************************/
void Application::Run()
{

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !Application::IsKeyPressed(VK_ESCAPE))// take away !VK_ESCAPE if you want to use it for another reason.
	{
		// Get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		if (m_dAccumulatedTime_ThreadOne > 0.03)
		{
			//Get Inputs
			GetMouseUpdate();
			GetKeyboardUpdate();

			//Update the GSM
			theGSM->HandleEvents();
			theGSM->Update(m_dElapsedTime);
		
			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		if (m_dAccumulatedTime_ThreadTwo > 1.0)
		{
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		// Render the scene
		theGSM->Draw();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed

	// Delete the scene
	theGSM->Cleanup();
}

/********************************************************************************
 Exit this program
 ********************************************************************************/
void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
