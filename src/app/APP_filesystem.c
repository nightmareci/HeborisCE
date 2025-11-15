#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_error.h"

static char* APP_BasePath = NULL;
static char* APP_PrefPath = NULL;

bool APP_InitFilesystem(int argc, char** argv) {
	if (argc > 1 && SDL_strlen(argv[1]) > 0) {
		APP_BasePath = SDL_strdup(argv[1]);
		if (!APP_BasePath) {
			return APP_SetError("Error duplicating base path string");
		}
		APP_PrefPath = SDL_strdup(argv[1]);
		if (!APP_PrefPath) {
			return APP_SetError("Error duplicating pref path string");
		}
	}
#if APP_FILESYSTEM_TYPE == APP_FILESYSTEM_CURRENT_DIRECTORY
	else {
		APP_BasePath = SDL_GetCurrentDirectory();
		if (!APP_BasePath) {
			return APP_SetError("Error getting current directory: %s", SDL_GetError());
		}
		APP_PrefPath = SDL_strdup(APP_BasePath);
		if (!APP_PrefPath) {
			return APP_SetError("Error duplicating current directory string");
		}
	}
#elif APP_FILESYSTEM_TYPE == APP_FILESYSTEM_PORTABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return APP_SetError("Failed getting base path: %s", SDL_GetError());
		}
		APP_BasePath = SDL_strdup(basePath);
		if (!APP_BasePath) {
			return APP_SetError("Error duplicating base path string");
		}
		APP_PrefPath = SDL_strdup(APP_BasePath);
		if (!APP_PrefPath) {
			return APP_SetError("Error duplicating base path string");
		}
	}
#elif APP_FILESYSTEM_TYPE == APP_FILESYSTEM_INSTALLABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return APP_SetError("Failed getting base path: %s", SDL_GetError());
		}
		if (SDL_asprintf(&APP_BasePath, "%s%s", basePath, APP_BASE_PATH_APPEND) < 0) {
			return APP_SetError("Failed creating full base path");
		}
		APP_PrefPath = SDL_GetPrefPath(APP_FILESYSTEM_ORG, APP_FILESYSTEM_APP);
		if (!APP_PrefPath) {
			return APP_SetError("Failed getting pref path: %s", SDL_GetError());
		}
	}

#else
	#error APP_FILESYSTEM_TYPE must be defined as APP_FILESYSTEM_CURRENT_DIRECTORY, APP_FILESYSTEM_PORTABLE, or APP_FILESYSTEM_INSTALLABLE.

#endif

#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
#endif

	return true;
}

bool APP_CreateDirectory(const char* directory) {
	char* path;

	if (SDL_asprintf(&path, "%s%s", APP_PrefPath, directory) < 0) {
		return APP_SetError("Failed creating directory string");
	}
	else if (!SDL_CreateDirectory(path)) {
		SDL_free(path);
		return APP_SetError("Failed creating directory \"%s\"", directory);
	}
	SDL_free(path);

#ifdef __EMSCRIPTEN__
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
#endif

	return true;
}

bool APP_FileExists(const char* filename)
{
	char* path;
	SDL_PathInfo info;

	if (SDL_asprintf(&path, "%s%s", APP_PrefPath, filename) < 0) {
		APP_SetError("Error allocating string to check if file exists");
		APP_Exit(EXIT_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	if (SDL_asprintf(&path, "%s%s", APP_BasePath, filename) < 0) {
		APP_SetError("Error allocating string to check if file exists");
		APP_Exit(EXIT_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	return false;
}

static SDL_IOStream* APP_OpenFromPath(const char* path, const char* filename, const char* mode) {
	char* fullPath;
	if (SDL_asprintf(&fullPath, "%s%s", path, filename) < 0) {
		APP_SetError("Error allocating string to open a file");
		APP_Exit(EXIT_FAILURE);
	}

	SDL_IOStream* file = SDL_IOFromFile(fullPath, mode);
	SDL_free(fullPath);
	if (file) {
		return file;
	}
	else {
		return NULL;
	}
}

SDL_IOStream* APP_OpenRead(const char* filename) {
	SDL_IOStream* file = APP_OpenFromPath(APP_PrefPath, filename, "rb");
	if (file) {
		return file;
	}
	return APP_OpenFromPath(APP_BasePath, filename, "rb");
}

SDL_IOStream* APP_OpenWrite(const char* filename) {
	return APP_OpenFromPath(APP_PrefPath, filename, "wb");
}

SDL_IOStream* APP_OpenAppend(const char* filename) {
	return APP_OpenFromPath(APP_PrefPath, filename, "ab");
}

static void APP_Swap32ArrayNativeToLE(int32_t* arrayNative, size_t size)
{
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	(void)arrayNative;
	(void)size;
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
	for (size_t i = 0; i < size / sizeof(int32_t); i++) {
		arrayNative[i] = SDL_Swap32(arrayNative[i]);
	}
#else
	#error Unsupported endianness!
#endif
}

static void APP_Swap32ArrayLEToNative(int32_t* arrayLE, size_t size)
{
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	(void)arrayLE;
	(void)size;
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
	for (size_t i = 0; i < size / sizeof(int32_t); i++) {
		arrayLE[i] = SDL_Swap32(arrayLE[i]);
	}
#else
	#error Unsupported endianness!
#endif
}

void APP_LoadFile(const char* filename, void* buf, size_t size)
{
	SDL_IOStream* src = APP_OpenRead(filename);
	if (!src) {
		return;
	}

	if (SDL_ReadIO(src, buf, size) != size || !SDL_CloseIO(src)) {
		APP_SetError("Error reading file: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	APP_Swap32ArrayLEToNative(buf, size);
}

void APP_ReadFile(const char* filename, void* buf, size_t size, size_t offset)
{
	SDL_IOStream* src = APP_OpenRead(filename);
	if (!src) {
		return;
	}

	if (SDL_SeekIO(src, offset, SDL_IO_SEEK_SET) < 0) {
		return;
	}
	SDL_ReadIO(src, buf, size);
	SDL_CloseIO(src);

	APP_Swap32ArrayLEToNative(buf, size);
}


void APP_SaveFile(const char* filename, void* buf, size_t size)
{
	SDL_IOStream* dst = APP_OpenWrite(filename);
	if (!dst) {
		return;
	}

	APP_Swap32ArrayNativeToLE(buf, size);

	if (SDL_WriteIO(dst, buf, size) != size) {
		APP_SetError("Error writing file: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	if (!SDL_CloseIO(dst))
	{
		APP_SetError("Error closing file: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	APP_Swap32ArrayLEToNative(buf, size);
}

void APP_AppendFile(const char* filename, void* buf, size_t size)
{
	SDL_IOStream* dst = APP_OpenAppend(filename);
	if (!dst) {
		return;
	}

	APP_Swap32ArrayNativeToLE(buf, size);

	if (SDL_WriteIO(dst, buf, size) != size) {
		APP_SetError("Error writing file: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}
	if (!SDL_CloseIO(dst)) {
		APP_SetError("Error closing file: %s", SDL_GetError());
		APP_Exit(EXIT_FAILURE);
	}

	APP_Swap32ArrayLEToNative(buf, size);
}

void APP_QuitFilesystem(void) {
#ifdef __EMSCRIPTEN__
	EM_ASM({
		FS.syncfs(function (err) {
			assert(!err);
		});
	});
#endif

	SDL_free(APP_BasePath);
	APP_BasePath = NULL;
	SDL_free(APP_PrefPath);
	APP_PrefPath = NULL;
}
