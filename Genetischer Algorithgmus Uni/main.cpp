#define SDL_MAIN_HANDLED

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include <atomic>

#include <thread_pool.hpp>

#include <SDL2/SDL.h>

#include "Utilities.hpp"
#include "WriteToTSV.h"
#include "Faltung.h"
#include "SDLZeug.h"

using namespace std::chrono;

void parallelProcessing ( Faltung& bestFaltung, std::vector<std::string>& data, SDLZeug* output, int generation, bool newBest, WriteToTSV& tsv );

int main ( int argc, char* argv[] )
{
	WriteToTSV tsv ( std::vector<std::string> {
		"Generation", "Average Fitness", 
			"Best Fitness (this generation)", 
			"Best Fitness (all)", 
			"Neighbours", 
			"Intersections" } );
	
	// initzialize SDl
	SDLZeug* output = new SDLZeug;
	output->init ( );

	// spawn population and randomize it
	std::vector<std::shared_ptr<Faltung>> population{};
	for (int i = 0; i < POPULATION; ++i)
		population.emplace_back ( new Faltung ( true ) );

	// initialize the Thread Pool
	thread_pool threadPoolThreads ( THREAD_COUNT );

	double bestFitnessOverall = 0;
	int generation = 0;

	microseconds durationFitness ( microseconds ( 0 ) );
	microseconds durationSelection ( microseconds ( 0 ) );
	microseconds durationMutation ( microseconds ( 0 ) );
	microseconds durationDraw ( microseconds ( 0 ) );
	microseconds durationGeneration ( microseconds ( 0 ) );

	while (generation < MAX_GENERATION)
	{
		auto startGen = high_resolution_clock::now ( );

		double bestFitnessGenerational = 0;
		double fitnessSum = 0;
		Faltung bestFaltung;
		bool newBest = false;
		std::vector<double> cumulativeFitness{};

		// loop to calculate fitness
		auto start = high_resolution_clock::now ( );

/*****************************************  FITNESS  **********************************************************************************************/

		auto vOut = split_vector ( population, POPS_PER_THREAD );

		for (int i = 0; i < THREAD_COUNT; ++i)
			threadPoolThreads.push_task ( threadFitness, std::ref ( vOut[i] ) );

		population.clear ( );

		threadPoolThreads.wait_for_tasks ( );

		for (auto& it : vOut)
			population.insert ( population.end ( ), it.begin ( ), it.end ( ) );

		auto stop = high_resolution_clock::now ( );
		durationFitness += duration_cast<microseconds>(stop - start);

		// loop for sum, comulative, average and best fitness
		for (int i = 0; i < POPULATION; ++i)
		{
			double fit = population[i]->getFitness ( );

			if (fit > bestFitnessGenerational)
			{
				bestFitnessGenerational = fit;
				bestFaltung = *population[i];

				if (fit > bestFitnessOverall)
				{
					bestFitnessOverall = fit;
					newBest = true;

				}
			}
			cumulativeFitness.push_back ( fitnessSum );
			fitnessSum += fit;
		}
		double averageFitness = fitnessSum / POPULATION;

		// Format generation Data into a string vector
		std::vector<std::string> data{ std::to_string ( generation ),
			std::to_string ( averageFitness ), std::to_string ( bestFitnessGenerational ), std::to_string ( bestFitnessOverall ),
			std::to_string ( bestFaltung.getNeighbours ( ).size ( ) ), std::to_string ( bestFaltung.getIntersections ( ).size ( ) ) };

/*****************************************  SELECTION  **********************************************************************************************/
		
		// select
		start = high_resolution_clock::now ( );

		vOut = split_vector ( population, POPS_PER_THREAD );

		for (int i = 0; i < THREAD_COUNT; ++i)
			threadPoolThreads.push_task ( proportionalSelection,
				std::ref ( vOut[i] ),
				std::ref ( population ),
				std::ref ( cumulativeFitness ),
				fitnessSum );

		// draw
		auto startD = high_resolution_clock::now ( );

/*****************************************  Starting parallelized processes  **********************************************************************************************/

		std::thread drawThread ( parallelProcessing, std::ref ( bestFaltung ), std::ref ( data ), output, generation, newBest, std::ref ( tsv ) );

		auto stopD = high_resolution_clock::now ( );
		durationDraw += duration_cast<microseconds>(stopD - startD);

		// clear the neighbour and intersection part in the population
		for (auto& it : population)
			it->clear ( );

		

		threadPoolThreads.wait_for_tasks ( );

		stop = high_resolution_clock::now ( );
		durationSelection += duration_cast<microseconds>(stop - start);



		// mutate
		start = high_resolution_clock::now ( );

/*****************************************  Mutation  **********************************************************************************************/

		for (int i = 0; i < THREAD_COUNT; ++i)
			threadPoolThreads.push_task ( std::bind ( mutation, std::ref ( vOut[i] ), MUTATION_RATE ) );

		population.clear ( );

		threadPoolThreads.wait_for_tasks ( );

		for (auto& it : vOut)
			population.insert ( population.end ( ), it.begin ( ), it.end ( ) );

		stop = high_resolution_clock::now ( );
		durationMutation += duration_cast<microseconds>(stop - start);

		output->eventHandler ( );

		//output->draw ( &bestFaltung, data );
		drawThread.join ( );

		auto stopGen = high_resolution_clock::now ( );
		durationGeneration += duration_cast<microseconds>(stopGen - startGen);

		++generation;
	}

	output->saveAsPng ( generation -1 );

	delete output;
	std::string filepath{ "TSV Output/" };
	filepath += "logSEQ" + std::to_string ( SEQUENCE.size() ) + 
		"Pops" + std::to_string ( POPULATION ) + 
		"Gen" + std::to_string ( generation ) + 
		"Mut" + std::to_string ( MUTATION_RATE ) + ".tsv";
	tsv.output ( filepath );

	std::cout 
		<< "Fitness:      " << (double)durationFitness.count ( ) / generation << std::endl
		<< "Selection:    " << (double)durationSelection.count ( ) / generation << std::endl
		<< "Mutation:     " << (double)durationMutation.count ( ) / generation << std::endl
		<< "Fitness pp:   " << (double)durationFitness.count ( ) / (POPULATION * generation) << std::endl
		<< "Selection pp: " << (double)durationSelection.count ( ) / (POPULATION * generation) << std::endl
		<< "Mutation pp:  " << (double)durationMutation.count ( ) / (POPULATION * generation) << std::endl
		<< "Draw:         " << (double)durationDraw.count ( ) / generation << std::endl
		<< "Generation:   " << (double)durationGeneration.count ( ) / generation << std::endl
		<< "Framerate:    " << 1000000.0f / ((double)durationGeneration.count ( ) / generation) << std::endl;

	// getchar ( );
	return 0;
}

void parallelProcessing ( Faltung& bestFaltung, std::vector<std::string>& data, SDLZeug* output, int generation, bool newBest, WriteToTSV& tsv )
{
	DrawWithThread::draw ( std::ref ( bestFaltung ), std::ref ( data ), output );

	if (generation % 20 == 0 || newBest)
		output->saveAsPng ( generation );

	// save for TSV
	tsv.addDataLine ( data );
}
