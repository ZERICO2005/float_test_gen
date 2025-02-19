#ifndef EXPORT_VALUE_H
#define EXPORT_VALUE_H

#include <string>
#include <bit>
#include <cinttypes>
#include <cstring>

template<typename T>
std::string export_value(const T& value);

template<>
inline std::string export_value(const std::string& value) {
	return value;
}

template<>
inline std::string export_value(const double& value) {
	std::string ret;
	char buf[256];
	std::memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "UINT64_C(0x%016" PRIX64 ")", std::bit_cast<uint64_t>(value));
	ret.assign(buf);
	return ret;
}

template<>
inline std::string export_value(const uint64_t& value) {
	std::string ret;
	char buf[256];
	std::memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "UINT64_C(0x%016" PRIX64 ")", value);
	ret.assign(buf);
	return ret;
}

template<>
inline std::string export_value(const int64_t& value) {
	std::string ret;
	char buf[256];
	std::memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "INT64_C(0x%016" PRIX64 ")", value);
	ret.assign(buf);
	return ret;
}

template<>
inline std::string export_value(const float& value) {
	std::string ret;
	char buf[256];
	std::memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "UINT32_C(0x%08" PRIX32 ")", std::bit_cast<uint32_t>(value));
	ret.assign(buf);
	return ret;
}

template<>
inline std::string export_value(const uint32_t& value) {
	std::string ret;
	char buf[256];
	std::memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "UINT32_C(0x%08" PRIX32 ")", value);
	ret.assign(buf);
	return ret;
}

template<>
inline std::string export_value(const int32_t& value) {
	std::string ret;
	char buf[256];
	std::memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "INT32_C(0x%08" PRIX32 ")", value);
	ret.assign(buf);
	return ret;
}


#endif /* EXPORT_VALUE_H */
