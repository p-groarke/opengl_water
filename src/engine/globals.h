#pragma once
#include <glad/glad.h>
#include <cstdio>
#include <cstring>
#include <string>

#define OUTPUT_ERROR(format, ...) \
	printf("%s(%d) : %s() : " format "\n" \
	, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define OUTPUT_MSG(format, ...) \
	printf("" format "\n", ##__VA_ARGS__)

namespace app {
extern std::string executable_dir;
} // namespace app
