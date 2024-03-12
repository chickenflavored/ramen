#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../gl/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../world/terrain/world_terrain.h"
#include "../world/water/water.h"

class renderer
{
public:
	static renderer& global();
	void init(sf::RenderWindow* w_ptr);

	void update(float dt);
	void draw();

	sf::Vector3f get_player_coords();
private:
	sf::RenderWindow* w_ptr;
	std::string read_file(const std::string& path);

	GLuint compile_shader(GLenum shader_type, const std::string& source);
	GLuint link_program(GLuint vertex_shader, GLuint fragment_shader);

	world_terrain* g_world_terrain;
	water* g_water;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 up;

	glm::vec3 position;
	glm::vec3 target;

	float last_x;
	float last_y;
	sf::Vector2i mouse_pos;

	float cam_pitch;

	bool mouse_right_held;


	bool wireframe;
	bool tilde_down;
};