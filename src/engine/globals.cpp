#include "engine/globals.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#else
#error Unknown platform
#endif

namespace app {
std::string ExecutableDirectory::path;
ExecutableDirectory ExecutableDirectory::instance;

ExecutableDirectory::ExecutableDirectory()
{
	char app_path[4096];
	uint32_t size = 4096;

	if (_NSGetExecutablePath(app_path, &size) == 0) {
		path = std::string(app_path);
		std::string::size_type slash_index = path.find_last_of('/');

		if (slash_index != std::string::npos) {
			path = path.substr(0, slash_index + 1);
		}
	} else {
		OUTPUT_ERROR("buffer too small; need size %u\n", size);
	}
}
} // namespace app
