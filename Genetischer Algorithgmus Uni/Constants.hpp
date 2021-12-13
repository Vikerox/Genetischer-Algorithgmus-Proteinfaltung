#pragma once

#include <vector>

struct Coord
{
	int x{ 0 }, y{ 0 };
	Coord ( int a, int b ) : x ( a ), y ( b ) { }
};

const struct SEQUENCES
{
	const std::vector<bool> SEQ20 = { true,false,true,false,false,true,true,false,true,false,false,true,false,true,true,false,false,true,false,true };
	const std::vector<bool> SEQ24 = { true,true,false,false,true,false,false,true,false,false,true,false,false,true,false,false,true,false,false,true,false,false,true,true };
	const std::vector<bool> SEQ25 = { false,false,true,false,false,true,true,false,false,false,false,true,true,false,false,false,false,true,true,false,false,false,false,true,true };
	const std::vector<bool> SEQ36 = { false,false,false,true,true,false,false,true,true,false,false,false,false,false,true,true,true,true,true,true,true,false,false,true,true,false,false,false,false,true,true,false,false,true,false,false };
	const std::vector<bool> SEQ48 = { false,false,true,false,false,true,true,false,false,true,true,false,false,false,false,false,true,true,true,true,true,true,true,true,true,true,false,false,false,false,false,false,true,true,false,false,true,true,false,false,true,false,false,true,true,true,true,true };
	const std::vector<bool> SEQ50 = { true,true,false,true,false,true,false,true,false,true,true,true,true,false,true,false,false,false,true,false,false,false,true,false,false,false,false,true,false,false,false,true,false,false,false,true,false,true,true,true,true,false,true,false,true,false,true,false,true,true };
}seq;




const unsigned int		POPULATION		= 50000;
const unsigned int		MAX_GENERATION	= 500;
const float				MUTATION_RATE	= 0.01f;
const std::vector<bool>	SEQUENCE		= seq.SEQ50;

const unsigned int THREAD_COUNT = 10;


// For splitting the pops on the threads
const int POPS_PER_THREAD = POPULATION / THREAD_COUNT;

static_assert(POPULATION % THREAD_COUNT == 0, "uneven Population to thread count ratio");
