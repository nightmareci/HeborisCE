#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INT32_VALUE_TO_DATA(value, data) \
	(data)[0] = (((int32_t)(value) >>  0) & 0xFF); \
	(data)[1] = (((int32_t)(value) >>  8) & 0xFF); \
	(data)[2] = (((int32_t)(value) >> 16) & 0xFF); \
	(data)[3] = (((int32_t)(value) >> 24) & 0xFF);

#define INT64_VALUE_TO_DATA(value, data) \
	(data)[0] = (((int64_t)(value) >>  0) & 0xFF); \
	(data)[1] = (((int64_t)(value) >>  8) & 0xFF); \
	(data)[2] = (((int64_t)(value) >> 16) & 0xFF); \
	(data)[3] = (((int64_t)(value) >> 24) & 0xFF); \
	(data)[4] = (((int64_t)(value) >> 32) & 0xFF); \
	(data)[5] = (((int64_t)(value) >> 40) & 0xFF); \
	(data)[6] = (((int64_t)(value) >> 48) & 0xFF); \
	(data)[7] = (((int64_t)(value) >> 56) & 0xFF);

int main(int argc, char** argv)
{
	FILE* const resourceDAT = fopen(argv[1], "w");
	if (!resourceDAT) {
		return EXIT_FAILURE;
	}

	uint8_t fileCount[sizeof(int32_t)];
	INT32_VALUE_TO_DATA(argc - 3, fileCount);
	fwrite(fileCount, 1, sizeof(fileCount), resourceDAT);
	if (ferror(resourceDAT)) {
		fclose(resourceDAT);
		return EXIT_FAILURE;
	}

	const size_t basePathLength = strlen(argv[2]);

	uint8_t data[0x8000];
	for (int i = 3; i < argc; i++) {
		fwrite(argv[i], 1, strlen(argv[i]) + 1, resourceDAT);
		if (ferror(resourceDAT)) {
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}

		const size_t filenameLength = strlen(argv[i]);
		char* fullPathFilename = malloc(basePathLength + filenameLength + 1);
		if (!fullPathFilename) {
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}
		sprintf(fullPathFilename, "%s%s", argv[2], argv[i]);

		FILE* const file = fopen(fullPathFilename, "rb");
		free(fullPathFilename);
		if (!file) {
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}

		fseek(file, 0, SEEK_END);
		if (ferror(file)) {
			fclose(file);
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}
		const long fileSize = ftell(file);
		if (ferror(file)) {
			fclose(file);
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}
		fseek(file, 0, SEEK_SET);
		if (ferror(file)) {
			fclose(file);
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}

		uint8_t fileSizeData[sizeof(int64_t)];
		INT64_VALUE_TO_DATA(fileSize, fileSizeData);
		fwrite(fileSizeData, 1, sizeof(fileSizeData), resourceDAT);
		if (ferror(resourceDAT)) {
			fclose(file);
			fclose(resourceDAT);
			return EXIT_FAILURE;
		}

		for (long pos = 0; pos < fileSize; pos = ftell(file)) {
			const unsigned long amountRead = fread(data, 1, sizeof(data), file);
			if (ferror(file)) {
				fclose(file);
				fclose(resourceDAT);
				return EXIT_FAILURE;
			}
			fwrite(data, 1, amountRead, resourceDAT);
			if (ferror(resourceDAT)) {
				fclose(file);
				fclose(resourceDAT);
				return EXIT_FAILURE;
			}
		}

		fclose(file);
	}

	fclose(resourceDAT);

	return EXIT_SUCCESS;
}
