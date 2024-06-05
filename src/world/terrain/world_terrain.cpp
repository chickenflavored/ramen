#include "world_terrain.h"

#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../../gl/stb_image.h"

world_terrain::world_terrain()
{
    vao = 0;
    vbo = 0;
    ebo = 0;
    texture_ID = 0;

    shader_program = 0;
    vertex_shader = 0;
    fragment_shader = 0;


    model_loc = 0;
    view_loc = 0;
    projection_loc = 0;

    map_data = nullptr;
    map_width = 0;
    map_height = 0;
    map_channels = 0;

    map_texture = nullptr;
	texture_width = 0;
    texture_height = 0;
    texture_channels = 0;

    vertices.clear();
    indices.clear();
}

void world_terrain::load()
{
    const char* path = "assets/world/terrain/hehe.png";
    map_data = stbi_load(path, &map_width, &map_height, &map_channels, 0);
    if (!map_data)
    {
        std::cerr << "failed to load the heightmap" << std::endl;
        return;
    }


    map_texture = stbi_load("assets/world/terrain/new_tex.png", &texture_width, &texture_height, &texture_channels, 0);

    if (!map_texture)
	{
		std::cerr << "failed to load map texture" << std::endl;
		return;
	}

    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, map_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    vertices.reserve(map_width * map_height);
    indices.reserve((map_height - 1) * (map_width - 1) * 6);

	build_mesh();
	init_shaders();
}

void world_terrain::build_mesh()
{
    for (int y = 0; y < map_height; y++)
    {
        for (int x = 0; x < map_width; x++)
        {
            // To flip horizontally, we access the pixel from the opposite side for the x-coordinate
            int flipped_x = map_width - 1 - x;

            unsigned char* pixel_offset = map_data + (y * map_width + flipped_x) * map_channels;
            float height_value = (pixel_offset[0]) / 200.0f;


            terrain_vertex vtx;
            vtx.position = glm::vec3(x, y, height_value * 15);
            //flip x textcoord
            int flipped_x_tex = texture_width - 1 - x;
            vtx.tex_coords = glm::vec2(flipped_x_tex / (float)texture_width, y / (float)texture_height);

            vertices.push_back(vtx);
        }
    }


    //create indices for a grid of triangles

    for (int y = 0; y < map_height - 1; y++)
    {
        for (int x = 0; x < map_width - 1; x++)
        {
            int top_left = y * map_width + x;
            int top_right = top_left + 1;
            int bottom_left = (y + 1) * map_width + x;
            int bottom_right = bottom_left + 1;

            indices.push_back(top_left);
            indices.push_back(bottom_left);
            indices.push_back(top_right);

            indices.push_back(top_right);
            indices.push_back(bottom_left);
            indices.push_back(bottom_right);

        }
    }


    std::cout << "created " << vertices.size() << " vertices and " << indices.size() << " indices" << std::endl;
}

void world_terrain::init_shaders()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(terrain_vertex), &vertices[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), (void*)0);

    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), (void*)offsetof(terrain_vertex, tex_coords));


    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertex_shader = load_shader(GL_VERTEX_SHADER, "assets/shaders/terrain/terrain_vert.glsl");
    fragment_shader = load_shader(GL_FRAGMENT_SHADER, "assets/shaders/terrain/terrain_frag.glsl");
    shader_program = link_program();

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    model_loc = glGetUniformLocation(shader_program, "model");
    view_loc = glGetUniformLocation(shader_program, "view");
    projection_loc = glGetUniformLocation(shader_program, "projection");
}

GLuint world_terrain::load_shader(GLuint type, const char* path)
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

GLuint world_terrain::link_program()
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

void world_terrain::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shader_program);


    //create a model that's 300x the size of the terrain
    glm::mat4 new_model = glm::scale(model, glm::vec3(800, 800, 800));

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(new_model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_2D, texture_ID);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
}