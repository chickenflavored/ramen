#include "renderer.h"

#include <glm/gtc/type_ptr.hpp>


renderer& renderer::global()
{
	static renderer instance;
	return instance;
}

void renderer::init(sf::RenderWindow* w_ptr)
{
	this->w_ptr = w_ptr;
	last_x = 0;
	last_y = 0;

	cam_pitch = 0;


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	position = glm::vec3(0.0f, 0.0f, 1.0f);

	target = glm::vec3(0.0f, 0.0f, 0.0f);

	// initialize the view matrix to look at the origin from a distance
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), // camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // camera target
		glm::vec3(0.0f, 0.01f, 0.0f)); // camera up vector

	// initialize the projection matrix to a perspective projection
	projection = glm::perspective(glm::radians(80.f), // field of view angle
		(float)1600 / (float)900, // aspect ratio
		3.0f, // near clipping plane
		250000.f); // far clipping plane


	//move camera up to simulate being a person in the world
	position.y += 605; 
	position.x += 263525.f;
	position.z -= 185288.f;

	g_water = new water();
	g_water->init();

	g_world_terrain = new world_terrain();
	g_world_terrain->load();

	g_statue = new statue();
	g_tree = new tree();
}

std::string renderer::read_file(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

GLuint renderer::compile_shader(GLenum shader_type, const std::string& source)
{
	GLuint shader = glCreateShader(shader_type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, nullptr, log);
		std::cerr << "Failed to compile shader: " << log << std::endl;
		delete[] log;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint renderer::link_program(GLuint vertex_shader, GLuint fragment_shader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(program, length, nullptr, log);
		std::cerr << "Failed to link program: " << log << std::endl;
		delete[] log;
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

void renderer::update(float dt)
{
	float speed = .2f; // camera speed per second
	float angle = dt * 0.005f; // camera angle per second

	// get the direction vector of the camera
	glm::vec3 direction = glm::normalize(target - position);

	// get the right vector of the camera
	glm::vec3 right = glm::normalize(glm::cross(direction, up));


	float distance = speed * dt; // camera distance per frame

	//move camera only if ui's console isn't open
	if (!ui::global().is_console_open())
	{
		// check if the W key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			// move the camera forward along the horizontal direction vector
			glm::vec3 horizontal_direction = glm::vec3(direction.x, 0.0f, direction.z);
			position += horizontal_direction * distance;
			target += horizontal_direction * distance;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			// move the camera backward along the horizontal direction vector
			glm::vec3 horizontal_direction = glm::vec3(direction.x, 0.0f, direction.z);
			position -= horizontal_direction * distance;
			target -= horizontal_direction * distance;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			// move the camera left along the right vector
			position -= right * distance;
			target -= right * distance;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			// move the camera right along the right vector
			position += right * distance;
			target += right * distance;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			glm::quat rotation = glm::angleAxis(angle, up);
			target = rotation * (target - position) + position;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			// rotate the camera right around the up vector
			glm::quat rotation = glm::angleAxis(-angle, up);
			target = rotation * (target - position) + position;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			// move the camera up along the up vector
			position += up * distance;
			target += up * distance;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			// move the camera down along the up vector
			position -= up * distance;
			target -= up * distance;
		}
	}
	

	if (ui::global().set_wireframe())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	


	//check if mouse right button is held down
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (!mouse_right_held)
		{
			//store mouse position for when we release the button
			mouse_pos = sf::Mouse::getPosition(*w_ptr);
			mouse_right_held = true;
			sf::Mouse::setPosition(sf::Vector2i(w_ptr->getSize() / 2u), *w_ptr);
			w_ptr->setMouseCursorVisible(false);
		}

		sf::Vector2i delta = sf::Mouse::getPosition(*w_ptr) - sf::Vector2i(w_ptr->getSize() / 2u);


		// get the current mouse position
		double xpos, ypos;
		xpos = sf::Mouse::getPosition(*w_ptr).x;
		ypos = sf::Mouse::getPosition(*w_ptr).y;


		// calculate the pitch and yaw angles from the mouse movement
		float sensitivity = 0.01f; // mouse sensitivity
		float pitch = -(delta.y) * sensitivity; // vertical angle
		float yaw = (delta.x) * sensitivity; // horizontal angle

		cam_pitch += pitch;

		if (cam_pitch > 1.5f)
		{
			pitch = 0;
			cam_pitch = 1.5f;
		}
		else if (cam_pitch < -1.5f)
		{
			pitch = 0;
			cam_pitch = -1.5f;
		}


		// update the last mouse position
		last_x = xpos;
		last_y = ypos;

		// rotate the target around the right vector by the pitch angle
		glm::quat pitch_rotation = glm::angleAxis(pitch, right);
		target = pitch_rotation * (target - position) + position;

		

		// rotate the target around the up vector by the yaw angle
		glm::quat yaw_rotation = glm::angleAxis(-yaw, up);
		target = yaw_rotation * (target - position) + position;

		sf::Mouse::setPosition(sf::Vector2i(w_ptr->getSize() / 2u), *w_ptr);

	}
	else
	{
		//show cursor
		w_ptr->setMouseCursorVisible(true);
		double mouse_x, mouse_y;
		mouse_x = sf::Mouse::getPosition(*w_ptr).x;
		mouse_y = sf::Mouse::getPosition(*w_ptr).y;
		last_x = mouse_x;
		last_y = mouse_y;
		if (mouse_right_held)
		{
			mouse_right_held = false;
			sf::Mouse::setPosition(mouse_pos, *w_ptr);
		}
	}

	// update the view matrix with the new position and target
	view = glm::lookAt(position, target, up);

}

sf::Vector3f renderer::get_player_coords()
{
	return sf::Vector3f(position.x, position.y, position.z);
}

void renderer::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	g_water->render(model, view, projection);
	g_world_terrain->render(model, view, projection);
	g_statue->render(model, view, projection);
	g_tree->render(model, view, projection);
}