#ifndef RANDOM_GEN_H
#define RANDOM_GEN_H

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

#include "edge_cases.h"

template <typename T>
void random_gen_basic(std::vector<T>& values, size_t offset = 0);

template <>
inline void random_gen_basic(std::vector<float>& values, size_t offset) {
	if (offset >= values.size()) {
		printf("Error: Offset %zu exceeds size of %zu\n", offset, values.size());
		return;
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> dist;
	
	for (size_t i = offset; i < values.size(); i++) {
		float temp;
		do {
			temp = std::bit_cast<float>(dist(gen));
		} while(!std::isfinite(temp));
		values[i] = temp;
	}
}

template <>
inline void random_gen_basic(std::vector<double>& values, size_t offset) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<uint64_t> dist;

	for (size_t i = offset; i < values.size(); i++) {
		double temp;
		do {
			temp = std::bit_cast<double>(dist(gen));
		} while(!std::isfinite(temp));
		values[i] = temp;
	}
}

#endif /* RANDOM_GEN_H */
