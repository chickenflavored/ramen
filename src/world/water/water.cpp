#include "water.h"

#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

water::water()
{
	vbo = 0;
	vao = 0;

	shader_program = 0;
	vertex_shader = 0;
	fragment_shader = 0;

	model_loc = 0;
	view_loc = 0;
	projection_loc = 0;
}

void water::init()
{
	vertices = {
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f,-1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	vertex_shader = load_shader(GL_VERTEX_SHADER, "assets/shaders/water/water_vert.glsl");
	fragment_shader = load_shader(GL_FRAGMENT_SHADER, "assets/shaders/water/water_frag.glsl");

	shader_program = link_program();
	model_loc = glGetUniformLocation(shader_program, "model");
	view_loc = glGetUniformLocation(shader_program, "view");
	projection_loc = glGetUniformLocation(shader_program, "projection");
}


GLuint water::load_shader(GLuint type, const char* path)
{
	std::ifstream shader_file(path);
	if (!shader_file.is_open())
	{
		std::cerr << "failed to open shader file: " << path << std::endl;
		return 0;
	}

	std::string shader_source((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
	shader_file.close();

	GLuint shader_id = glCreateShader(type);
	const char* source_cstr = shader_source.c_str();
	glShaderSource(shader_id, 1, &source_cstr, nullptr);
	glCompileShader(shader_id);

	GLint compile_status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status)
	{
		char log[512];
		glGetShaderInfoLog(shader_id, 512, nullptr, log);
		std::cerr << "shader compilation error for " << path << ": " << log << std::endl;
		return 0;
	}

	return shader_id;
}

GLuint water::link_program()
{
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);

	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
	if (!link_status)
	{
		char log[512];
		glGetProgramInfoLog(program_id, 512, nullptr, log);
		std::cerr << "shader program linking error: " << log << std::endl;
		return 0;
	}

	return program_id;
}

void water::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	glUseProgram(shader_program);
	glBindVertexArray(vao);

	//glm::mat4 new_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2000.5f, 0.0f));

	glm::mat4 new_model = glm::scale(model, glm::vec3(500000, 500000, 1));
	//translate new_model up a bit
	new_model = glm::translate(new_model, glm::vec3(0.0f, 0.0f, -2.0f));


	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(new_model));
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
}
