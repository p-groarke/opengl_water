#pragma once

#include <glad/glad.h>
#include <cstdio>
#include <cstring>
#include <string>

#define DEBUG_MODE

#ifndef TS_MAXIMUM_ENTITY
#define TS_MAXIMUM_ENTITY 10000
#endif

#ifndef TS_MAXIMUM_COMPONENT
#define TS_MAXIMUM_COMPONENT 10000
#endif

#define OUTPUT_ERROR(format, ...)                                                                            \
	printf("%s(%d) : %s() : " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define OUTPUT_MSG(format, ...) printf("" format "\n", ##__VA_ARGS__)

#ifdef DEBUG_MODE
#define assert_error(cond, format, ...)                                                                      \
	(!(cond)) ? printf("%s(%d) : %s() : " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__) : 0
#else
#define assert_error(...)
#endif // DEBUG_MODE

namespace app {

class ExecutableDirectory {
public:
	static std::string path;

private:
	ExecutableDirectory();
	static ExecutableDirectory instance;
};
} // namespace app
