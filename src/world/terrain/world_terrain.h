#ifndef __world_terrain_h__
#define __world_terrain_h__

#include <glm/glm.hpp>
#include "../../gl/glad.h"

#include <vector>

struct terrain_vertex
{
	glm::vec3 position;
	glm::vec2 tex_coords;
};

class world_terrain
{
public:
	world_terrain();

	void load();
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
	void build_mesh();
	void init_shaders();

	GLuint load_shader(GLuint type, const char* path);
	GLuint link_program();

	unsigned char* map_data;
	unsigned char* map_texture;
	int map_width, map_height, map_channels;
	int texture_width, texture_height, texture_channels;

	GLuint vao, vbo, ebo;
	GLuint texture_ID;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;

	GLuint model_loc, view_loc, projection_loc;

	std::vector<terrain_vertex> vertices;
	std::vector<GLuint> indices;
};

#endif // __world_terrain_h__