#pragma once

#include "include.h"

bool FSInit(int argc, char** argv);
void FSDeInit();

bool FSMkdir(const char* const directory);

SDL_RWops* FSOpenRead(const char* const filename);
SDL_RWops* FSOpenWrite(const char* const filename);
SDL_RWops* FSOpenAppend(const char* const filename);