#include "ui.h"

#include <iostream>

ui& ui::global()
{
	static ui instance;
	return instance;
}

void ui::init(sf::RenderWindow* w_ptr)
{
	this->w_ptr = w_ptr;
	font.loadFromFile("assets/fonts/constan.ttf");

	ui_player_coords.setFont(font);
	ui_player_coords.setCharacterSize(16);
	ui_player_coords.setFillColor(sf::Color::White);
	ui_player_coords.setPosition(10, 10);
	ui_player_coords.setString("player coords");

	ui_fps.setFont(font);
	ui_fps.setCharacterSize(16);
	ui_fps.setFillColor(sf::Color::White);
	ui_fps.setPosition(10, 30);
	ui_fps.setString("fps");

	//console
	console_open = false;
	is_wireframe = false;

	console_bg.setSize(sf::Vector2f(w_ptr->getSize().x - 700, 500));
	console_bg.setFillColor(sf::Color(0, 0, 0, 150));
	console_bg.setOutlineThickness(1);
	console_bg.setOutlineColor(sf::Color(255, 255, 255, 200));
	console_bg.setPosition(5, 5);

	console_top_line.setSize(sf::Vector2f(console_bg.getSize().x - 20, 1));
	console_top_line.setFillColor(sf::Color(255, 255, 255, 200));
	console_top_line.setPosition(console_bg.getPosition().x + 10, 30);

	console_bottom_line.setSize(sf::Vector2f(console_bg.getSize().x, 1));
	console_bottom_line.setFillColor(sf::Color(255, 255, 255, 200));
	console_bottom_line.setPosition(5, console_bg.getSize().y - 25);

	console_cursor.setSize(sf::Vector2f(1, 18));
	console_cursor.setFillColor(sf::Color(255, 255, 255, 200));
	console_cursor.setPosition(console_bg.getPosition().x + 5, console_bg.getSize().y - 18);
	cursor_timer.restart();

	console_greeting.setFont(font);
	console_greeting.setCharacterSize(12);
	console_greeting.setFillColor(sf::Color(255, 255, 255, 255));
	console_greeting.setString("* * * ramen * * *");
	console_greeting.setPosition(console_bg.getPosition().x + (console_bg.getSize().x / 2) - (console_greeting.getGlobalBounds().width / 2), 10);

	console_input_text.setFont(font);
	console_input_text.setCharacterSize(12);
	console_input_text.setFillColor(sf::Color(255, 255, 255, 255));
	console_input_text.setPosition(console_bg.getPosition().x + 6, console_bg.getSize().y - 17);

	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		key_states[i] = false;
	}

	last_pressed_key = sf::Keyboard::Unknown;
}

void ui::update(float dt)
{
	ui_player_coords.setPosition(w_ptr->getSize().x - ui_player_coords.getGlobalBounds().width - 10, 10);
	ui_fps.setPosition(w_ptr->getSize().x - ui_fps.getGlobalBounds().width - 10, 30);

	//std::cout << "console: " << console_open << std::endl;
}

void ui::set_player_coords(sf::Vector3f coord)
{
	ui_player_coords.setString("player X,Y,Z: " + std::to_string((int)coord.x) + ", " + std::to_string((int)coord.y) + ", " + std::to_string((int)coord.z));
}

void ui::set_fps_str(int fps)
{
	//set fps string if it's been over a third of a second since last update
	if (fps_clock.getElapsedTime().asSeconds() > 0.33f)
	{
		ui_fps.setString("fps: " + std::to_string(fps));
		fps_clock.restart();
	}
	else
	{
		return;
	}
}

void ui::update_keyboard_state(const sf::Event& evt)
{
	//keyboard
	if (evt.type == sf::Event::KeyPressed)
	{
		key_states[evt.key.code] = true;
		last_pressed_key = evt.key.code;
	}
	else if (evt.type == sf::Event::KeyReleased)
	{
		key_states[evt.key.code] = false;
	}


	//console
	if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Grave)
	{
		console_open = !console_open;

		if (!console_open)
		{
			console_input_text.setString("");
			console_cursor.setPosition(console_input_text.getPosition().x + console_input_text.getGlobalBounds().width + 1, console_cursor.getPosition().y);
		}
	}

	//console typing
	if (console_open)
	{
		if (evt.type == sf::Event::TextEntered)
		{
			//8 is backspace, 96 is grave accent (`)
			if (evt.text.unicode < 128 && evt.text.unicode != 8 && evt.text.unicode != 96)
			{
				console_input_text.setString(console_input_text.getString() + static_cast<char>(evt.text.unicode));
			}
			else if (evt.text.unicode == 8)
			{
				std::string str = console_input_text.getString();
				if (str.size() > 0)
				{
					str.pop_back();
					console_input_text.setString(str);
				}
			}
			console_cursor.setPosition(console_input_text.getPosition().x + console_input_text.getGlobalBounds().width + 1, console_cursor.getPosition().y);

			//if enter is pressed, push the command to the console text vector
			if (evt.text.unicode == 13)
			{
				std::string command = console_input_text.getString();
				console_input_text.setString("");
				console_cursor.setPosition(console_input_text.getPosition().x + console_input_text.getGlobalBounds().width + 1, console_cursor.getPosition().y);

				std::cout << command << std::endl;



				//add the command to the console text vector
				sf::Text new_text;
				new_text.setFont(font);
				new_text.setCharacterSize(12);
				new_text.setFillColor(sf::Color(255, 255, 255, 255));

				//if command contains "wireframe", toggle wireframe
				if (command.find("wireframe") != std::string::npos)
				{
					if (!is_wireframe)
					{
						is_wireframe = true;
					}
					else
					{
						is_wireframe = false;
					}
					is_wireframe ? new_text.setString("wireframe ON") : new_text.setString("wireframe OFF");
				}
				else
				{
					new_text.setString(command);
				}

				new_text.setPosition(console_bg.getPosition().x + 10, console_top_line.getPosition().y + 10 + (console_text.size() * 15));
				console_text.push_back(new_text);
			}
		}
	}
}

bool ui::is_key_pressed(sf::Keyboard::Key key)
{
	return key_states[key];
}

sf::Keyboard::Key ui::get_last_pressed_key()
{
	return last_pressed_key;
}

bool ui::is_console_open()
{
	return console_open;
}

bool ui::set_wireframe()
{
	return is_wireframe;
}

void ui::render()
{
	w_ptr->draw(ui_player_coords);
	w_ptr->draw(ui_fps);

	if (console_open)
	{
		w_ptr->draw(console_bg);
		w_ptr->draw(console_top_line);
		w_ptr->draw(console_bottom_line);
		w_ptr->draw(console_greeting);
		w_ptr->draw(console_input_text);
		if (cursor_timer.getElapsedTime().asSeconds() > 0.5f)
		{
			if (cursor_timer.getElapsedTime().asSeconds() > 1.0f)
			{
				cursor_timer.restart();
			}
			else
			{
				w_ptr->draw(console_cursor);
			}

		}

		for (int i = 0; i < console_text.size(); i++)
		{
			w_ptr->draw(console_text[i]);
		}
	}
}