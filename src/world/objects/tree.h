#ifndef __tree_h__
#define __tree_h__

#include <glm/glm.hpp>
#include "../../gl/glad.h"

#include <vector>
#include <string>

class tree
{
public:
	tree();

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:
	void load_obj();
	void setup_buffers();
	void init_shaders();

	std::string read_shader_file(std::string path);
	void check_shader_compilation(GLuint shader, std::string type);
	void check_shader_link(GLuint program);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> tex_coords;

	std::vector<glm::vec3> tree_positions;

	GLuint gl_texture;
	int tex_width, tex_height, tex_channels;

	uint32_t num_instances;

	GLuint vao, vbo, ebo;
	GLuint instance_vbo;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;
};

#endif // __tree_h__