#ifndef __ui_h__
#define __ui_h__

#include <SFML/Graphics.hpp>

class ui
{
public:
	static ui& global();
	void init(sf::RenderWindow* w_ptr);

	void update(float dt);
	void draw();

	void set_player_coords(sf::Vector3f coord);
	void set_fps(int fps);

private:
	sf::RenderWindow* w_ptr;
	sf::Font font;


	sf::Text ui_player_coords;
	sf::Text ui_fps;

	sf::Clock fps_clock;
};
#endif // __ui_h__