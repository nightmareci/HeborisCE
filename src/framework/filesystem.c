#include <framework/filesystem.h>
#include <framework/main.h>
#include "filesystem_private.h"
#include "main_private.h"
#include "error.h"

typedef struct FILESYSTEM_DATFile
{
	int64_t start;
	int64_t size;
} FILESYSTEM_DATFile;

typedef struct FILESYSTEM_DATFileUserdata
{
	FILESYSTEM_DATFile* file;
	SDL_IOStream* stream;
} FILESYSTEM_DATFileUserdata;

static char* FILESYSTEM_PrefPath = NULL;
static char* FILESYSTEM_DATPrefPath = NULL;
static SDL_PropertiesID FILESYSTEM_DATPrefFiles = 0;
static FILESYSTEM_DATFile* FILESYSTEM_DATPrefFilesArray = NULL;

static char* FILESYSTEM_BasePath = NULL;
static char* FILESYSTEM_DATBasePath = NULL;
static SDL_PropertiesID FILESYSTEM_DATBaseFiles = 0;
static FILESYSTEM_DATFile* FILESYSTEM_DATBaseFilesArray = NULL;

static SDL_IOStream* FILESYSTEM_OpenFromPath(const char* path, const char* pathName, const char* filename, const char* mode)
{
	char* fullPath;
	if (SDL_asprintf(&fullPath, "%s%s", path, filename) < 0) {
		ERROR_Set("Error allocating string to open a file from %s path", pathName);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	SDL_PathInfo info;
	if (SDL_strchr(mode, 'w') == NULL && SDL_strchr(mode, 'a') == NULL && (!SDL_GetPathInfo(fullPath, &info) || info.type != SDL_PATHTYPE_FILE)) {
		SDL_free(fullPath);
		return NULL;
	}

	SDL_IOStream* const stream = SDL_IOFromFile(fullPath, mode);
	SDL_free(fullPath);
	if (!stream) {
		ERROR_Set("Error opening file from %s path: %s", pathName, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	return stream;
}

static bool FILESYSTEM_InitDATFiles(SDL_PropertiesID* files, FILESYSTEM_DATFile** filesArray, const char* path, const char* pathName)
{
	*files = SDL_CreateProperties();
	if (!*files) {
		return ERROR_Set("Failed creating properties object for files in %s path resource DAT: %s", pathName, SDL_GetError());
	}
	SDL_IOStream* const DATFile = SDL_IOFromFile(path, "rb");
	if (!DATFile) {
		return ERROR_Set("Error opening %s path resource DAT: %s", pathName, SDL_GetError());
	}
	int64_t filesCount;
	if (!SDL_ReadS64LE(DATFile, &filesCount)) {
		if (!SDL_CloseIO(DATFile)) {
			return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
		}
		else {
			return ERROR_Set("Error reading count of files in %s path resource DAT: %s", pathName, SDL_GetError());
		}
	}
	if (filesCount <= 0) {
		if (!SDL_CloseIO(DATFile)) {
			return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
		}
		else {
			return ERROR_Set("Invalid count of files in %s path resource DAT", pathName);
		}
	}
	*filesArray = SDL_malloc(filesCount * sizeof(FILESYSTEM_DATFile));
	if (!*filesArray) {
		if (!SDL_CloseIO(DATFile)) {
			return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
		}
		else {
			return ERROR_Set("Error allocating array for %s path resource DAT files metadata", pathName);
		}
	}
	for (int64_t i = 0; i < filesCount; i++) {
		const int64_t filenamePos = SDL_TellIO(DATFile);
		if (filenamePos < 0) {
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error getting position of filename in %s path resource DAT file: %s", pathName, SDL_GetError());
			}
		}
		int64_t filenameSize = 0;
		uint8_t c;
		do {
			if (!SDL_ReadU8(DATFile, &c)) {
				if (!SDL_CloseIO(DATFile)) {
					return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
				}
				else {
					return ERROR_Set("Error getting filename byte in %s path resource DAT file: %s", pathName, SDL_GetError());
				}
			}
			filenameSize++;
		} while (c != '\0');
		if (SDL_SeekIO(DATFile, filenamePos, SDL_IO_SEEK_SET) < 0) {
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error seeking %s path resource DAT file: %s", pathName, SDL_GetError());
			}
		}
		char* const filename = SDL_malloc(filenameSize);
		if (!filename) {
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error allocating string for a file in %s path resource DAT", pathName);
			}
		}
		if (!SDL_ReadIO(DATFile, filename, filenameSize)) {
			SDL_free(filename);
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error reading filename of a file in %s path resource DAT", pathName);
			}
		}
		if (!SDL_ReadS64LE(DATFile, &(*filesArray)[i].size)) {
			SDL_free(filename);
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error reading size of a file in %s path resource DAT", pathName);
			}
		}
		if ((*filesArray)[i].size < 0) {
			SDL_free(filename);
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Invalid size of a file in %s path resource DAT", pathName);
			}
		}
		(*filesArray)[i].start = SDL_TellIO(DATFile);
		if ((*filesArray)[i].start < 0) {
			SDL_free(filename);
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error getting starting position of a file in %s path resource DAT", pathName);
			}
		}
		if (SDL_SeekIO(DATFile, (*filesArray)[i].size, SDL_IO_SEEK_CUR) < 0) {
			SDL_free(filename);
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error skipping over file data in %s path resource DAT", pathName);
			}
		}
		if (!SDL_SetPointerProperty(*files, filename, &(*filesArray)[i])) {
			SDL_free(filename);
			if (!SDL_CloseIO(DATFile)) {
				return ERROR_Set("Error closing %s path resource DAT: %s", pathName, SDL_GetError());
			}
			else {
				return ERROR_Set("Error setting a property in the %s path resource DAT files properties object: %s", pathName, SDL_GetError());
			}
		}
		SDL_free(filename);
	}
	return SDL_CloseIO(DATFile);
}

bool FILESYSTEM_Init(int argc, char** argv)
{
	if (argc > 1 && SDL_strlen(argv[1]) > 0) {
		SDL_PathInfo info;
		if (!SDL_GetPathInfo(argv[1], &info) || info.type != SDL_PATHTYPE_DIRECTORY) {
			return ERROR_Set("Directory provided as a program argument isn't valid");
		}
		FILESYSTEM_PrefPath = SDL_strdup(argv[1]);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error creating pref path string");
		}
		if (SDL_asprintf(&FILESYSTEM_DATPrefPath, "%s%s", argv[1], PROJECT_NAME ".dat") < 0) {
			FILESYSTEM_DATPrefPath = NULL;
			return ERROR_Set("Error creating resource DAT pref path string");
		}
	}
#if FILESYSTEM_TYPE == FILESYSTEM_CURRENT_DIRECTORY
	else {
		char* const currentDirectory = SDL_GetCurrentDirectory();
		if (!currentDirectory) {
			return ERROR_Set("Error getting current directory string");
		}

		FILESYSTEM_PrefPath = currentDirectory;

		SDL_PathInfo info;
		char* DATFile;
		if (SDL_asprintf(&DATFile, "%s%s", currentDirectory, PROJECT_NAME ".dat") < 0) {
			return ERROR_Set("Error creating resource DAT file current directory string");
		}
		if (SDL_GetPathInfo(DATFile, &info) && info.type == SDL_PATHTYPE_FILE) {
			FILESYSTEM_DATPrefPath = DATFile;
		}
		else {
			SDL_free(DATFile);
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_PORTABLE
	else {
		const char* const basePath = SDL_GetBasePath();
		if (!basePath) {
			return ERROR_Set("Failed getting base path: %s", SDL_GetError());
		}

		FILESYSTEM_PrefPath = SDL_strdup(basePath);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Error creating pref path string");
		}

		SDL_PathInfo info;
		char* DATFile;
		if (SDL_asprintf(&DATFile, "%s%s", basePath, PROJECT_NAME ".dat") < 0) {
			return ERROR_Set("Error creating resource DAT file string");
		}
		if (SDL_GetPathInfo(DATFile, &info) && info.type == SDL_PATHTYPE_FILE) {
			FILESYSTEM_DATPrefPath = DATFile;
		}
		else {
			SDL_free(DATFile);
		}
	}
#elif FILESYSTEM_TYPE == FILESYSTEM_INSTALLABLE
	else {
		FILESYSTEM_PrefPath = SDL_GetPrefPath(FILESYSTEM_ORG, FILESYSTEM_APP);
		if (!FILESYSTEM_PrefPath) {
			return ERROR_Set("Failed getting pref path: %s", SDL_GetError());
		}
		const char* const basePath = SDL_GetBasePath();
		if (!basePath) {
			return ERROR_Set("Failed getting base path: %s", SDL_GetError());
		}
		if (SDL_asprintf(&FILESYSTEM_BasePath, "%s%s", basePath, FILESYSTEM_BASE_PATH_APPEND) < 0) {
			FILESYSTEM_BasePath = NULL;
			return ERROR_Set("Error creating base path string");
		}

		SDL_PathInfo info;
		char* DATFile;
		if (SDL_asprintf(&DATFile, "%s%s", FILESYSTEM_PrefPath, PROJECT_NAME ".dat") < 0) {
			return ERROR_Set("Error creating resource DAT file pref path string");
		}
		if (SDL_GetPathInfo(DATFile, &info) && info.type == SDL_PATHTYPE_FILE) {
			FILESYSTEM_DATPrefPath = DATFile;
		}
		else {
			SDL_free(DATFile);
		}
		if (SDL_asprintf(&DATFile, "%s%s", FILESYSTEM_BasePath, PROJECT_NAME ".dat") < 0) {
			return ERROR_Set("Error creating resource DAT file base path string");
		}
		if (SDL_GetPathInfo(DATFile, &info) && info.type == SDL_PATHTYPE_FILE) {
			FILESYSTEM_DATBasePath = DATFile;
		}
		else {
			SDL_free(DATFile);
		}
	}
#else
	#error FILESYSTEM_TYPE must be defined as FILESYSTEM_CURRENT_DIRECTORY, FILESYSTEM_PORTABLE, or FILESYSTEM_INSTALLABLE.
#endif

	if (FILESYSTEM_DATPrefPath && !FILESYSTEM_InitDATFiles(&FILESYSTEM_DATPrefFiles, &FILESYSTEM_DATPrefFilesArray, FILESYSTEM_DATPrefPath, "pref")) {
		return false;
	}
	if (FILESYSTEM_DATBasePath && !FILESYSTEM_InitDATFiles(&FILESYSTEM_DATBaseFiles, &FILESYSTEM_DATBaseFilesArray, FILESYSTEM_DATBasePath, "base")) {
		return false;
	}

	return true;
}

bool FILESYSTEM_FileExists(const char* filename)
{
	char* path;
	SDL_PathInfo info;
	if (FILESYSTEM_PrefPath) {
		if (SDL_asprintf(&path, "%s%s", FILESYSTEM_PrefPath, filename) < 0) {
			ERROR_Set("Error allocating string to check if file \"%s\" exists", filename);
			MAIN_Exit(SDL_APP_FAILURE);
		}
		if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
			SDL_free(path);
			return true;
		}
		SDL_free(path);
	}
	if (FILESYSTEM_DATPrefFiles != 0 && SDL_HasProperty(FILESYSTEM_DATPrefFiles, filename)) {
		return true;
	}
	if (FILESYSTEM_BasePath) {
		if (SDL_asprintf(&path, "%s%s", FILESYSTEM_BasePath, filename) < 0) {
			ERROR_Set("Error allocating string to check if file \"%s\" exists", filename);
			MAIN_Exit(SDL_APP_FAILURE);
		}
		if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE) {
			SDL_free(path);
			return true;
		}
		SDL_free(path);
	}
	return FILESYSTEM_DATBaseFiles != 0 && SDL_HasProperty(FILESYSTEM_DATBaseFiles, filename);
}

static int64_t SDLCALL FILESYSTEM_DATFileSize(void* userdata)
{
	FILESYSTEM_DATFileUserdata* const fileUserdata = userdata;
	return fileUserdata->file->size;
}

static int64_t SDLCALL FILESYSTEM_DATFileSeek(void* userdata, int64_t offset, SDL_IOWhence whence)
{
	FILESYSTEM_DATFileUserdata* const fileUserdata = userdata;
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
	FILESYSTEM_DATFileUserdata* const fileUserdata = userdata;
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
	FILESYSTEM_DATFileUserdata* const fileUserdata = userdata;
	const bool success = SDL_CloseIO(fileUserdata->stream);
	SDL_free(fileUserdata);
	return success;
}

static SDL_IOStream* FILESYSTEM_OpenFromDAT(SDL_PropertiesID files, const char* filesPath, const char* pathName, const char* filename)
{
	FILESYSTEM_DATFile* const file = SDL_GetPointerProperty(files, filename, NULL);
	if (!file) {
		return NULL;
	}

	FILESYSTEM_DATFileUserdata* const fileUserdata = SDL_malloc(sizeof(FILESYSTEM_DATFileUserdata));
	if (!fileUserdata) {
		ERROR_Set("Failed allocating data to open file \"%s\" for reading from %s path resource DAT", pathName, filename);
		MAIN_Exit(SDL_APP_FAILURE);
	}
	fileUserdata->file = file;
	fileUserdata->stream = SDL_IOFromFile(filesPath, "rb");
	if (!fileUserdata->stream) {
		SDL_free(fileUserdata);
		ERROR_Set("Failed opening %s path resource DAT file for opening file \"%s\" for reading: %s", pathName, filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}
	if (SDL_SeekIO(fileUserdata->stream, file->start, SDL_IO_SEEK_SET) < 0) {
		SDL_free(fileUserdata);
		if (!SDL_CloseIO(fileUserdata->stream)) {
			ERROR_Set("Failed closing %s path resource DAT after failing to open file \"%s\" for reading: %s", pathName, filename, SDL_GetError());
		}
		else {
			ERROR_Set("Failed seeking %s path resource DAT to open file \"%s\" for reading: %s", pathName, filename, SDL_GetError());
		}
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
		SDL_free(fileUserdata);
		if (!SDL_CloseIO(fileUserdata->stream)) {
			ERROR_Set("Failed closing %s path resource DAT after failing to open file \"%s\" for reading: %s", pathName, filename, SDL_GetError());
		}
		else {
			ERROR_Set("Failed creating a stream to open file \"%s\" in %s path resource DAT for reading: %s", filename, pathName, SDL_GetError());
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	return datStream;
}

SDL_IOStream* FILESYSTEM_OpenRead(const char* filename)
{
	SDL_IOStream* stream;

	stream = FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, "pref", filename, "rb");
	if (stream) {
		return stream;
	}

	stream = FILESYSTEM_OpenFromDAT(FILESYSTEM_DATPrefFiles, FILESYSTEM_DATPrefPath, "pref", filename);
	if (stream) {
		return stream;
	}

	stream = FILESYSTEM_OpenFromPath(FILESYSTEM_BasePath, "base", filename, "rb");
	if (stream) {
		return stream;
	}

	stream = FILESYSTEM_OpenFromDAT(FILESYSTEM_DATBaseFiles, FILESYSTEM_DATBasePath, "base", filename);
	if (stream) {
		return stream;
	}

	return NULL;
}

void FILESYSTEM_Quit(void)
{
	SDL_free(FILESYSTEM_PrefPath);
	FILESYSTEM_PrefPath = NULL;

	SDL_free(FILESYSTEM_DATBasePath);
	FILESYSTEM_DATBasePath = NULL;

	SDL_free(FILESYSTEM_DATPrefFilesArray);
	FILESYSTEM_DATPrefFilesArray = NULL;
	SDL_DestroyProperties(FILESYSTEM_DATPrefFiles);
	FILESYSTEM_DATPrefFiles = 0;

	SDL_free(FILESYSTEM_DATBaseFilesArray);
	FILESYSTEM_DATBaseFilesArray = NULL;
	SDL_DestroyProperties(FILESYSTEM_DATBaseFiles);
	FILESYSTEM_DATBaseFiles = 0;
}

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

	return true;
}

SDL_IOStream* FILESYSTEM_OpenWrite(const char* filename)
{
	return FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, "pref", filename, "wb");
}

SDL_IOStream* FILESYSTEM_OpenAppend(const char* filename)
{
	return FILESYSTEM_OpenFromPath(FILESYSTEM_PrefPath, "pref", filename, "ab");
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
	SDL_IOStream* const src = FILESYSTEM_OpenRead(filename);
	if (!src) {
		return;
	}

	const int64_t srcSize = SDL_GetIOSize(src);
	if (srcSize < 0) {
		if (!SDL_CloseIO(src)) {
			ERROR_Set("Error closing file \"%s\" after failing to get its size: %s", filename, SDL_GetError());
		}
		else {
			ERROR_Set("Error getting size of file \"%s\": %s", filename, SDL_GetError());
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (srcSize == 0) {
		if (!SDL_CloseIO(src)) {
			ERROR_Set("Error closing file \"%s\": %s", filename, SDL_GetError());
			MAIN_Exit(SDL_APP_FAILURE);
		}
		return;
	}

	if (count > (srcSize / sizeof(int32_t)) - start) {
		count = (srcSize / sizeof(int32_t)) - start;
	}

	if (SDL_SeekIO(src, start * sizeof(int32_t), SDL_IO_SEEK_SET) < 0) {
		if (!SDL_CloseIO(src)) {
			ERROR_Set("Error closing file \"%s\" after failing to seek it for reading: %s", filename, SDL_GetError());
		}
		else {
			ERROR_Set("Error seeking file \"%s\": %s", filename, SDL_GetError());
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (SDL_ReadIO(src, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		if (!SDL_CloseIO(src)) {
			ERROR_Set("Error closing file \"%s\" after failing to read it: %s", filename, SDL_GetError());
		}
		else {
			ERROR_Set("Error reading file \"%s\": %s", filename, SDL_GetError());
		}
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
	SDL_IOStream* const dst = FILESYSTEM_OpenWrite(filename);
	if (!dst) {
		return;
	}

	FILESYSTEM_Swap32ArrayNativeToLE(values, count);

	if (SDL_WriteIO(dst, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		if (!SDL_CloseIO(dst)) {
			ERROR_Set("Error closing file \"%s\" after failing to write to it: %s", filename, SDL_GetError());
		}
		else {
			ERROR_Set("Error writing file \"%s\": %s", filename, SDL_GetError());
		}
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
	SDL_IOStream* const dst = FILESYSTEM_OpenAppend(filename);
	if (!dst) {
		return;
	}

	FILESYSTEM_Swap32ArrayNativeToLE(values, count);

	if (SDL_WriteIO(dst, values, count * sizeof(int32_t)) != count * sizeof(int32_t)) {
		if (!SDL_CloseIO(dst)) {
			ERROR_Set("Error closing file \"%s\" after failing to append to it: %s", filename, SDL_GetError());
		}
		else {
			ERROR_Set("Error appending to file \"%s\": %s", filename, SDL_GetError());
		}
		MAIN_Exit(SDL_APP_FAILURE);
	}
	else if (!SDL_CloseIO(dst)) {
		ERROR_Set("Error closing file \"%s\": %s", filename, SDL_GetError());
		MAIN_Exit(SDL_APP_FAILURE);
	}

	FILESYSTEM_Swap32ArrayLEToNative(values, count);
}
