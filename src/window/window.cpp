#include "window.h"

#include "../gl/glad.h"

#include <iostream>
#include <string>



window::window()
{
	window_width = 1600;
	window_height = 900;
	dt = 0.0f;
	
	sf_window = nullptr;


	focus = true;
}

window::~window()
{
}

void window::init()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	
	//opengl ver
	settings.majorVersion = 4;
	settings.minorVersion = 6;

	sf_window = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "ramen", sf::Style::Close, settings);


	sf_window->setVerticalSyncEnabled(true);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	renderer::global().init(sf_window);
	ui::global().init(sf_window);

	run();
}

void window::run()
{
	sf::Clock clock;

	while (sf_window->isOpen())
	{
		dt = clock.restart().asMilliseconds();
		poll_events();
		update(dt);
	}
}

void window::poll_events()
{
	sf::Event evt;
	while (sf_window->pollEvent(evt))
	{
		if (evt.type == sf::Event::Closed)
		{
			sf_window->close();
		}

		if (evt.type == sf::Event::GainedFocus)
		{
			focus = true;
		}

		if (evt.type == sf::Event::LostFocus)
		{
			focus = false;
		}

		ui::global().update_keyboard_state(evt);
	}
}

void window::update(float dt)
{

	//set ui player coords to player coords
	ui::global().set_player_coords(renderer::global().get_player_coords());

	//set ui fps to fps
	ui::global().set_fps_str(1000 / dt);

	if (focus)
	{
		renderer::global().update(dt);
		ui::global().update(dt);
	}

	renderer::global().render();
	sf_window->pushGLStates();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ui::global().render();
	sf_window->popGLStates();
	sf_window->display();
}