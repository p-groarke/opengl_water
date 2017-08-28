#pragma once
#include <cstdio>
#include <cstring>
#include <string>

#define OUTPUT_ERROR(format, ...) \
	printf("%s(%d) : %s() :\n  " format "\n" \
	, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define OUTPUT_MSG(format, ...) \
	printf("\n" format "\n", ##__VA_ARGS__)

namespace app {
extern std::string path;

void init_executable_path();
} // namespace app
