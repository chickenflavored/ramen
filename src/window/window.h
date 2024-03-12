#ifndef __window_h__
#define __window_h__

#include <SFML/Graphics.hpp>

//renderer and ui call init in window.cpp. afterwards they are globally accessible
#include "../renderer/renderer.h"
#include "../ui/ui.h"

class window
{
public:
	window();
	~window();

	void init();
private:
	void run();

	void update(float dt);

	void poll_events();

	sf::RenderWindow* sf_window;
	int16_t window_width;
	int16_t window_height;

	float dt;

	bool focus;
};

#endif // __window_h__