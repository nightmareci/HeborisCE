#include "filesystem.h"
#ifndef __EMSCRIPTEN__
#include "physfsrwops.h"
#endif

static char* alloc_strcat_multi(const char* const s, ...) {
	if (!s) {
		return NULL;
	}
	else {
		va_list args;
		va_start(args, s);

		struct str_node;
		typedef struct str_node {
			const char* s;
			size_t length;
			struct str_node* next;
		} str_node;

		str_node* head;
		if ((head = malloc(sizeof(str_node))) == NULL) {
			abort();
		}

		*head = (str_node) {
			.s = s,
			.length = strlen(s),
			.next = NULL
		};

		size_t cat_length = head->length;
		
		str_node* cur = head;
		size_t num_nodes = 1u;
		while (true) {
			const char* const next_s = va_arg(args, const char*);
			if (next_s == NULL) {
				break;
			}

			if ((cur->next = malloc(sizeof(str_node))) == NULL) {
				abort();
			}
			cur->next->s = next_s;
			cur->next->length = strlen(next_s);
			cur->next->next = NULL;

			cat_length += cur->next->length;
			num_nodes++;

			cur = cur->next;
		}

		va_end(args);

		char* cat;
		if ((cat = malloc(cat_length + 1u)) == NULL) {
			abort();
		}
		char* cur_cat = cat;
		cur = head;
		for (size_t i = 0u; i < num_nodes; i++) {
			memcpy(cur_cat, cur->s, cur->length);
			cur_cat += cur->length;
			str_node* next = cur->next;
			free(cur);
			cur = next;
		}
		cat[cat_length] = '\0';
		return cat;
	}
}

#ifdef __EMSCRIPTEN__
static char* FSPrefPath = NULL;
#endif

bool FSInit(int argc, char** argv) {
#ifdef __EMSCRIPTEN__
	char* prefPath = SDL_GetPrefPath(FILESYSTEM_ORG, FILESYSTEM_APP);
	if ( !prefPath )
	{
		fprintf(stderr, "Error getting pref path\n");
	}
	FSPrefPath = prefPath;

	return true;

#else
	if ( !PHYSFS_init(argv[0]) )
	{
		fprintf(stderr, "Couldn't initialize file access: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	if ( argc > 1 && strlen(argv[1]) > 0 )
	{
		char *specifiedPath = argv[1];
		if ( !PHYSFS_mount(specifiedPath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting specified path \"%s\": %s\n", specifiedPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return false;
		}
		if ( !PHYSFS_setWriteDir(specifiedPath) )
		{
			fprintf(stderr, "Error setting specified path \"%s\" for writing: %s\n", specifiedPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return false;
		}
	}

#if FILESYSTEM_TYPE == FILESYSTEM_WORKINGDIR
	else
	{
		if ( !PHYSFS_mount("./", NULL, 0) )
		{
			fprintf(stderr, "Error mounting working directory: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return false;
		}
		if ( !PHYSFS_setWriteDir("./") )
		{
			fprintf(stderr, "Error setting working directory for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return false;
		}
	}

#elif FILESYSTEM_TYPE == FILESYSTEM_PORTABLE
	else
	{
		char *basePath = SDL_GetBasePath();
		if ( !basePath )
		{
			fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
			return false;
		}
		if ( !PHYSFS_mount(basePath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting base path \"%s\": %s\n", basePath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			free(basePath);
			return false;
		}
		if ( !PHYSFS_setWriteDir(basePath) )
		{
			fprintf(stderr, "Error setting working directory for writing: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			SDL_free(basePath);
			return false;
		}
		SDL_free(basePath);
	}

#elif FILESYSTEM_TYPE == FILESYSTEM_INSTALLABLE
	else
	{
		char *basePath = SDL_GetBasePath();
		if ( !basePath )
		{
			fprintf(stderr, "Failed getting base path: %s\n", SDL_GetError());
			return false;
		}
		char* fullBasePath;
		if ( !(fullBasePath = malloc(strlen(basePath) + strlen(BASE_PATH_APPEND) + 1)) )
		{
			fprintf(stderr, "Failed creating full base path.\n");
			SDL_free(basePath);
			return false;
		}
		sprintf(fullBasePath, "%s%s", basePath, BASE_PATH_APPEND);
		SDL_free(basePath);
		if ( !PHYSFS_mount(fullBasePath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting full base path \"%s\": %s\n", fullBasePath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			free(fullBasePath);
			return false;
		}
		free(fullBasePath);

		char *prefPath = SDL_GetPrefPath(FILESYSTEM_ORG, FILESYSTEM_APP);
		if ( !prefPath )
		{
			fprintf(stderr, "Failed getting pref path: %s\n", SDL_GetError());
			return false;
		}
		if ( !PHYSFS_setWriteDir(prefPath) )
		{
			fprintf(stderr, "Error setting pref path \"%s\" for writing: %s\n", prefPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			SDL_free(prefPath);
			return false;
		}
		if ( !PHYSFS_mount(prefPath, NULL, 0) )
		{
			fprintf(stderr, "Error mounting pref path \"%s\": %s\n", prefPath, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			SDL_free(prefPath);
			return false;
		}
		SDL_free(prefPath);
	}

#elif FILESYSTEM_TYPE == FILESYSTEM_PHYSFS
	else if ( !PHYSFS_setSaneConfig(FILESYSTEM_ORG, FILESYSTEM_APP, "ZIP", 0, 0) )
	{
		fprintf(stderr, "Error setting sane PhysicsFS config: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

#else
	#error FILESYSTEM_TYPE must be defined as FILESYSTEM_WORKINGDIR, FILESYSTEM_PORTABLE, FILESYSTEM_INSTALLABLE, or FILESYSTEM_PHYSFS.

#endif

	return true;

#endif
}

void FSDeInit() {
#ifdef __EMSCRIPTEN__
	EM_ASM({
		FS.syncfs(function (err) {
			assert(!err);
		});
	});
	if (FSPrefPath) {
		SDL_free(FSPrefPath);
		FSPrefPath = NULL;
	}

#else
	if (!PHYSFS_deinit()) {
		fprintf(stderr, "Failed closing access to files: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

#endif
}

bool FSMkdir(const char* const directory) {
	#ifdef __EMSCRIPTEN__
	char* const prefPathDirectory = alloc_strcat_multi(FSPrefPath, directory, NULL);

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

	free(prefPathDirectory);
	return true;

	#else
	if (!PHYSFS_mkdir(directory)) {
		fprintf(stderr, "Failed creating directory \"%s\"\n", directory);
		return false;
	}
	else {
		return true;
	}

	#endif
}

#ifdef __EMSCRIPTEN__
SDL_RWops* FSOpenInMode(const char* const filename, const char mode) {
	const char* mode_str = NULL;
	bool canWrite = false;

	switch (mode) {
	case 'r':
		mode_str = "rb";
		canWrite = false;
		break;
	case 'w':
		mode_str = "wb";
		canWrite = true;
		break;
	case 'a':
		mode_str = "ab";
		canWrite = true;
		break;
	default:
		return NULL;
	}

	{
		char* const prefPathFilename = alloc_strcat_multi(FSPrefPath, filename, NULL);
		SDL_RWops* const file = SDL_RWFromFile(prefPathFilename, mode_str);
		free(prefPathFilename);
		if (file) {
			return file;
		}
		else if (canWrite) {
			return NULL;
		}
	}

	{
		char* const basePathFilename = alloc_strcat_multi("basepath/", filename, NULL);
		SDL_RWops* const file = SDL_RWFromFile(basePathFilename, mode_str);
		free(basePathFilename);
		return file;
	}
}

#else
SDL_RWops* FSOpenInMode(const char* const filename, const char mode) {
	switch (mode) {
	case 'r':
		return PHYSFSRWOPS_openRead(filename);
	case 'w':
		return PHYSFSRWOPS_openWrite(filename);
	case 'a':
		return PHYSFSRWOPS_openAppend(filename);
	default:
		return NULL;
	}
}

#endif

SDL_RWops* FSOpenRead(const char* const filename) {
	return FSOpenInMode(filename, 'r');
}

SDL_RWops* FSOpenWrite(const char* const filename) {
	return FSOpenInMode(filename, 'w');
}

SDL_RWops* FSOpenAppend(const char* const filename) {
	return FSOpenInMode(filename, 'a');
}
