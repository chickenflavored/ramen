#include "ui.h"

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
}

void ui::update(float dt)
{
}

void ui::set_player_coords(sf::Vector3f coord)
{
	ui_player_coords.setString("player X,Y,Z: " + std::to_string((int)coord.x) + ", " + std::to_string((int)coord.y) + ", " + std::to_string((int)coord.z));
}

void ui::set_fps(int fps)
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


void ui::draw()
{
	w_ptr->draw(ui_player_coords);
	w_ptr->draw(ui_fps);
}