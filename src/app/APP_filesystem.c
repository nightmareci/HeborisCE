#include "APP_filesystem.h"

static char* APP_BasePath = NULL;
static char* APP_PrefPath = NULL;

bool APP_InitFilesystem(int argc, char** argv) {
	if ( argc > 1 && SDL_strlen(argv[1]) > 0 )
	{
		APP_BasePath = SDL_strdup(argv[1]);
		if ( !APP_BasePath )
		{
			fprintf(stderr, "Error duplicating base path string\n");
			return false;
		}
		APP_PrefPath = SDL_strdup(argv[1]);
		if ( !APP_PrefPath )
		{
			fprintf(stderr, "Error duplicating pref path string\n");
			return false;
		}
	}

#if APP_FILESYSTEM_TYPE == APP_FILESYSTEM_WORKINGDIR
	else
	{
		APP_BasePath = SDL_GetCurrentDirectory();
		if ( !APP_BasePath )
		{
			fprintf(stderr, "Error getting current directory: %s\n", SDL_GetError());
			return false;
		}
		APP_PrefPath = SDL_strdup(APP_BasePath);
		if ( !APP_PrefPath )
		{
			fprintf(stderr, "Error duplicating current directory string\n");
			return false;
		}
	}

#elif APP_FILESYSTEM_TYPE == APP_FILESYSTEM_PORTABLE
	else
	{
		const char* basePath = SDL_GetBasePath();
		if ( !basePath )
		{
			fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
			return false;
		}
		APP_BasePath = SDL_strdup(basePath);
		if ( !APP_BasePath )
		{
			fprintf(stderr, "Error duplicating base path string\n");
			return false;
		}
		APP_PrefPath = SDL_strdup(APP_BasePath);
		if ( !APP_PrefPath )
		{
			fprintf(stderr, "Error duplicating base path string\n");
			return false;
		}
	}

#elif APP_FILESYSTEM_TYPE == APP_FILESYSTEM_INSTALLABLE
	else
	{
		const char* basePath = SDL_GetBasePath();
		if ( !basePath )
		{
			fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
			return false;
		}
		if (SDL_asprintf(&APP_BasePath, "%s%s", basePath, APP_BASE_PATH_APPEND) < 0) {
			fprintf(stderr, "Failed creating full base path\n");
			return false;
		}
		APP_PrefPath = SDL_GetPrefPath(APP_FILESYSTEM_ORG, APP_FILESYSTEM_APP);
		if ( !APP_PrefPath )
		{
			fprintf(stderr, "Failed getting pref path: %s\n", SDL_GetError());
			return false;
		}
	}

#else
	#error APP_FILESYSTEM_TYPE must be defined as APP_FILESYSTEM_WORKINGDIR, APP_FILESYSTEM_PORTABLE, or APP_FILESYSTEM_INSTALLABLE.

#endif

	return true;
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

bool APP_CreateDirectory(const char* const directory) {
	char* prefPathDirectory;
	if (SDL_asprintf(&prefPathDirectory, "%s%s", APP_PrefPath, directory) < 0) {
		fprintf(stderr, "Failed creating directory string\n");
		return false;
	}

	#ifdef __EMSCRIPTEN__
	// TODO: This might not be needed, SDL_CreateDirectory() might work fine with Emscripten
	// NOTE: It seems this only really works with one FS.mkdir call in the try
	// block; putting more than one seems to always cause a fatal error.
	EM_ASM({
		var prefPathDirectory = UTF8ToString($0);

		try {
			FS.mkdir(prefPathDirectory);
		}
		catch (err) {
			assert(!err || err.errno == 20 || err.errno == 10);
		}
		FS.mount(IDBFS, {}, prefPathDirectory);
	}, prefPathDirectory);

	SDL_free(prefPathDirectory);
	return true;

	#else
	if (!SDL_CreateDirectory(prefPathDirectory)) {
		fprintf(stderr, "Failed creating directory \"%s\"\n", directory);
		SDL_free(prefPathDirectory);
		return false;
	}
	else {
		SDL_free(prefPathDirectory);
		return true;
	}

	#endif
}

SDL_IOStream* APP_OpenInMode(const char* const filename, const char* const mode) {
	char* path;
	SDL_PathInfo info;
	SDL_IOStream* file;

	if (SDL_asprintf(&path, "%s%s", APP_PrefPath, filename) < 0) {
		return NULL;
	}
	if (!SDL_GetPathInfo(path, &info)) {
		SDL_free(path);
		return NULL;
	}
	if (info.type == SDL_PATHTYPE_FILE) {
		file = SDL_IOFromFile(path, mode);
		if (file) {
			SDL_free(path);
			return file;
		}
		else {
			SDL_free(path);
			return NULL;
		}
	}
	SDL_free(path);

	if (SDL_asprintf(&path, "%s%s", APP_BasePath, filename) < 0) {
		return NULL;
	}
	if (!SDL_GetPathInfo(path, &info)) {
		SDL_free(path);
		return NULL;
	}
	if (info.type != SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return NULL;
	}
	file = SDL_IOFromFile(path, mode);
	if (!file) {
		SDL_free(path);
		return NULL;
	}
	else {
		return file;
	}
}

SDL_IOStream* APP_OpenRead(const char* const filename) {
	return APP_OpenInMode(filename, "r");
}

SDL_IOStream* APP_OpenWrite(const char* const filename) {
	return APP_OpenInMode(filename, "w");
}

SDL_IOStream* APP_OpenAppend(const char* const filename) {
	return APP_OpenInMode(filename, "a");
}

bool APP_GetPathInfo(const char* const filename, SDL_PathInfo* info)
{
	char* path;

	if (SDL_asprintf(&path, "%s%s", APP_PrefPath, filename) < 0) {
		return false;
	}
	if (!SDL_GetPathInfo(path, info)) {
		SDL_free(path);
		return false;
	}
	if (info->type != SDL_PATHTYPE_NONE) {
		return true;
	}
	SDL_free(path);

	if (SDL_asprintf(&path, "%s%s", APP_BasePath, filename) < 0) {
		return false;
	}
	if (!SDL_GetPathInfo(path, info)) {
		SDL_free(path);
		return false;
	}
	SDL_free(path);
	return true;
}

void* APP_GetFileBuffer(const char* const filename, size_t* size) {
	SDL_IOStream* file = APP_OpenInMode(filename, "rb");
	if (!file) {
		return NULL;
	}
	void* data = SDL_LoadFile(filename, size);
	SDL_CloseIO(file);
	return data;
}
