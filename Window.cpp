#include "window.h"

const char* window_title = "GLFW Starter Project";

GLint shaderProgram;
Cloth * cloth;

std::vector<Cube *> keys;
std::vector<glm::vec3> rotate_vecs;

int select_idx = 0;
int interp_idx = 0;

bool gl_catmull = false;

Cube * floorCube;
CLoth * cloth;

float lastPos_x;
float lastPos_y;

bool windToggle;

bool gl_start = false;

glm::vec3 adjustWind;
glm::vec3 adjustPosition;

float time = 0.0f;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define PI 3.14159265

// Default camera parameters
glm::vec3 cam_pos(0.0f, 5.0f, 35.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

// Mouse trackball coords
bool isLeftPressed = false;
bool isRightPressed = false;
double rightxpos, rightypos;

void Window::initialize_objects(char * skel_file, char * skin_file)
{

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	cloth = new Cloth(10.0f, 10.0f, 10.0f, 10.0f);
	floorCube = new Cube();

	floorCube->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 35.0f)) * floorCube->toWorld;
	floorCube->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -5.0f, 0.5f)) * floorCube->toWorld;

	adjustWind = glm::vec3(0.0f, 0.0f, 1.0f);
	adjustPosition = glm::vec3(0.0f);
	windToggle = true; 

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{

	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);

	cloth->Draw(shaderProgram, Window::V, Window::P, cam_pos);
	floorCube->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		} 
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		lastPos_x = xpos;
		lastPos_y = ypos;
		isLeftPressed = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		isLeftPressed = false;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &rightxpos, &rightypos);
		isRightPressed = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		isRightPressed = false;
	} 
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (isLeftPressed)
	{
		float angle;
		// Perform horizontal (y-axis) rotation
		angle = (float)(lastPos_x - xpos) / 100.0f;
		cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f));
		cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_up, 1.0f));
		//Now rotate vertically based on current orientation
		angle = (float)(ypos - lastPos_y) / 100.0f;
		glm::vec3 axis = glm::cross((cam_pos - cam_look_at), cam_up);
		cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_pos, 1.0f));
		cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_up, 1.0f));
		// Now update the camera
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		lastPos_x = xpos;
		lastPos_y = ypos;
	} 
}

void Window::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	float coef = 1.0f;

	if (yoffset > 0)
	{
		coef = -1.0f;
	}

	if (cam_pos.x > 0.01f)
	{
		cam_pos.x += cam_pos.x * coef * 0.1f;
	}
	else if (cam_pos.x < -0.01f)
	{
		cam_pos.x += cam_pos.x * coef * 0.1f;
	}
	else
	{
		// x at 0, do nothing
	}


	if (cam_pos.y > 0.01f)
	{
		cam_pos.y += cam_pos.y * coef * 0.1f;
	}
	else if (cam_pos.y < -0.01f)
	{
		cam_pos.x += cam_pos.y * coef * 0.1f;
	}
	else
	{
		// y at 0, do nothing
	}

	if (cam_pos.z > 0.01f)
	{
		cam_pos.z += cam_pos.z * coef * 0.1f;
	}
	else if (cam_pos.z < -0.01f)
	{
		cam_pos.z += cam_pos.z * coef * 0.1f;
	}
	else
	{
		// z at 0, do nothing
	}

	V = glm::lookAt(cam_pos, cam_look_at, cam_up); 
}

