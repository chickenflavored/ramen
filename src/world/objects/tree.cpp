#include "tree.h"

#include "../../gl/tiny_obj_loader.h"
#include "../../gl/stb_image.h"

#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

tree::tree()
{
	vao = 0;
	vbo = 0;
	ebo = 0;

	gl_texture = 0;

	shader_program = 0;
	vertex_shader = 0;
	fragment_shader = 0;

	tree_positions = {
		glm::vec3(263925, 185388, 430),
		glm::vec3(263955, 185388, 430),
		glm::vec3(263985, 185388, 430),
	};

	load_obj();
	setup_buffers();
	init_shaders();
}


void tree::load_obj()
{
	//tinyobjloader
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "assets/world/objects/tree.obj", nullptr, true);


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
		std::cerr << "Failed to load tree.obj" << std::endl;
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

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0)
				{
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					tex_coords.push_back(glm::vec2(tx, -ty));
				}

				indices.push_back(indices.size());
			}
			index_offset += fv;
		}
		std::cout << "loaded model with " << vertices.size() << " vertices and " << indices.size() << " indices" << std::endl;
	}
}

void tree::setup_buffers()
{
	//texture loading
	unsigned char* data = stbi_load("assets/world/objects/tree_tex2.png", &tex_width, &tex_height, &tex_channels, 0);
	if (!data)
	{
		std::cerr << "tree::load_obj--Failed to load tree texture" << std::endl;
		return;
	}

	glGenTextures(1, &gl_texture);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

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

	// Generate and bind the texture coordinate buffer
	GLuint texCoordBuffer;
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(glm::vec2), tex_coords.data(), GL_STATIC_DRAW);

	// Set the vertex attribute pointers for texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);

	// Unbind the VAO
	glBindVertexArray(0);
}

void tree::init_shaders()
{
	std::string vertexShaderCode = read_shader_file("assets/shaders/objects/tree/tree_vert.glsl");
	const char* vertexShaderSource = vertexShaderCode.c_str();

	// Compile the vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);
	check_shader_compilation(vertex_shader, "VERTEX");

	// Read the fragment shader source code from file
	std::string fragmentShaderCode = read_shader_file("assets/shaders/objects/tree/tree_frag.glsl");
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

std::string tree::read_shader_file(std::string path)
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

void tree::check_shader_compilation(GLuint shader, std::string type)
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

void tree::check_shader_link(GLuint program)
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

void tree::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	glm::mat4 new_model = glm::translate(model, glm::vec3(263925, 185388, 430));
	//rotate new_model 90 degrees around the x-axis
	new_model = glm::rotate(new_model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	new_model = glm::scale(new_model, glm::vec3(15, 15, 15));

	glUseProgram(shader_program);

	// Set the model, view and projection matrices
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(new_model));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);

	// Bind the VAO
	glBindVertexArray(vao);

	// Draw the object
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO
	glBindVertexArray(0);
}