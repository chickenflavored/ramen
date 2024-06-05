#ifndef __statue_h__
#define __statue_h__

#include <glm/glm.hpp>
#include "../../gl/glad.h"

#include <vector>
#include <string>

class statue
{
public:
	statue();

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

	GLuint vao, vbo, ebo;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;
};

#endif // __statue_h__