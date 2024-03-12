#ifndef __water_h__
#define __water_h__

#include <glm/glm.hpp>

#include "../../gl/glad.h"

#include <vector>

class water
{
public:
	water();

	void init();
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
	GLuint load_shader(GLuint type, const char* path);
	GLuint link_program();


	GLuint vao, vbo;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;

	GLuint model_loc, view_loc, projection_loc;

	std::vector<float> vertices;
};

#endif // __water_h__