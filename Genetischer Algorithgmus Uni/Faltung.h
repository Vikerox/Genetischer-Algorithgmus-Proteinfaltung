#pragma once

#include <vector>
#include <string>
#include <bitset>

#include "Utilities.hpp"

class Faltung
{
private:
	std::vector<char> m_direction{};
	double m_fitness = 0;
	std::vector<std::pair<Coord, Coord>> m_neighbours{};
	std::vector<Coord> m_intesect{};
	std::vector<Coord> m_coordinates{};
	void randomize ( );

public:
	Faltung ( bool random = false );
	Faltung ( const Faltung& f );
	~Faltung ( ) {};

	auto getDirectionAt		( int index )	const -> char									{ return m_direction[index]; }
	auto getNeighbours		( )				const -> std::vector<std::pair<Coord, Coord>>	{ return m_neighbours; }
	auto getIntersections	( )				const -> std::vector<Coord>						{ return m_intesect; }
	auto getDirection		( )				const -> std::vector<char>						{ return m_direction; }
	auto getFitness			( )				const -> double									{ return m_fitness; }
	auto getCoordinates		( )				const -> std::vector<Coord>						{ return m_coordinates; };

	std::string directionToString ( );

	std::vector<Coord>& generateCoordinates ( );

	void addIntersection ( Coord inter ) { m_intesect.push_back ( inter ); }
	void addNeighbour ( std::pair<Coord, Coord> neigh ) { m_neighbours.push_back ( neigh ); }

	void setDirectionAt ( int index, char val ) { m_direction[index] = val; }
	void setFitness ( double fit ) { m_fitness = fit; }
	void clear ( );
};

