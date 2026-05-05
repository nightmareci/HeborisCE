#include <FILESYSTEM.h>
#include <MAIN.h>
#include <ERROR.h>

static char* FILESYSTEM_PrefPath = NULL;

static SDL_IOStream* FILESYSTEM_OpenFromPath(const char* path, const char* filename, const char* mode)
{
	char* fullPath;
	if (SDL_asprintf(&fullPath, "%s%s", path, filename) < 0) {
		ERROR_Set("Error allocating string to open a file");
		MAIN_Exit(SDL_APP_FAILURE);
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

#ifdef FILESYSTEM_USING_RESOURCE_DAT

typedef struct FILESYSTEM_File
{
	int64_t start;
	int64_t size;
} FILESYSTEM_File;

typedef struct FILESYSTEM_FileUserdata
{
	FILESYSTEM_File* file;
	SDL_IOStream* stream;
} FILESYSTEM_FileUserdata;

static char* FILESYSTEM_FilesDATPath = NULL;
static FILESYSTEM_File* FILESYSTEM_FilesArray = NULL;
static SDL_PropertiesID FILESYSTEM_FilesTable = 0;

bool FILESYSTEM_Init(int argc, char** argv)
{
	if (argc > 1 && SDL_strlen(argv[1]) > 0) {
		if (SDL_asprintf(&FILESYSTEM_FilesDATPath, "%s%s", argv[1], PROJECT_NAME ".dat") < 0) {
			FILESYSTEM_FilesDATPath = NULL;
			return ERROR_Set("Error creating resource DAT path string");
		}
		FILESYSTEM_PrefPath = SDL_strdup(argv[1]);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error duplicating pref path string");
		}
	}
#if FILESYSTEM_TYPE == FILESYSTEM_CURRENT_DIRECTORY
	else {
		FILESYSTEM_PrefPath = SDL_GetCurrentDirectory();
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error getting current directory: %s", SDL_GetError());
		}
		if (SDL_asprintf(&FILESYSTEM_FilesDATPath, "%s%s", FILESYSTEM_PrefPath, PROJECT_NAME ".dat") < 0) {
			FILESYSTEM_FilesDATPath = NULL;
			return ERROR_Set("Error creating resource DAT path string");
		}
		SDL_PathInfo info;
		if (!SDL_GetPathInfo(FILESYSTEM_FilesDATPath, &info) || info.type != SDL_PATHTYPE_FILE) {
			SDL_free(FILESYSTEM_FilesDATPath);
			const char* basePath = SDL_GetBasePath();
			if (!basePath) {
				return ERROR_Set("Failed getting base path: %s", SDL_GetError());
			}
			if (SDL_asprintf(&FILESYSTEM_FilesDATPath, "%s%s", basePath, PROJECT_NAME ".dat") < 0) {
				FILESYSTEM_FilesDATPath = NULL;
				return ERROR_Set("Error creating resource DAT path string");
			}
			if (!SDL_GetPathInfo(FILESYSTEM_FilesDATPath, &info) || info.type != SDL_PATHTYPE_FILE) {
				return ERROR_Set("Resource DAT file was not found");
			}
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_PORTABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return ERROR_Set("Failed getting base path: %s", SDL_GetError());
		}
		if (SDL_asprintf(&FILESYSTEM_FilesDATPath, "%s%s", basePath, PROJECT_NAME ".dat") < 0) {
			FILESYSTEM_FilesDATPath = NULL;
			return ERROR_Set("Error creating resource DAT path string");
		}
		FILESYSTEM_PrefPath = SDL_strdup(basePath);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error duplicating base path string");
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_INSTALLABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return ERROR_Set("Failed getting base path: %s", SDL_GetError());
		}
		if (SDL_asprintf(&FILESYSTEM_FilesDATPath, "%s%s%s", basePath, FILESYSTEM_BASE_PATH_APPEND, PROJECT_NAME ".dat") < 0) {
			FILESYSTEM_FilesDATPath = NULL;
			return ERROR_Set("Error creating resource DAT path string");
		}
		FILESYSTEM_PrefPath = SDL_GetPrefPath(FILESYSTEM_ORG, FILESYSTEM_APP);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Failed getting pref path: %s", SDL_GetError());
		}
	}
#else
	#error FILESYSTEM_TYPE must be defined as FILESYSTEM_CURRENT_DIRECTORY, FILESYSTEM_PORTABLE, or FILESYSTEM_INSTALLABLE.
#endif

	FILESYSTEM_FilesTable = SDL_CreateProperties();
	if (!FILESYSTEM_FilesTable) {
		return ERROR_Set("Failed creating properties object for files in resource DAT: %s", SDL_GetError());
	}
	SDL_IOStream* datFile = SDL_IOFromFile(FILESYSTEM_FilesDATPath, "rb");
	if (!datFile) {
		ERROR_Set("Error opening resource DAT: %s", FILESYSTEM_FilesDATPath, SDL_GetError());
		return false;
	}
	int64_t filesCount;
	if (!SDL_ReadS64LE(datFile, &filesCount)) {
		return ERROR_Set("Error reading count of files in resource DAT: %s", SDL_GetError());
	}
	if (filesCount <= 0) {
		return ERROR_Set("Invalid count of files in resource DAT");
	}
	FILESYSTEM_FilesArray = SDL_malloc(filesCount * sizeof(FILESYSTEM_File));
	if (!FILESYSTEM_FilesArray) {
		return ERROR_Set("Error allocating array for resource DAT files metadata");
	}
	for (int64_t i = 0; i < filesCount; i++) {
		const int64_t filenamePos = SDL_TellIO(datFile);
		if (filenamePos < 0) {
			return ERROR_Set("Error getting position of filename in resource DAT file: %s", SDL_GetError());
		}
		int64_t filenameSize = 0;
		uint8_t c;
		do {
			if (!SDL_ReadU8(datFile, &c)) {
				return ERROR_Set("Error getting filename byte in resource DAT file: %s", SDL_GetError());
			}
			filenameSize++;
		} while (c != '\0');
		if (SDL_SeekIO(datFile, filenamePos, SDL_IO_SEEK_SET) < 0) {
			return ERROR_Set("Error seeking resource DAT file: %s", SDL_GetError());
		}
		char* const filename = SDL_malloc(filenameSize);
		if (!filename) {
			return ERROR_Set("Error allocating string for a file in resource DAT");
		}
		if (!SDL_ReadIO(datFile, filename, filenameSize)) {
			SDL_free(filename);
			return ERROR_Set("Error reading filename of a file in resource DAT");
		}
		if (!SDL_ReadS64LE(datFile, &FILESYSTEM_FilesArray[i].size)) {
			SDL_free(filename);
			return ERROR_Set("Error reading size of a file in resource DAT");
		}
		if (FILESYSTEM_FilesArray[i].size < 0) {
			SDL_free(filename);
			return ERROR_Set("Invalid size of a file in resource DAT");
		}
		FILESYSTEM_FilesArray[i].start = SDL_TellIO(datFile);
		if (FILESYSTEM_FilesArray[i].start < 0) {
			SDL_free(filename);
			return ERROR_Set("Error getting starting position of a file in resource DAT");
		}
		if (SDL_SeekIO(datFile, FILESYSTEM_FilesArray[i].size, SDL_IO_SEEK_CUR) < 0) {
			SDL_free(filename);
			return ERROR_Set("Error skipping over file data in resource DAT");
		}
		if (!SDL_SetPointerProperty(FILESYSTEM_FilesTable, filename, &FILESYSTEM_FilesArray[i])) {
			SDL_free(filename);
			return ERROR_Set("Error setting a property in the resource DAT files properties object: %s", SDL_GetError());
		}
		SDL_free(filename);
	}
	SDL_CloseIO(datFile);

	return true;
}

bool FILESYSTEM_FileExists(const char* filename)
{
	char* path;
	SDL_PathInfo info;

	if (SDL_asprintf(&path, "%s%s", FILESYSTEM_PrefPath, filename) < 0) {
		ERROR_Set("Error allocating string to check if file \"%s\" exists", filename);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	return SDL_HasProperty(FILESYSTEM_FilesTable, filename);
}

static int64_t SDLCALL FILESYSTEM_DATFileSize(void* userdata)
{
	FILESYSTEM_FileUserdata* const fileUserdata = userdata;
	return fileUserdata->file->size;
}

static int64_t SDLCALL FILESYSTEM_DATFileSeek(void* userdata, int64_t offset, SDL_IOWhence whence)
{
	FILESYSTEM_FileUserdata* const fileUserdata = userdata;
	int64_t pos;
	switch (whence) {
	default:
	case SDL_IO_SEEK_SET:
		if (offset < 0) {
			return -1;
		}
		else if (offset > fileUserdata->file->size) {
			return -1;
		}
		return
			SDL_SeekIO(fileUserdata->stream, fileUserdata->file->start + offset, SDL_IO_SEEK_SET) < 0 ?
			-1 :
			offset;

	case SDL_IO_SEEK_CUR:
		pos = SDL_TellIO(fileUserdata->stream);
		if (pos < 0) {
			return -1;
		}
		pos += offset;
		if (pos < fileUserdata->file->start || pos > fileUserdata->file->start + fileUserdata->file->size) {
			return -1;
		}
		return
			SDL_SeekIO(fileUserdata->stream, pos, SDL_IO_SEEK_SET) < 0 ?
			-1 :
			pos - fileUserdata->file->start;

	case SDL_IO_SEEK_END:
		if (offset > 0) {
			return -1;
		}
		else if (fileUserdata->file->size + offset < 0) {
			return -1;
		}
		return
			SDL_SeekIO(fileUserdata->stream, fileUserdata->file->start + fileUserdata->file->size + offset, SDL_IO_SEEK_SET) < 0 ?
			-1 :
			fileUserdata->file->size + offset;
	}
}

static size_t SDLCALL FILESYSTEM_DATFileRead(void* userdata, void* ptr, size_t size, SDL_IOStatus* status)
{
	FILESYSTEM_FileUserdata* const fileUserdata = userdata;
	const int64_t pos = SDL_TellIO(fileUserdata->stream);
	if (pos < 0) {
		*status = SDL_GetIOStatus(fileUserdata->stream);
		return 0;
	}
	if (pos + size > (size_t)fileUserdata->file->start + fileUserdata->file->size) {
		size = fileUserdata->file->size - (pos - fileUserdata->file->start);
		*status = SDL_IO_STATUS_EOF;
	}
	if (!SDL_ReadIO(fileUserdata->stream, ptr, size)) {
		*status = SDL_GetIOStatus(fileUserdata->stream);
		return 0;
	}
	return size;
}

static size_t SDLCALL FILESYSTEM_DATFileWrite(void* userdata, const void* ptr, size_t size, SDL_IOStatus* status)
{
	(void)userdata;
	(void)ptr;
	(void)size;
	*status = SDL_IO_STATUS_ERROR;
	return 0;
}

static bool SDLCALL FILESYSTEM_DATFileFlush(void* userdata, SDL_IOStatus* status)
{
	(void)userdata;
	*status = SDL_IO_STATUS_ERROR;
	return 0;
}

static bool SDLCALL FILESYSTEM_DATFileClose(void* userdata)
{
	FILESYSTEM_FileUserdata* const fileUserdata = userdata;
	SDL_CloseIO(fileUserdata->stream);
	SDL_free(fileUserdata);
	return true;
}

SDL_IOStream* FILESYSTEM_OpenRead(const char* filename)
{
	SDL_IOStream* prefStream = FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, filename, "rb");
	if (prefStream) {
		return prefStream;
	}

	FILESYSTEM_File* const file = SDL_GetPointerProperty(FILESYSTEM_FilesTable, filename, NULL);
	if (!file) {
		return NULL;
	}

	FILESYSTEM_FileUserdata* const fileUserdata = SDL_malloc(sizeof(FILESYSTEM_FileUserdata));
	if (!fileUserdata) {
		ERROR_Set("Failed allocating data to open file \"%s\" for reading from resource DAT", filename);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	fileUserdata->file = file;
	fileUserdata->stream = SDL_IOFromFile(FILESYSTEM_FilesDATPath, "rb");
	if (!fileUserdata->stream) {
		ERROR_Set("Failed opening resource DAT file for opening file \"%s\" for reading: %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (SDL_SeekIO(fileUserdata->stream, file->start, SDL_IO_SEEK_SET) < 0) {
		ERROR_Set("Failed seeking resource DAT to open file \"%s\" for reading: %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}

	SDL_IOStreamInterface interface;
	SDL_INIT_INTERFACE(&interface);
	interface.size = FILESYSTEM_DATFileSize;
	interface.seek = FILESYSTEM_DATFileSeek;
	interface.read = FILESYSTEM_DATFileRead;
	interface.write = FILESYSTEM_DATFileWrite;
	interface.flush = FILESYSTEM_DATFileFlush;
	interface.close = FILESYSTEM_DATFileClose;

	SDL_IOStream* datStream = SDL_OpenIO(&interface, fileUserdata);
	if (!datStream) {
		ERROR_Set("Failed creating a stream to read file \"%s\" in resource DAT: %s", filename, SDL_GetError());
		SDL_free(fileUserdata);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	return datStream;
}

void FILESYSTEM_Quit(void)
{
	SDL_free(FILESYSTEM_FilesDATPath);
	FILESYSTEM_FilesDATPath = NULL;
	SDL_free(FILESYSTEM_FilesArray);
	FILESYSTEM_FilesArray = NULL;
	SDL_DestroyProperties(FILESYSTEM_FilesTable);
	FILESYSTEM_FilesTable = 0;

	SDL_free(FILESYSTEM_PrefPath);
	FILESYSTEM_PrefPath = NULL;
}

#else

static char* FILESYSTEM_BasePath = NULL;

bool FILESYSTEM_Init(int argc, char** argv)
{
	if (argc > 1 && SDL_strlen(argv[1]) > 0) {
		FILESYSTEM_BasePath = SDL_strdup(argv[1]);
		if (!FILESYSTEM_BasePath) {
			return ERROR_Set("Error duplicating base path string");
		}
		FILESYSTEM_PrefPath = SDL_strdup(argv[1]);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error duplicating pref path string");
		}
	}
#if FILESYSTEM_TYPE == FILESYSTEM_CURRENT_DIRECTORY
	else {
		FILESYSTEM_BasePath = SDL_GetCurrentDirectory();
		if (!FILESYSTEM_BasePath) {
			return ERROR_Set("Error getting current directory: %s", SDL_GetError());
		}
		FILESYSTEM_PrefPath = SDL_strdup(FILESYSTEM_BasePath);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error duplicating current directory string");
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_PORTABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return ERROR_Set("Failed getting base path: %s", SDL_GetError());
		}
		FILESYSTEM_BasePath = SDL_strdup(basePath);
		if (!FILESYSTEM_BasePath) {
			return ERROR_Set("Error duplicating base path string");
		}
		FILESYSTEM_PrefPath = SDL_strdup(FILESYSTEM_BasePath);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error duplicating base path string");
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_INSTALLABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return ERROR_Set("Failed getting base path: %s", SDL_GetError());
		}
		if (SDL_asprintf(&FILESYSTEM_BasePath, "%s%s", basePath, FILESYSTEM_BASE_PATH_APPEND) < 0) {
			FILESYSTEM_BasePath = NULL;
			return ERROR_Set("Failed creating full base path");
		}
		FILESYSTEM_PrefPath = SDL_GetPrefPath(FILESYSTEM_ORG, FILESYSTEM_APP);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Failed getting pref path: %s", SDL_GetError());
		}
	}

#else
	#error FILESYSTEM_TYPE must be defined as FILESYSTEM_CURRENT_DIRECTORY, FILESYSTEM_PORTABLE, or FILESYSTEM_INSTALLABLE.

#endif

#ifdef SDL_PLATFORM_EMSCRIPTEN
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
#endif

	return true;
}

bool FILESYSTEM_FileExists(const char* filename)
{
	char* path;
	SDL_PathInfo info;

	if (SDL_asprintf(&path, "%s%s", FILESYSTEM_PrefPath, filename) < 0) {
		ERROR_Set("Error allocating string to check if file \"%s\" exists", filename);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	if (SDL_asprintf(&path, "%s%s", FILESYSTEM_BasePath, filename) < 0) {
		ERROR_Set("Error allocating string to check if file \"%s\" exists", filename);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	return false;
}

SDL_IOStream* FILESYSTEM_OpenRead(const char* filename)
{
	SDL_IOStream* file = FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, filename, "rb");
	if (file) {
		return file;
	}
	return FILESYSTEM_OpenFromPath(FILESYSTEM_BasePath, filename, "rb");
}

void FILESYSTEM_Quit(void)
{
#ifdef SDL_PLATFORM_EMSCRIPTEN
	EM_ASM({
		FS.syncfs(function (err) {
			assert(!err);
		});
	});
#endif

	SDL_free(FILESYSTEM_BasePath);
	FILESYSTEM_BasePath = NULL;
	SDL_free(FILESYSTEM_PrefPath);
	FILESYSTEM_PrefPath = NULL;
}

#endif

bool FILESYSTEM_CreateDirectory(const char* directory)
{
	char* path;

	if (SDL_asprintf(&path, "%s%s", FILESYSTEM_PrefPath, directory) < 0) {
		return ERROR_Set("Failed creating directory string for \"%s\"", directory);
	}
	else if (!SDL_CreateDirectory(path)) {
		SDL_free(path);
		return ERROR_Set("Failed creating directory \"%s\"", directory);
	}
	SDL_free(path);

#ifdef SDL_PLATFORM_EMSCRIPTEN
		EM_ASM({
			FS.syncfs(function (err) {
				assert(!err);
			});
		});
#endif

	return true;
}

SDL_IOStream* FILESYSTEM_OpenWrite(const char* filename)
{
	return FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, filename, "wb");
}

SDL_IOStream* FILESYSTEM_OpenAppend(const char* filename)
{
	return FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, filename, "ab");
}

static void FILESYSTEM_Swap32ArrayNativeToLE(int32_t* arrayNative, size_t count)
{
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	(void)arrayNative;
	(void)count;
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
	for (size_t i = 0; i < count; i++) {
		arrayNative[i] = SDL_Swap32(arrayNative[i]);
	}
#else
	#error Unsupported endianness!
#endif
}

static void FILESYSTEM_Swap32ArrayLEToNative(int32_t* arrayLE, size_t count)
{
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	(void)arrayLE;
	(void)count;
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
	for (size_t i = 0; i < count; i++) {
		arrayLE[i] = SDL_Swap32(arrayLE[i]);
	}
#else
	#error Unsupported endianness!
#endif
}

void FILESYSTEM_ReadFile32(const char* filename, int32_t* values, size_t count, size_t start)
{
	SDL_IOStream* src = FILESYSTEM_OpenRead(filename);
	if (!src) {
		return;
	}

	if (SDL_SeekIO(src, start * sizeof(int32_t), SDL_IO_SEEK_SET) < 0) {
		ERROR_Set("Error seeking file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (SDL_ReadIO(src, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		ERROR_Set("Error reading file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(src)) {
		ERROR_Set("Error closing file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}

	FILESYSTEM_Swap32ArrayLEToNative(values, count);
}

void FILESYSTEM_WriteFile32(const char* filename, int32_t* values, size_t count)
{
	SDL_IOStream* dst = FILESYSTEM_OpenWrite(filename);
	if (!dst) {
		return;
	}

	FILESYSTEM_Swap32ArrayNativeToLE(values, count);

	if (SDL_WriteIO(dst, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		ERROR_Set("Error writing file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(dst)) {
		ERROR_Set("Error closing file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}

	FILESYSTEM_Swap32ArrayLEToNative(values, count);
}

void FILESYSTEM_AppendFile32(const char* filename, int32_t* values, size_t count)
{
	SDL_IOStream* dst = FILESYSTEM_OpenAppend(filename);
	if (!dst) {
		return;
	}

	FILESYSTEM_Swap32ArrayNativeToLE(values, count);

	if (SDL_WriteIO(dst, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		ERROR_Set("Error writing file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(dst)) {
		ERROR_Set("Error closing file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}

	FILESYSTEM_Swap32ArrayLEToNative(values, count);
}
