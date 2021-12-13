#include "Faltung.h"

#include <random>
#include <ctime>
#include <sstream>

void Faltung::randomize ( )
{
	static std::mt19937 randomEngine ( (unsigned int)time ( nullptr ) );

	for (int i = 0; i < m_direction.size ( ); ++i) {
		int n = randomEngine ( ) % 3;
		switch (n) {
		case 0:
			m_direction[i] = 1;
			break;
		case 1:
			m_direction[i] = 0;
			break;
		case 2:
			m_direction[i] = -1;
			break;
		default:
			break;
		}
	}
}

Faltung::Faltung ( bool random )
{
	for (int i = 0; i < SEQUENCE.size ( ) - 1; ++i) {
		m_direction.push_back ( 0 );
	}
	if (random)
	{
		randomize ( );
	}
}

Faltung::Faltung ( const Faltung& f )
{
	m_direction = f.getDirection ( );
}

std::string Faltung::directionToString ( )
{
	std::stringstream ss;
	for (int i = 0; i < m_direction.size ( ); ++i)
		ss << m_direction[i] << " ";
	ss << "\n";
	return ss.str ( );
}

void Faltung::generateCoordinates (  )
{
	m_coordinates.push_back ( Coord( 0,0 ) );
	unsigned int dir = 1;
	for (int i = 0; i < m_direction.size ( ); ++i)
	{
		Coord temp = m_coordinates[m_coordinates.size ( ) - 1];
		//if (m_direction[i] == 's')
		//{
		//	// dir = dir;
		//}
		//if (m_direction[i] == 'r')
		//{
		//	++dir;
		//	if (dir > 3)
		//		dir = 0;
		//}
		//if (m_direction[i] == 'l')
		//{
		//	--dir;
		//	if (dir < 0)
		//		dir = 3;
		//}

		dir = (dir + m_direction[i]) % 4;

		switch (dir)
		{
		case 0:
			++temp.y;
			break;
		case 1:
			++temp.x;
			break;
		case 2:
			--temp.y;
			break;
		case 3:
			--temp.x;
			break;
		default:
			break;
		}

		m_coordinates.push_back ( temp );
	}
}

void Faltung::clear ( )
{
	m_neighbours = {};
	m_intesect = {};
	m_coordinates = {};
}
