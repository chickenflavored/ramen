#ifndef __ui_h__
#define __ui_h__

#include <SFML/Graphics.hpp>

#include <vector>

class ui
{
public:
	static ui& global();
	void init(sf::RenderWindow* w_ptr);

	void update(float dt);
	void render();

	void set_player_coords(sf::Vector3f coord);
	void set_fps_str(int fps);

	void update_keyboard_state(const sf::Event& evt);
	bool is_key_pressed(sf::Keyboard::Key key);
	sf::Keyboard::Key get_last_pressed_key();

	bool is_console_open();
	bool set_wireframe();

private:
	sf::RenderWindow* w_ptr;
	sf::Font font;

	sf::Text ui_player_coords;
	sf::Text ui_fps;

	sf::Clock fps_clock;

	//console
	bool console_open;
	sf::RectangleShape console_bg;
	sf::RectangleShape console_top_line;
	sf::RectangleShape console_bottom_line;
	sf::RectangleShape console_cursor;
	sf::Text console_greeting;
	sf::Clock cursor_timer;
	sf::Text console_input_text;
	std::vector<sf::Text> console_text;

	//eventually replace this bool with an observer pattern
	bool is_wireframe;

	//keyboard
	bool key_states[sf::Keyboard::KeyCount];
	sf::Keyboard::Key last_pressed_key;

};
#endif // __ui_h__