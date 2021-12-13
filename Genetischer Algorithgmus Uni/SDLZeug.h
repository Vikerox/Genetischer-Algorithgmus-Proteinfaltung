#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>

#include "Faltung.h"

class SDLZeug
{
private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	int m_screenWidth{};
	int m_screenHeight{};

	friend class DrawWithThread;

public:
	SDLZeug ( int width = 1000, int height = 1000 ) : m_screenWidth ( width ), m_screenHeight ( height ) {};
	~SDLZeug ( ) 
	{
		TTF_Quit ( );
		SDL_DestroyRenderer ( m_renderer );
		SDL_DestroyWindow ( m_window );
		SDL_Quit ( );
	};

	void init ( );
	void eventHandler ( );
	void saveAsPng ( int gen );
};

class DrawWithThread
{
public:
	static void draw ( Faltung& faltung, const std::vector<std::string>& data, SDLZeug* sdlInfo );
};
