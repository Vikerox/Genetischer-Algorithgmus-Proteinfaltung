#include "SDLZeug.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <filesystem>

#include "Utilities.hpp"

const int SCALE = 50;

const int SIZE_RECT = SCALE / 2;
const int RECT_OFFSET = SIZE_RECT / 2;
const int NEIGHBOUR_OFFSET =  4 + RECT_OFFSET;

void SDLZeug::init ( )
{
	SDL_Init ( SDL_INIT_VIDEO );
	TTF_Init ( );
	m_window = SDL_CreateWindow ( "Genetic Algorithm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL );

	SDL_SetHint ( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );

	m_renderer = SDL_CreateRenderer ( m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	// delete all content in the images folder 
	for (const auto& entry : std::filesystem::directory_iterator ( "Images/" ))
		std::filesystem::remove_all ( entry.path ( ) );
}

void SDLZeug::eventHandler ( )
{
	// Poll for events
	SDL_Event ev;
	while (SDL_PollEvent ( &ev ))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			break;
		}
	}
}

/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/

void SDLZeug::saveAsPng ( int gen )
{
	SDL_Surface* surface = SDL_CreateRGBSurface ( 0, m_screenWidth, m_screenHeight, 32, 0, 0, 0, 0 );

	SDL_RenderReadPixels ( m_renderer, NULL, surface->format->format, surface->pixels, surface->pitch );

	if (surface == nullptr)
		return;

	std::string fp = "Images/" + std::to_string ( gen ) + ".png";

	IMG_SavePNG ( surface, fp.c_str ( ) );

	SDL_FreeSurface ( surface );
}

/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/

void DrawWithThread::draw ( Faltung& faltung, const std::vector<std::string>& data, SDLZeug* sdlInfo )
{
	SDL_SetRenderDrawColor ( sdlInfo->m_renderer, 255, 255, 255, 255 );
	SDL_RenderClear ( sdlInfo->m_renderer );

	// draw the lines
	std::vector<Coord> coords = faltung.getCoordinates ( );
	SDL_SetRenderDrawColor ( sdlInfo->m_renderer, 0, 0, 0, 255 );

	for (unsigned int i = 0; i < coords.size ( ) - 1; ++i)
	{
		SDL_RenderDrawLine ( sdlInfo->m_renderer,
			coords[i].x * SCALE + RECT_OFFSET + sdlInfo->m_screenWidth / 2,
			coords[i].y * SCALE + RECT_OFFSET + sdlInfo->m_screenHeight / 2,
			coords[i + 1].x * SCALE + RECT_OFFSET + sdlInfo->m_screenWidth / 2,
			coords[i + 1].y * SCALE + RECT_OFFSET + sdlInfo->m_screenHeight / 2 );
	}

	// draw the proteins
	SDL_SetRenderDrawColor ( sdlInfo->m_renderer, 0, 0, 0, 255 );
	int i = 0;
	for (auto& it : coords)
	{
		SDL_Rect* rect = new SDL_Rect{
			it.x * SCALE + sdlInfo->m_screenWidth / 2 ,
			it.y * SCALE + sdlInfo->m_screenHeight / 2,
			SIZE_RECT,
			SIZE_RECT };
		if (constant::SEQUENCE[i])
			SDL_RenderFillRect ( sdlInfo->m_renderer, rect );
		else
			SDL_RenderDrawRect ( sdlInfo->m_renderer, rect );

		delete rect;
		++i;
	}

	// enclose the neighbours in a rectangle
	auto neigh = faltung.getNeighbours ( );
	SDL_SetRenderDrawColor ( sdlInfo->m_renderer, 0, 0, 255, 255 );

	for (auto& it : neigh)
	{
		Coord start{ it.first }, end{ it.second };

		Coord dir ( end.x - start.x, end.y - start.y );

		int prefixX{ (dir.x < 0) ? -1 : 1 }, prefixY{ (dir.y < 0) ? -1 : 1 };

		Coord rectStart = {
			start.x * SCALE + RECT_OFFSET + -prefixX * NEIGHBOUR_OFFSET + sdlInfo->m_screenWidth / 2,
			start.y * SCALE + RECT_OFFSET + -prefixY * NEIGHBOUR_OFFSET + sdlInfo->m_screenHeight / 2 };
		Coord rectEnd = {
			end.x * SCALE + RECT_OFFSET + prefixX * NEIGHBOUR_OFFSET + sdlInfo->m_screenWidth / 2,
			end.y * SCALE + RECT_OFFSET + prefixY * NEIGHBOUR_OFFSET + sdlInfo->m_screenHeight / 2 };

		rectangleRGBA ( sdlInfo->m_renderer, (Sint16)rectStart.x, (Sint16)rectStart.y, (Sint16)rectEnd.x, (Sint16)rectEnd.y, 0, 0, 255, 255 );
	}

	// draw a red dot at intersections
	auto inter = faltung.getIntersections ( );
	for (auto& it : inter)
	{
		filledCircleRGBA ( sdlInfo->m_renderer,
			(Sint16)(it.x * SCALE + RECT_OFFSET + sdlInfo->m_screenWidth / 2),
			(Sint16)(it.y * SCALE + RECT_OFFSET + sdlInfo->m_screenHeight / 2),
			3, 255, 0, 0, 255 );
	}

	// draw the data
	// formatting the data
	std::vector<std::string> stats = {
		"Generation:         " + data[0],
		"Avg Fitness:        " + data[1],
		"Best Fitness:       " + data[2],
		"Best Fitness (all): " + data[3],
		"Hydrophob pairs:    " + data[4],
		"Intersections:      " + data[5] };

	TTF_Font* font = TTF_OpenFont ( "Fonts/consolab.ttf", 25 );

	int textVertOffset = 0;
	SDL_Color color = { 64, 64, 64, 255 };
	for (auto& it : stats)
	{
		SDL_Surface* surface = TTF_RenderText_Solid ( font, it.c_str ( ), color );
		SDL_Texture* texture = SDL_CreateTextureFromSurface ( sdlInfo->m_renderer, surface );

		int texW{ 0 }, texH{ 0 };
		SDL_QueryTexture ( texture, NULL, NULL, &texW, &texH );
		SDL_Rect destRect = { 0, textVertOffset, texW, texH };

		SDL_RenderCopy ( sdlInfo->m_renderer, texture, NULL, &destRect );

		textVertOffset += texH;
		SDL_DestroyTexture ( texture );
		SDL_FreeSurface ( surface );
	}

	TTF_CloseFont ( font );

	SDL_RenderPresent ( sdlInfo->m_renderer );

	SDL_GL_SwapWindow ( sdlInfo->m_window );
}
