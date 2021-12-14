#include "Utilities.hpp"

#include <random>
#include <ctime>

std::mt19937 randomEngine ( (unsigned int)time ( nullptr ) );


void threadFitness ( std::vector<std::shared_ptr<Faltung>>& pops )
{
	for (auto& it : pops)
	{
		auto& coordinates = it->generateCoordinates ( );
		fitness ( it, coordinates );
	}
}

void fitness ( std::shared_ptr<Faltung>& faltung, const std::vector<Coord>& coords )
{
	static const int hps = std::count ( constant::SEQUENCE.begin ( ), constant::SEQUENCE.end ( ), true );

	int neigh{ neighbours ( faltung, coords ) }, inter{ intersections ( faltung, coords ) };

	// variables mainly for code readability
	double x, z;

	// v1
	// normalizing the cross over to a max of 1
	// if number crossovers = 0 and neighbours not 0 set x to 0 to avoid going into
	// the negative
	// x = (c == 0 && n != 0) ? 0 : -(1 / (c + 0.1f)) + 1;

	// normalizing the neighbour to be 1 at its lowest
	// y = (1 / (n + 0.1f)) + 1;

	// faltung->setFitness ( 1 / (x + y) );

	// v2
	x = ((neigh == 0) ? 1 : neigh);
	//y = ((c == 0) ? 1 : c);
	z = hps * 18.0f / hps;
	faltung->setFitness ( 1 / (z / x + inter) );
}

int neighbours ( std::shared_ptr<Faltung>& faltung, const std::vector<Coord>& coords )
{
	int numNeigh{ 0 };

	for (int i = 0; i < constant::SEQUENCE.size ( ); ++i)
	{
		for (int j = i + 2; j < constant::SEQUENCE.size ( ); ++j)
		{
			if(constant::SEQUENCE[i] == constant::SEQUENCE[j] && constant::SEQUENCE[i] )
			{
				if (coords[i].x == coords[j].x + 1 && coords[i].y == coords[j].y)	faltung->addNeighbour ( std::make_pair ( coords[i], coords[j] ) );
				else if (coords[i].x == coords[j].x - 1 && coords[i].y == coords[j].y)	faltung->addNeighbour ( std::make_pair ( coords[i], coords[j] ) );
				else if (coords[i].x == coords[j].x && coords[i].y == coords[j].y + 1)	faltung->addNeighbour ( std::make_pair ( coords[i], coords[j] ) );
				else if (coords[i].x == coords[j].x && coords[i].y == coords[j].y - 1)	faltung->addNeighbour ( std::make_pair ( coords[i], coords[j] ) );
				else continue;
				++numNeigh;
			}
		}
	}

	return numNeigh;
}

int intersections ( std::shared_ptr<Faltung>& faltung, const std::vector<Coord>& coords )
{
	int numOfIntersections = 0;

	for (int i = 0; i < constant::SEQUENCE.size ( ); ++i)
	{
		for (int j = i + 1; j < constant::SEQUENCE.size ( ); ++j)
		{
			if (coords[i].x == coords[j].x && coords[i].y == coords[j].y)
			{
				++numOfIntersections;
				faltung->addIntersection ( coords[i] );
			}
		}
	}
	return numOfIntersections;
}


void proportionalSelection ( std::vector<std::shared_ptr<Faltung>>& child, const std::vector<std::shared_ptr<Faltung>> parent,
	const std::vector<double>& cumulativeFitness, double sumFitness)
{
	size_t cumSize = cumulativeFitness.size ( );
	
	std::vector<std::shared_ptr<Faltung>> result{};
	result.resize ( child.size ( ) );

	for (int it = 0; it < child.size(); ++it)
	{
		double rand = (double)randomEngine ( ) / (double)randomEngine.max ( );

		for (int i = 1; i < cumSize; ++i)
		{
			if (i == cumSize - 1)
			{
				result[it] = parent[i];
				break;
			}
			else if (cumulativeFitness[i] / sumFitness > rand)
			{
				result[it] = parent[i - 1];
				break;
			}
		}
	}

	child.clear ( );
	child.resize ( result.size ( ) );

	for (int i = 0; i < result.size(); ++i)
	{
		child[i] = result[i];
	}
}

void mutation ( std::vector<std::shared_ptr<Faltung>>& child, double mutationRate)
{
	std::vector<std::shared_ptr<Faltung>> result;
	result.resize ( child.size ( ) );
	for (int i = 0; i < child.size ( ); ++i)
		result[i] = (std::shared_ptr<Faltung>) new Faltung ( *child[i] );
	
	for (int i = 0; i < child.size ( ); ++i)
	{
		double mutWant = (double)randomEngine ( ) / (double)randomEngine.max ( );
		if (mutWant <= mutationRate)
		{
			// random element in protein sequence
			int r = (int) (((double)randomEngine ( ) / (double)randomEngine.max ( )) * child[0]->getDirection ( ).size ( ));

			// Punktmutation wenn r < 0.5 oder nur ein Element da ist
			double mutKind = (double)randomEngine ( ) / (double)randomEngine.max ( );

			if (mutKind < 0.5 || i == 0 )
			{
				int t = randomEngine ( ) % 3;
				result[i]->setDirectionAt ( r, (t == 0) ? -1 : ((t == 1) ? 0 : 1) );
			}
			else
			{
				int secFalt = randomEngine ( ) % child.size ( );
				if (secFalt == i) secFalt -= 1;

				for (int j = 0; j <= r; ++j)
				{
					char e1 = child[i]->getDirectionAt ( j );
					char e2 = child[secFalt]->getDirectionAt ( j );
					result[i]->setDirectionAt ( j, e2 );
					result[secFalt]->setDirectionAt ( j, e1 );
				}
			}
		}
	}

	child.clear ( );
	child.resize ( result.size ( ) );

	for (int i = 0; i < result.size ( ); ++i)
	{
		child[i] = result[i];
	}
}
