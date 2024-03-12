#ifndef _DEBUG
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

//#include <SFML/Graphics.hpp>
//
//#include <iostream>
//
//#include "gl/glad.h"
//
//const char* vertexShaderSource = R"(
//#version 460 core
//layout (location = 0) in vec3 aPos;
//
//void main()
//{
//    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
//}
//)";
//
//const char* fragmentShaderSource = R"(
//#version 460 core
//out vec4 FragColor;
//
//void main()
//{
//    FragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f); // orange color
//}
//)";
//
//int main()
//{
//	//setup window for opengl rendering
//	sf::ContextSettings settings;
//	settings.depthBits = 24;
//	settings.stencilBits = 8;
//	settings.antialiasingLevel = 2;
//	settings.majorVersion = 4;
//	settings.minorVersion = 6;
//
//	sf::RenderWindow window(sf::VideoMode(800, 600), "ramen", sf::Style::Close, settings);
//
//    window.setVerticalSyncEnabled(true);
//
//	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction)))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//    GLuint vao, vbo;
//    glGenVertexArrays(1, &vao);
//    glGenBuffers(1, &vbo);
//
//    // Vertex data for a triangle
//    GLfloat vertices[] = {
//        // Positions
//        -0.5f, -0.5f, 0.0f,
//        0.5f, -0.5f, 0.0f,
//        0.0f, 0.5f, 0.0f
//    };
//
//    // Bind the vertex array object
//    glBindVertexArray(vao);
//
//    // Bind the vertex buffer object and fill it with vertex data
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Enable vertex attribute array for position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Unbind vertex array object
//    glBindVertexArray(0);
//
//    // Create vertex and fragment shaders
//    GLuint vertexShader, fragmentShader;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//    // Set shader source code
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//
//    // Compile shaders
//    glCompileShader(vertexShader);
//    glCompileShader(fragmentShader);
//
//    // Check for shader compilation errors
//    GLint success;
//    GLchar infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
//        std::cout << "Error compiling vertex shader: " << infoLog << std::endl;
//    }
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
//        std::cout << "Error compiling fragment shader: " << infoLog << std::endl;
//    }
//
//    // Create shader program
//    GLuint shaderProgram = glCreateProgram();
//
//    // Attach shaders to program
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//
//    // Link shaders
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//	sf::Font font;
//	if (!font.loadFromFile("assets/fonts/constan.ttf"))
//	{
//	}
//
//	sf::Text text;
//	text.setFont(font);
//	text.setString("figs");
//	text.setCharacterSize(16);
//	text.setFillColor(sf::Color::White);
//	text.setPosition(100, 100);
//
//
//
//	//main loop
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			//close window
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//       // window.setActive(true);
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//
//        // Set viewport to match window size
//        glViewport(0, 0, window.getSize().x, window.getSize().y);
//
//        // Bind the vertex array object
//        glBindVertexArray(vao);
//
//        // Draw the triangle
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glUseProgram(0);
//        glBindVertexArray(0);
//
//        window.pushGLStates();
//
//
//		//draw stuff
//		//...
//		window.draw(text);
//
//        window.popGLStates();
//
//
//        //window.setActive(false);
//
//
//		//display window
//		window.display();
//	}
//
//    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
//    glDeleteProgram(shaderProgram);
//
//	return 0;
//}

#include "window/window.h"

int main()
{
	window* wnd = new window();
	wnd->init();
	delete wnd;
	return 0;
}