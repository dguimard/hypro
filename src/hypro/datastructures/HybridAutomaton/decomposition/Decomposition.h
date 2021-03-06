#pragma once
#include <vector>

namespace hypro {

struct Decomposition {
	std::vector<std::vector<std::size_t>> mDecomposition;

	std::size_t size() const { return mDecomposition.size(); }

	void push_back( const std::vector<std::size_t>& decomp ) { mDecomposition.push_back( decomp ); }
	const std::vector<std::size_t>& at( std::size_t pos ) const { return mDecomposition.at( pos ); }

	std::vector<std::vector<std::size_t>>::iterator begin() { return mDecomposition.begin(); }
	std::vector<std::vector<std::size_t>>::const_iterator begin() const { return mDecomposition.begin(); }
	std::vector<std::vector<std::size_t>>::iterator end() { return mDecomposition.end(); }
	std::vector<std::vector<std::size_t>>::const_iterator end() const { return mDecomposition.end(); }
};

}  // namespace hypro
