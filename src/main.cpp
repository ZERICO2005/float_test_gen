/*
**	Author: zerico2005 (2025)
**	Project: 
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <limits>
#include <string>
#include <vector>

#include "edge_cases.h"
#include "test_gen.hpp"

#include "random_gen.h"

#include "export_value.h"

template<typename T>
struct float_name {
	static const char* fX;
	static const char* fpX;
	static const char* abi_type;
	static const char* int_type;
	static const char* int_literal;
};

template<> const char* float_name<float>::fX = "f32";
template<> const char* float_name<float>::fpX = "fp32";
template<> const char* float_name<float>::abi_type = "float";
template<> const char* float_name<float>::int_type = "uint32_t";
template<> const char* float_name<float>::int_literal = "UINT32_C";

template<> const char* float_name<double>::fX = "f64";
template<> const char* float_name<double>::fpX = "fp64";
template<> const char* float_name<double>::abi_type = "long double";
template<> const char* float_name<double>::int_type = "uint64_t";
template<> const char* float_name<double>::int_literal = "UINT64_C";

template <typename T>
inline void generate_ilogb_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::copy(edge_cases<T>.begin(), edge_cases<T>.end(), values.begin());
	random_gen_basic(values, edge_cases<T>.size());
	for (size_t i = 0; i < input.size(); i++) {
		input[i] = export_value(values[i]);
		int fp_class = std::fpclassify(values[i]);
		int result = std::ilogb(values[i]);
		switch(fp_class) {
			case FP_INFINITE: output[i] = "INT_MAX"; break;
			case FP_NAN: output[i] = "FP_ILOGBNAN"; break;
			case FP_ZERO: output[i] = "FP_ILOGB0"; break;
			default: output[i] = std::to_string(result);
		}
	}
}

template <typename T>
inline void generate_logb_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::copy(edge_cases<T>.begin(), edge_cases<T>.end(), values.begin());
	random_gen_basic(values, edge_cases<T>.size());
	for (size_t i = 0; i < input.size(); i++) {
		input[i] = export_value(values[i]);
		T result = std::logb(values[i]);
		output[i] = export_value(result);
	}
}

template <typename T>
inline void generate_frexp_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::copy(edge_cases<T>.begin(), edge_cases<T>.end(), values.begin());
	random_gen_basic(values, edge_cases<T>.size());
	for (size_t i = 0; i < input.size(); i++) {
		input[i] = export_value(values[i]);
		int fp_class = std::fpclassify(values[i]);
		int expon;
		T result = std::frexp(values[i], &expon);
		output[i] = "{";
		output[i] += export_value(result);
		output[i] += ", ";
		switch(fp_class) {
			case FP_INFINITE: output[i] += "INT_MAX"; break;
			case FP_NAN: output[i] += "FP_ILOGBNAN"; break;
			case FP_ZERO: output[i] += "FP_ILOGB0"; break;
			default: output[i] += std::to_string(expon);
		}
		output[i] += "}";
	}
}

template <typename T>
inline void generate_ldexp_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::vector<int> expon(input.size());

	std::random_device rd;
	std::mt19937 gen(rd());
	
	int rand_expon_range
	= std::numeric_limits<T>::max_exponent
	- std::numeric_limits<T>::min_exponent
	+ std::numeric_limits<T>::digits;
	
	const std::vector<int> expon_edge_cases = {
		0, 1, -1, 2, -2,
		std::numeric_limits<T>::digits,
		std::numeric_limits<T>::max_exponent,
		rand_expon_range,
		rand_expon_range - 1,
		-std::numeric_limits<T>::digits,
		-std::numeric_limits<T>::max_exponent,
		-rand_expon_range,
		-rand_expon_range + 1,
	};
	
	if (input.size() < edge_cases<T>.size() * expon_edge_cases.size()) {
		printf(
			"Error: Input size (%zu) must be at least %zu\n",
			input.size(), edge_cases<T>.size() * expon_edge_cases.size()
		);
		return;
	}
	
	random_gen_basic(values, edge_cases<T>.size() * expon_edge_cases.size());
	std::uniform_int_distribution<int> dist(
		-rand_expon_range, rand_expon_range
	);
	{
		size_t offset = 0;
		for (size_t e = 0; e < expon_edge_cases.size(); e++) {
			for (size_t i = 0; i < edge_cases<T>.size(); i++) {
				values[offset] = edge_cases<T>[i];
				expon[offset] = expon_edge_cases[e];
				offset++;
			}
		}
		for (; offset < expon.size(); offset++) {
			expon[offset] = dist(gen);
			if (offset % 16 != 0) {
				expon[offset] /= std::numeric_limits<T>::max_exponent / 64;
			}
		}
	}

	for (size_t i = 0; i < input.size(); i++) {
		input[i] = "{";
		input[i] += export_value(values[i]);
		input[i] += ", ";
		input[i] += std::to_string(expon[i]);
		input[i] += "}";

		T result = std::ldexp(values[i], expon[i]);
		output[i] = export_value(result);
	}
}

template <typename T>
inline void generate_nextafter_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::vector<T> target(input.size());
	
	const std::vector<T> target_edge_cases = {
		static_cast<T>(0.0),
		std::numeric_limits<T>::denorm_min(),
		static_cast<T>(1.0),
		std::numeric_limits<T>::max(),
		std::numeric_limits<T>::infinity(),
		std::numeric_limits<T>::quiet_NaN(),
		std::numeric_limits<T>::signaling_NaN(),
		-static_cast<T>(0.0),
		-std::numeric_limits<T>::denorm_min(),
		-static_cast<T>(1.0),
		-std::numeric_limits<T>::max(),
		-std::numeric_limits<T>::infinity(),
		-std::numeric_limits<T>::quiet_NaN(),
		-std::numeric_limits<T>::signaling_NaN(),
	};
	
	if (input.size() < edge_cases<T>.size() * target_edge_cases.size()) {
		printf(
			"Error: Input size (%zu) must be at least %zu\n",
			input.size(), edge_cases<T>.size() * target_edge_cases.size()
		);
		return;
	}
	
	random_gen_basic(values, edge_cases<T>.size() * target_edge_cases.size());
	random_gen_basic(target, edge_cases<T>.size() * target_edge_cases.size());

	{
		size_t offset = 0;
		for (size_t t = 0; t < target_edge_cases.size(); t++) {
			for (size_t i = 0; i < edge_cases<T>.size(); i++) {
				values[offset] = edge_cases<T>[i];
				target[offset] = target_edge_cases[t];
				offset++;
			}
		}
	}

	for (size_t i = 0; i < input.size(); i++) {
		input[i] = "{";
		input[i] += export_value(values[i]);
		input[i] += ", ";
		input[i] += export_value(target[i]);
		input[i] += "}";

		T result = std::nextafter(values[i], target[i]);
		output[i] = export_value(result);
	}
}

template <typename T>
inline void generate_fma_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());

	const std::vector<T> fma_edge_cases = {
		static_cast<T>(0.0),
		static_cast<T>(1.0),
		std::numeric_limits<T>::infinity(),
		std::numeric_limits<T>::quiet_NaN(),
		-static_cast<T>(0.0),
		-static_cast<T>(1.0),
		-std::numeric_limits<T>::infinity(),
		-std::numeric_limits<T>::quiet_NaN(),
	};
	
	size_t offset = fma_edge_cases.size() * fma_edge_cases.size() * fma_edge_cases.size();

	if (input.size() < offset) {
		printf(
			"Error: Input size (%zu) must be at least %zu\n",
			input.size(), offset
		);
		return;
	}

	std::vector<T> x(input.size());
	std::vector<T> y(input.size());
	std::vector<T> z(input.size());

	{
		size_t i = 0;
		for (size_t i0 = 0; i0 < fma_edge_cases.size(); i0++) {
			for (size_t i1 = 0; i1 < fma_edge_cases.size(); i1++) {
				for (size_t i2 = 0; i2 < fma_edge_cases.size(); i2++) {
					x[i] = fma_edge_cases[i0];
					y[i] = fma_edge_cases[i1];
					z[i] = fma_edge_cases[i2];
					i++;
				}
			}
		}
	}

	random_gen_basic(x, offset);
	random_gen_basic(y, offset);
	random_gen_basic(z, offset);

	for (size_t i = 0; i < input.size(); i++) {
		input[i] = "{";
		input[i] += export_value(x[i]);
		input[i] += ", ";
		input[i] += export_value(y[i]);
		input[i] += ", ";
		input[i] += export_value(z[i]);
		input[i] += "}";

		T result = std::fma(x[i], y[i], z[i]);
		output[i] = export_value(result);
	}
}

template <typename T>
inline void generate_sqrt_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::copy(edge_cases<T>.begin(), edge_cases<T>.end(), values.begin());
	random_gen_basic(values, edge_cases<T>.size());
	for (size_t i = 0; i < input.size(); i++) {
		input[i] = export_value(values[i]);
		T result = std::sqrt(values[i]);
		output[i] = export_value(result);
	}
}

template <typename T>
inline void generate_modf_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::copy(edge_cases<T>.begin(), edge_cases<T>.end(), values.begin());
	random_gen_basic(values, edge_cases<T>.size());
	for (size_t i = 0; i < input.size(); i++) {
		input[i] = export_value(values[i]);
		T integral_part;
		T result = std::modf(values[i], &integral_part);
		output[i] = "{";
		output[i] += export_value(result);
		output[i] += ", ";
		output[i] += export_value(integral_part);
		output[i] += "}";
	}
}

template <typename T>
inline void generate_rounding_test(
	std::vector<std::string>& input,
	std::vector<std::string>& output
) {
	assert(input.size() == output.size());
	std::vector<T> values(input.size());
	std::copy(edge_cases<T>.begin(), edge_cases<T>.end(), values.begin());
	random_gen_basic(values, edge_cases<T>.size());
	for (size_t i = 0; i < input.size(); i++) {
		input[i] = export_value(values[i]);
		output[i] = "{";
		output[i] += export_value(std::floor(values[i]));
		output[i] += ", ";
		output[i] += export_value(std::ceil(values[i]));
		output[i] += ", ";
		output[i] += export_value(std::round(values[i]));
		output[i] += "}";
	}
}

inline std::string get_ISO8601Timestamp() {
	time_t now;
	time(&now);
	char buf[sizeof("2000-01-01T00:00:00Z")];
	strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
	std::string ret = buf;
	return ret;
}

template<typename T>
void export_table(
	const Test_Gen<T>& table,
	const std::vector<std::string>& input,
	const std::vector<std::string>& output
) {
	std::string file_name = float_name<T>::fX;
	file_name += "_";
	file_name += table.table_name;
	file_name += ".h";
	std::string include_guard = file_name;
	std::transform(include_guard.begin(), include_guard.end(), include_guard.begin(), ::toupper);
	std::replace(include_guard.begin(), include_guard.end(), '.', '_');

    FILE* file = fopen(file_name.c_str(), "wb");
	if (file == nullptr) {
		printf("Unable to open file \"%s\"\n", file_name.c_str());
		return;
	}
	
	/* begin */

	fprintf(file, "#ifndef %s\n", include_guard.c_str());
	fprintf(file, "#define %s\n\n", include_guard.c_str());
	fprintf(file, "%s\n\n", table.headers.c_str());
	
	fprintf(file, "/* Generated %s */\n\n", get_ISO8601Timestamp().c_str());

	fprintf(file, "typedef %s input_type;\n\n", table.input_type.c_str());
	fprintf(file, "typedef %s output_type;\n\n", table.output_type.c_str());

	/* input values */

	fprintf(file,
		"const input_type %s_%s_input[%zu] = {\n",
		float_name<T>::fX, table.table_name.c_str(),
		input.size()
	);
	for (size_t i = 0; i < input.size(); i++) {
		fprintf(file, "/* %4zu */ %s,\n", i, export_value(input[i]).c_str());
	}
	fprintf(file, "};\n\n");

	/* output values */

	fprintf(file,
		"const output_type %s_%s_output[%zu] = {\n",
		float_name<T>::fX, table.table_name.c_str(),
		output.size()
	);
	for (size_t i = 0; i < output.size(); i++) {
		fprintf(file, "/* %4zu */ %s,\n", i, export_value(output[i]).c_str());
	}
	fprintf(file, "};\n\n");

	/* end */

	fprintf(file, "#endif /* %s */\n", include_guard.c_str());

	fclose(file);
	printf("Wrote file \"%s\"\n", file_name.c_str());
}

template<typename T>
void generate_all_tests(void) {
	const char* float_int_type = float_name<T>::int_type;
	std::vector<Test_Gen<T>> Test_List;

	{
		Test_List.push_back(Test_Gen<T>(
			generate_ilogb_test<T>,
			"ilogb_LUT",
			float_int_type,
			"int",
			"#include <stdint.h>\n#include <limits.h>\n#include <math.h>",
			sizeof(T) + 3
		));
	}
	{
		Test_List.push_back(Test_Gen<T>(
			generate_logb_test<T>,
			"logb_LUT",
			float_int_type,
			float_int_type,
			"#include <stdint.h>",
			2 * sizeof(T)
		));
	}
	{
		std::string frexp_output_type = "struct { ";
		frexp_output_type += float_int_type;
		frexp_output_type += " frac; int expon; }";
		Test_List.push_back(Test_Gen<T>(
			generate_frexp_test<T>,
			"frexp_LUT",
			float_int_type,
			frexp_output_type.c_str(),
			"#include <stdint.h>\n#include <limits.h>\n#include <math.h>",
			2 * sizeof(T) + 3
		));
	}
	{
		std::string ldexp_input_type = "struct { ";
		ldexp_input_type += float_int_type;
		ldexp_input_type += " value; int expon; }";
		Test_List.push_back(Test_Gen<T>(
			generate_ldexp_test<T>,
			"ldexp_LUT",
			ldexp_input_type.c_str(),
			float_int_type,
			"#include <stdint.h>",
			2 * sizeof(T) + 3
		));
	}
	{
		std::string nextafter_input_type = "struct { ";
		nextafter_input_type += float_int_type;
		nextafter_input_type += " value; ";
		nextafter_input_type += float_int_type;
		nextafter_input_type += " target; }";
		Test_List.push_back(Test_Gen<T>(
			generate_nextafter_test<T>,
			"nextafter_LUT",
			nextafter_input_type.c_str(),
			float_int_type,
			"#include <stdint.h>",
			3 * sizeof(T)
		));
	}
	{
		Test_List.push_back(Test_Gen<T>(
			generate_sqrt_test<T>,
			"sqrt_LUT",
			float_int_type,
			float_int_type,
			"#include <stdint.h>",
			2 * sizeof(T)
		));
	}
	{
		std::string fma_input_type = "struct { ";
		fma_input_type += float_int_type;
		fma_input_type += " x; ";
		fma_input_type += float_int_type;
		fma_input_type += " y; ";
		fma_input_type += float_int_type;
		fma_input_type += " z; }";
		Test_List.push_back(Test_Gen<T>(
			generate_fma_test<T>,
			"fma_LUT",
			fma_input_type.c_str(),
			float_int_type,
			"#include <stdint.h>",
			4 * sizeof(T)
		));
	}
	{
		std::string modf_output_type = "struct { ";
		modf_output_type += float_int_type;
		modf_output_type += " frac_part; ";
		modf_output_type += float_int_type;
		modf_output_type += " trunc_part; }";
		Test_List.push_back(Test_Gen<T>(
			generate_modf_test<T>,
			"modf_LUT",
			float_int_type,
			modf_output_type.c_str(),
			"#include <stdint.h>",
			3 * sizeof(T)
		));
	}
	{
		std::string rounding_output_type = "struct { ";
		rounding_output_type += float_int_type;
		rounding_output_type += " r_floor; ";
		rounding_output_type += float_int_type;
		rounding_output_type += " r_ceil; ";
		rounding_output_type += float_int_type;
		rounding_output_type += " r_round; }";
		Test_List.push_back(Test_Gen<T>(
			generate_rounding_test<T>,
			"rounding_LUT",
			float_int_type,
			rounding_output_type.c_str(),
			"#include <stdint.h>",
			4 * sizeof(T)
		));
	}

	for (size_t i = 0; i < Test_List.size(); i++) {
		size_t elem_count = 32768 / Test_List[i].element_size;
		elem_count = std::min<size_t>(elem_count, 1024);
		std::vector<std::string> input(elem_count);
		std::vector<std::string> output(elem_count);
		Test_List[i].generate(input, output);
		export_table(Test_List[i], input, output);
	}
}

int main(void) {
	generate_all_tests<float>();
	generate_all_tests<double>();
	return 0;
}
