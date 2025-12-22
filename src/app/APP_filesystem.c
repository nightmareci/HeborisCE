#include "APP_filesystem.h"
#include "APP_main.h"
#include "APP_error.h"

static char* APP_PrefPath = NULL;

static SDL_IOStream* APP_OpenFromPath(const char* path, const char* filename, const char* mode)
{
	char* fullPath;
	if (SDL_asprintf(&fullPath, "%s%s", path, filename) < 0) {
		APP_SetError("Error allocating string to open a file");
		APP_Exit(SDL_APP_FAILURE);
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

#ifdef APP_RESOURCE_DAT

typedef struct APP_File
{
	int64_t start;
	int64_t size;
} APP_File;

typedef struct APP_FileUserdata
{
	APP_File* file;
	SDL_IOStream* stream;
} APP_FileUserdata;

static char* APP_FilesDATPath = NULL;
static APP_File* APP_FilesArray = NULL;
static SDL_PropertiesID APP_FilesTable = 0;

bool APP_InitFilesystem(int argc, char** argv)
{
	if (argc > 1 && SDL_strlen(argv[1]) > 0) {
		if (SDL_asprintf(&APP_FilesDATPath, "%s%s", argv[1], APP_PROJECT_NAME ".dat") < 0) {
			APP_FilesDATPath = NULL;
			return APP_SetError("Error creating resource DAT path string");
		}
		APP_PrefPath = SDL_strdup(argv[1]);
		if (!APP_PrefPath) {
			return APP_SetError("Error duplicating pref path string");
		}
	}
#if APP_FILESYSTEM_TYPE == APP_FILESYSTEM_CURRENT_DIRECTORY
	else {
		APP_PrefPath = SDL_GetCurrentDirectory();
		if (!APP_PrefPath) {
			return APP_SetError("Error getting current directory: %s", SDL_GetError());
		}
		if (SDL_asprintf(&APP_FilesDATPath, "%s%s", APP_PrefPath, APP_PROJECT_NAME ".dat") < 0) {
			APP_FilesDATPath = NULL;
			return APP_SetError("Error creating resource DAT path string");
		}
		SDL_PathInfo info;
		if (!SDL_GetPathInfo(APP_FilesDATPath, &info) || info.type != SDL_PATHTYPE_FILE) {
			SDL_free(APP_FilesDATPath);
			const char* basePath = SDL_GetBasePath();
			if (!basePath) {
				return APP_SetError("Failed getting base path: %s", SDL_GetError());
			}
			if (SDL_asprintf(&APP_FilesDATPath, "%s%s", basePath, APP_PROJECT_NAME ".dat") < 0) {
				APP_FilesDATPath = NULL;
				return APP_SetError("Error creating resource DAT path string");
			}
			if (!SDL_GetPathInfo(APP_FilesDATPath, &info) || info.type != SDL_PATHTYPE_FILE) {
				return APP_SetError("Resource DAT file was not found");
			}
		}
	}
#elif APP_FILESYSTEM_TYPE == APP_FILESYSTEM_PORTABLE
	else {
		const char* basePath = SDL_GetBasePath();
		if (!basePath) {
			return APP_SetError("Failed getting base path: %s", SDL_GetError());
		}
		if (SDL_asprintf(&APP_FilesDATPath, "%s%s", basePath, APP_PROJECT_NAME ".dat") < 0) {
			APP_FilesDATPath = NULL;
			return APP_SetError("Error creating resource DAT path string");
		}
		APP_PrefPath = SDL_strdup(basePath);
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
		if (SDL_asprintf(&APP_FilesDATPath, "%s%s%s", basePath, APP_BASE_PATH_APPEND, APP_PROJECT_NAME ".dat") < 0) {
			APP_FilesDATPath = NULL;
			return APP_SetError("Error creating resource DAT path string");
		}
		APP_PrefPath = SDL_GetPrefPath(APP_FILESYSTEM_ORG, APP_FILESYSTEM_APP);
		if (!APP_PrefPath) {
			return APP_SetError("Failed getting pref path: %s", SDL_GetError());
		}
	}
#else
	#error APP_FILESYSTEM_TYPE must be defined as APP_FILESYSTEM_CURRENT_DIRECTORY, APP_FILESYSTEM_PORTABLE, or APP_FILESYSTEM_INSTALLABLE.
#endif

	APP_FilesTable = SDL_CreateProperties();
	if (!APP_FilesTable) {
		return APP_SetError("Failed creating properties object for files in resource DAT: %s", SDL_GetError());
	}
	SDL_IOStream* datFile = SDL_IOFromFile(APP_FilesDATPath, "rb");
	if (!datFile) {
		APP_SetError("Error opening resource DAT: %s", APP_FilesDATPath, SDL_GetError());
		return false;
	}
	int64_t filesCount;
	if (!SDL_ReadS64LE(datFile, &filesCount)) {
		return APP_SetError("Error reading count of files in resource DAT: %s", SDL_GetError());
	}
	if (filesCount <= 0) {
		return APP_SetError("Invalid count of files in resource DAT");
	}
	APP_FilesArray = SDL_malloc(filesCount * sizeof(APP_File));
	if (!APP_FilesArray) {
		return APP_SetError("Error allocating array for resource DAT files metadata");
	}
	for (int64_t i = 0; i < filesCount; i++) {
		const int64_t filenamePos = SDL_TellIO(datFile);
		if (filenamePos < 0) {
			return APP_SetError("Error getting position of filename in resource DAT file: %s", SDL_GetError());
		}
		int64_t filenameSize = 0;
		uint8_t c;
		do {
			if (!SDL_ReadU8(datFile, &c)) {
				return APP_SetError("Error getting filename byte in resource DAT file: %s", SDL_GetError());
			}
			filenameSize++;
		} while (c != '\0');
		if (SDL_SeekIO(datFile, filenamePos, SDL_IO_SEEK_SET) < 0) {
			return APP_SetError("Error seeking resource DAT file: %s", SDL_GetError());
		}
		char* const filename = SDL_malloc(filenameSize);
		if (!filename) {
			return APP_SetError("Error allocating string for a file in resource DAT");
		}
		if (!SDL_ReadIO(datFile, filename, filenameSize)) {
			SDL_free(filename);
			return APP_SetError("Error reading filename of a file in resource DAT");
		}
		if (!SDL_ReadS64LE(datFile, &APP_FilesArray[i].size)) {
			SDL_free(filename);
			return APP_SetError("Error reading size of a file in resource DAT");
		}
		if (APP_FilesArray[i].size < 0) {
			SDL_free(filename);
			return APP_SetError("Invalid size of a file in resource DAT");
		}
		APP_FilesArray[i].start = SDL_TellIO(datFile);
		if (APP_FilesArray[i].start < 0) {
			SDL_free(filename);
			return APP_SetError("Error getting starting position of a file in resource DAT");
		}
		if (SDL_SeekIO(datFile, APP_FilesArray[i].size, SDL_IO_SEEK_CUR) < 0) {
			SDL_free(filename);
			return APP_SetError("Error skipping over file data in resource DAT");
		}
		if (!SDL_SetPointerProperty(APP_FilesTable, filename, &APP_FilesArray[i])) {
			SDL_free(filename);
			return APP_SetError("Error setting a property in the resource DAT files properties object: %s", SDL_GetError());
		}
		SDL_free(filename);
	}
	SDL_CloseIO(datFile);

	return true;
}

bool APP_FileExists(const char* filename)
{
	char* path;
	SDL_PathInfo info;

	if (SDL_asprintf(&path, "%s%s", APP_PrefPath, filename) < 0) {
		APP_SetError("Error allocating string to check if file \"%s\" exists", filename);
		APP_Exit(SDL_APP_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	return SDL_HasProperty(APP_FilesTable, filename);
}

static int64_t SDLCALL APP_DATFileSize(void* userdata)
{
	APP_FileUserdata* const fileUserdata = userdata;
	return fileUserdata->file->size;
}

static int64_t SDLCALL APP_DATFileSeek(void* userdata, int64_t offset, SDL_IOWhence whence)
{
	APP_FileUserdata* const fileUserdata = userdata;
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

static size_t SDLCALL APP_DATFileRead(void* userdata, void* ptr, size_t size, SDL_IOStatus* status)
{
	APP_FileUserdata* const fileUserdata = userdata;
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

static size_t SDLCALL APP_DATFileWrite(void* userdata, const void* ptr, size_t size, SDL_IOStatus* status)
{
	(void)userdata;
	(void)ptr;
	(void)size;
	*status = SDL_IO_STATUS_ERROR;
	return 0;
}

static bool SDLCALL APP_DATFileFlush(void* userdata, SDL_IOStatus* status)
{
	(void)userdata;
	*status = SDL_IO_STATUS_ERROR;
	return 0;
}

static bool SDLCALL APP_DATFileClose(void* userdata)
{
	APP_FileUserdata* const fileUserdata = userdata;
	SDL_CloseIO(fileUserdata->stream);
	SDL_free(fileUserdata);
	return true;
}

SDL_IOStream* APP_OpenRead(const char* filename)
{
	SDL_IOStream* prefStream = APP_OpenFromPath(APP_PrefPath, filename, "rb");
	if (prefStream) {
		return prefStream;
	}

	APP_File* const file = SDL_GetPointerProperty(APP_FilesTable, filename, NULL);
	if (!file) {
		return NULL;
	}

	APP_FileUserdata* const fileUserdata = SDL_malloc(sizeof(APP_FileUserdata));
	if (!fileUserdata) {
		APP_SetError("Failed allocating data to open file \"%s\" for reading from resource DAT", filename);
		APP_Exit(SDL_APP_FAILURE);
	}
	fileUserdata->file = file;
	fileUserdata->stream = SDL_IOFromFile(APP_FilesDATPath, "rb");
	if (!fileUserdata->stream) {
		APP_SetError("Failed opening resource DAT file for opening file \"%s\" for reading: %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
	if (SDL_SeekIO(fileUserdata->stream, file->start, SDL_IO_SEEK_SET) < 0) {
		APP_SetError("Failed seeking resource DAT to open file \"%s\" for reading: %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}

	SDL_IOStreamInterface interface;
	SDL_INIT_INTERFACE(&interface);
	interface.size = APP_DATFileSize;
	interface.seek = APP_DATFileSeek;
	interface.read = APP_DATFileRead;
	interface.write = APP_DATFileWrite;
	interface.flush = APP_DATFileFlush;
	interface.close = APP_DATFileClose;

	SDL_IOStream* datStream = SDL_OpenIO(&interface, fileUserdata);
	if (!datStream) {
		APP_SetError("Failed creating a stream to read file \"%s\" in resource DAT: %s", filename, SDL_GetError());
		SDL_free(fileUserdata);
		APP_Exit(SDL_APP_FAILURE);
	}
	return datStream;
}

void APP_QuitFilesystem(void)
{
	SDL_free(APP_FilesDATPath);
	APP_FilesDATPath = NULL;
	SDL_free(APP_FilesArray);
	APP_FilesArray = NULL;
	SDL_DestroyProperties(APP_FilesTable);
	APP_FilesTable = 0;

	SDL_free(APP_PrefPath);
	APP_PrefPath = NULL;
}

#else

static char* APP_BasePath = NULL;

bool APP_InitFilesystem(int argc, char** argv)
{
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
			APP_BasePath = NULL;
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

#ifdef SDL_PLATFORM_EMSCRIPTEN
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
		APP_SetError("Error allocating string to check if file \"%s\" exists", filename);
		APP_Exit(SDL_APP_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	if (SDL_asprintf(&path, "%s%s", APP_BasePath, filename) < 0) {
		APP_SetError("Error allocating string to check if file \"%s\" exists", filename);
		APP_Exit(SDL_APP_FAILURE);
	}
	if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
		SDL_free(path);
		return true;
	}
	SDL_free(path);

	return false;
}

SDL_IOStream* APP_OpenRead(const char* filename)
{
	SDL_IOStream* file = APP_OpenFromPath(APP_PrefPath, filename, "rb");
	if (file) {
		return file;
	}
	return APP_OpenFromPath(APP_BasePath, filename, "rb");
}

void APP_QuitFilesystem(void)
{
#ifdef SDL_PLATFORM_EMSCRIPTEN
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

#endif

bool APP_CreateDirectory(const char* directory)
{
	char* path;

	if (SDL_asprintf(&path, "%s%s", APP_PrefPath, directory) < 0) {
		return APP_SetError("Failed creating directory string for \"%s\"", directory);
	}
	else if (!SDL_CreateDirectory(path)) {
		SDL_free(path);
		return APP_SetError("Failed creating directory \"%s\"", directory);
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

SDL_IOStream* APP_OpenWrite(const char* filename)
{
	return APP_OpenFromPath(APP_PrefPath, filename, "wb");
}

SDL_IOStream* APP_OpenAppend(const char* filename)
{
	return APP_OpenFromPath(APP_PrefPath, filename, "ab");
}

static void APP_Swap32ArrayNativeToLE(int32_t* arrayNative, size_t count)
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

static void APP_Swap32ArrayLEToNative(int32_t* arrayLE, size_t count)
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

void APP_ReadFile32(const char* filename, int32_t* values, size_t count, size_t start)
{
	SDL_IOStream* src = APP_OpenRead(filename);
	if (!src) {
		return;
	}

	if (SDL_SeekIO(src, start * sizeof(int32_t), SDL_IO_SEEK_SET) < 0) {
		APP_SetError("Error seeking file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
	else if (SDL_ReadIO(src, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		APP_SetError("Error reading file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(src)) {
		APP_SetError("Error closing file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}

	APP_Swap32ArrayLEToNative(values, count);
}

void APP_WriteFile32(const char* filename, int32_t* values, size_t count)
{
	SDL_IOStream* dst = APP_OpenWrite(filename);
	if (!dst) {
		return;
	}

	APP_Swap32ArrayNativeToLE(values, count);

	if (SDL_WriteIO(dst, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		APP_SetError("Error writing file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(dst)) {
		APP_SetError("Error closing file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}

	APP_Swap32ArrayLEToNative(values, count);
}

void APP_AppendFile32(const char* filename, int32_t* values, size_t count)
{
	SDL_IOStream* dst = APP_OpenAppend(filename);
	if (!dst) {
		return;
	}

	APP_Swap32ArrayNativeToLE(values, count);

	if (SDL_WriteIO(dst, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		APP_SetError("Error writing file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(dst)) {
		APP_SetError("Error closing file \"%s\": %s", filename, SDL_GetError());
		APP_Exit(SDL_APP_FAILURE);
	}

	APP_Swap32ArrayLEToNative(values, count);
}
