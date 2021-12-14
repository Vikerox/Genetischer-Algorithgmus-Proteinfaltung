#pragma once

#include <vector>

struct Coord
{
	int x{ 0 }, y{ 0 };
	Coord ( int a, int b ) : x ( a ), y ( b ) { }
};

#define SEQ20 { true,false,true,false,false,true,true,false,true,false,false,true,false,true,true,false,false,true,false,true };
#define SEQ24 { true,true,false,false,true,false,false,true,false,false,true,false,false,true,false,false,true,false,false,true,false,false,true,true };
#define SEQ25 { false,false,true,false,false,true,true,false,false,false,false,true,true,false,false,false,false,true,true,false,false,false,false,true,true };
#define SEQ36 { false,false,false,true,true,false,false,true,true,false,false,false,false,false,true,true,true,true,true,true,true,false,false,true,true,false,false,false,false,true,true,false,false,true,false,false };
#define SEQ48 { false,false,true,false,false,true,true,false,false,true,true,false,false,false,false,false,true,true,true,true,true,true,true,true,true,true,false,false,false,false,false,false,true,true,false,false,true,true,false,false,true,false,false,true,true,true,true,true };
#define SEQ50 { true,true,false,true,false,true,false,true,false,true,true,true,true,false,true,false,false,false,true,false,false,false,true,false,false,false,false,true,false,false,false,true,false,false,false,true,false,true,true,true,true,false,true,false,true,false,true,false,true,true };


namespace constant
{
	inline constexpr unsigned int		POPULATION = 50000;
	inline constexpr unsigned int		MAX_GENERATION = 500;
	inline constexpr float				MUTATION_RATE = 0.01f;
	const std::vector<bool>				SEQUENCE = SEQ50;
		
	inline constexpr unsigned int THREAD_COUNT = 10;


	// For splitting the pops on the threads
	inline constexpr int POPS_PER_THREAD = POPULATION / THREAD_COUNT;

	static_assert(POPULATION % THREAD_COUNT == 0, "Uneven Population to thread count ratio");
}
