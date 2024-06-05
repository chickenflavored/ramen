#include "statue.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../gl/tiny_obj_loader.h"

#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>


statue::statue()
{
	vao = 0;
	vbo = 0;
	ebo = 0;

	shader_program = 0;
	vertex_shader = 0;
	fragment_shader = 0;

	load_obj();
	setup_buffers();
	init_shaders();
}

void statue::load_obj()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "assets/world/objects/statue.obj", nullptr, true);


	if (!warn.empty())
	{
		std::cout << "WARN: " << warn << std::endl;
	}

	if (!err.empty())
	{
		std::cerr << "ERR: " << err << std::endl;
		return;
	}

	if (!ret)
	{
		std::cerr << "Failed to load statue.obj" << std::endl;
		return;
	}

	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Iterate over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Iterate over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				vertices.push_back(glm::vec3(vx, vy, vz));

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0)
				{
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					normals.push_back(glm::vec3(nx, ny, nz));
				}

				indices.push_back(indices.size());
			}
			index_offset += fv;
		}

		std::cout << "loaded model with " << vertices.size() << " vertices and " << indices.size() << " indices" << std::endl;
	}
}

void statue::setup_buffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate and bind the Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	// Set the vertex attribute pointers for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	// Generate and bind the Element Buffer Object (EBO)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Generate and bind the normal buffer
	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	// Set the vertex attribute pointers for normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	// Unbind the VAO
	glBindVertexArray(0);
}

void statue::init_shaders()
{
	std::string vertexShaderCode = read_shader_file("assets/shaders/objects/statue/statue_vert.glsl");
	const char* vertexShaderSource = vertexShaderCode.c_str();

	// Compile the vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);
	check_shader_compilation(vertex_shader, "VERTEX");

	// Read the fragment shader source code from file
	std::string fragmentShaderCode = read_shader_file("assets/shaders/objects/statue/statue_frag.glsl");
	const char* fragmentShaderSource = fragmentShaderCode.c_str();

	// Compile the fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);
	check_shader_compilation(fragment_shader, "FRAGMENT");

	// Create and link the shader program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	check_shader_link(shader_program);

	// Clean up
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

std::string statue::read_shader_file(std::string path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		//file opening failure
		return "";
	}

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	return content;
}

void statue::check_shader_compilation(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}

void statue::check_shader_link(GLuint program)
{
	GLint success;
	GLchar infoLog[1024];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}

void statue::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	glm::mat4 new_model = glm::translate(model, glm::vec3(264925, 183388, 1400));
	//rotate new_model 90 degrees around the x-axis
	new_model = glm::rotate(new_model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	//rotate new_model 45 degrees around the y-axis
	new_model = glm::rotate(new_model, glm::radians(-145.0f), glm::vec3(0, 1, 0));
	new_model = glm::scale(new_model, glm::vec3(1600, 1600, 1600));

	//move new_model up slightly
	glUseProgram(shader_program);

	// Set the model, view, and projection matrices
	GLuint modelLoc = glGetUniformLocation(shader_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(new_model));

	GLuint viewLoc = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLoc = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Bind the VAO
	glBindVertexArray(vao);

	// Draw the object
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO
	glBindVertexArray(0);
}