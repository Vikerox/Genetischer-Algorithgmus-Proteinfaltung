#pragma once
#include "Constants.hpp"
#include "Faltung.h"

#include <vector>
#include <string>
#include <memory>
#include <iterator>

class Faltung;

extern void threadFitness ( std::vector<std::shared_ptr<Faltung>>& parent );

extern void fitness			( std::shared_ptr<Faltung>& faltung, const std::vector<Coord>& coords );
extern int neighbours		( std::shared_ptr<Faltung>& faltung, const std::vector<Coord>& coords  );
extern int intersections	( std::shared_ptr<Faltung>& faltung, const std::vector<Coord>& coords  );


// returns a vector with the selected Faltungen
extern void proportionalSelection ( std::vector<std::shared_ptr<Faltung>>& child, const std::vector<std::shared_ptr<Faltung>> parent,
	const std::vector<double>& cumulativeFitness, double sumFitness);



// Mutates the given Faltung with point mutation and one point crossover
extern void mutation ( std::vector<std::shared_ptr<Faltung>>& child, double mutationRate);



template<typename Vector>
auto split_vector ( const Vector& v, unsigned number_lines ) {
	using Iterator = typename Vector::const_iterator;
	std::vector<Vector> rtn;
	Iterator it = v.cbegin ( );
	const Iterator end = v.cend ( );

	while (it != end) {
		Vector vec;
		std::back_insert_iterator<Vector> inserter ( vec );
		const auto num_to_copy = std::min ( static_cast<unsigned>
			(std::distance ( it, end )), number_lines );
		std::copy ( it, it + num_to_copy, inserter );
		rtn.push_back ( std::move ( vec ) );
		std::advance ( it, num_to_copy );
	}
	return rtn;
}