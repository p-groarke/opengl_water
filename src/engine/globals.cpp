#include "engine/globals.h"

#include <cassert>

#if defined(__APPLE__)
	#include <mach-o/dyld.h>
#elif defined(_WIN32)
	#include <windows.h>
#endif

namespace app {

std::string path = "";

void init_executable_path() {
	char platform_slash = '/';
#if defined(__APPLE__)
	uint32_t size = 512;
	char app_path[size];
	int err = _NSGetExecutablePath(app_path, &size);
	assert(err == 0 && "Problem getting application path.");
#elif defined(_WIN32)
	TCHAR app_path[MAX_PATH];
	int err = GetModuleFileName(NULL, app_path, MAX_PATH);
	platform_slash = '\\';
	//assert(err == 0 && "Problem getting application path."); // FIXME
#endif
	path = std::string(app_path);
	auto slash_index = path.find_last_of(platform_slash);
	if (slash_index != std::string::npos) {
		path = path.substr(0, slash_index + 1);
	}
}

} //namespace app
