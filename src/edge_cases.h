#ifndef EDGE_CASES_H
#define EDGE_CASES_H

#include <cmath>
#include <limits>
#include <array>

template<typename T>
static inline T nextup(T x) {
	return std::nextafter(x, std::numeric_limits<T>::infinity());
}

template<typename T>
static inline T nextdown(T x) {
	return std::nextafter(x, -std::numeric_limits<T>::infinity());
}

template<typename T>
std::array edge_cases = {
	static_cast<T>(0.0),
	std::numeric_limits<T>::denorm_min(),
	std::numeric_limits<T>::min(),	
	static_cast<T>(0.5),
	static_cast<T>(1.0),
	static_cast<T>(2.0),
	std::numeric_limits<T>::max(),
	std::numeric_limits<T>::infinity(),
	std::numeric_limits<T>::quiet_NaN(),
	std::numeric_limits<T>::signaling_NaN(),
	
	nextup(static_cast<T>(0.5)),
	nextup(static_cast<T>(1.0)),
	nextup(static_cast<T>(2.0)),
	nextdown(static_cast<T>(0.5)),
	nextdown(static_cast<T>(1.0)),
	nextdown(static_cast<T>(2.0)),

	nextup(std::numeric_limits<T>::denorm_min()),
	nextup(std::numeric_limits<T>::min()),
	nextdown(std::numeric_limits<T>::min()),
	nextdown(std::numeric_limits<T>::max()),

	-static_cast<T>(0.0),
	-std::numeric_limits<T>::denorm_min(),
	-std::numeric_limits<T>::min(),	
	-static_cast<T>(0.5),
	-static_cast<T>(1.0),
	-static_cast<T>(2.0),
	-std::numeric_limits<T>::max(),
	-std::numeric_limits<T>::infinity(),
	-std::numeric_limits<T>::quiet_NaN(),
	-std::numeric_limits<T>::signaling_NaN(),
	
	-nextup(static_cast<T>(0.5)),
	-nextup(static_cast<T>(1.0)),
	-nextup(static_cast<T>(2.0)),
	-nextdown(static_cast<T>(0.5)),
	-nextdown(static_cast<T>(1.0)),
	-nextdown(static_cast<T>(2.0)),

	-nextup(std::numeric_limits<T>::denorm_min()),
	-nextup(std::numeric_limits<T>::min()),
	-nextdown(std::numeric_limits<T>::min()),
	-nextdown(std::numeric_limits<T>::max()),
};

#endif /* EDGE_CASES_H */
