#ifndef TEST_GEN_HPP
#define TEST_GEN_HPP

#include <vector>
#include <string>
#include <functional>
#include <cstddef>

template<typename T>
struct Test_Gen {
	std::function<
		void (std::vector<std::string>&, std::vector<std::string>&)
	> generate;

	std::string table_name;
	std::string input_type;
	std::string output_type;
	std::string headers;
	size_t element_size;

	Test_Gen(
		std::function<
			void (std::vector<std::string>&, std::vector<std::string>&)
		> generate_function,
		const char* table_name,
		const char* input_type,
		const char* output_type,
		const char* headers,
		size_t element_size
	) :
		generate(generate_function),
		table_name(table_name),
		input_type(input_type),
		output_type(output_type),
		headers(headers),
		element_size(element_size)
	{}
};

#endif /* TEST_GEN_HPP */
