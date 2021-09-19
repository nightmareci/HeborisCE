#include "physfs_rwops.h"
#include "physfs.h"

static Sint64 RWsize( struct SDL_RWops *context )
{
	PHYSFS_File *file = context->hidden.unknown.data1;
	return PHYSFS_fileLength(file);
}

static Sint64 RWseek( struct SDL_RWops *context, Sint64 offset, int whence)
{
	PHYSFS_File *file = context->hidden.unknown.data1;
	PHYSFS_uint64 pos = 0u;
	switch( whence )
	{
	default:
	case RW_SEEK_SET:
		pos = (PHYSFS_uint64)offset; break;
	case RW_SEEK_CUR:
		pos = (PHYSFS_uint64)(PHYSFS_tell(file) + offset); break;
	case RW_SEEK_END:
		pos = (PHYSFS_uint64)(PHYSFS_fileLength(file) + offset); break;
	}
	PHYSFS_seek(file, pos);

	return PHYSFS_tell(file);
}

static size_t RWread(struct SDL_RWops *context, void *ptr, size_t size, size_t maxnum)
{
	PHYSFS_File *file = context->hidden.unknown.data1;
	PHYSFS_sint64 readsize = PHYSFS_readBytes(file, ptr, size * maxnum);
	if( readsize < 0 )
	{
		return 0;
	}
	else
	{
		return readsize / size;
	}
}

static size_t RWwrite(struct SDL_RWops *context, const void *ptr, size_t size, size_t num)
{
	PHYSFS_File *file = context->hidden.unknown.data1;
	PHYSFS_sint64 writesize = PHYSFS_writeBytes(file, ptr, size * num);
	if ( writesize < 0 )
	{
		return 0;
	}
	else
	{
		return writesize / size;
	}
}

static int RWclose( struct SDL_RWops *context )
{
	PHYSFS_File *file = context->hidden.unknown.data1;
	int success = PHYSFS_close(file) ? 0 : -1;
	SDL_FreeRW(context);
	return success;
}

SDL_RWops *PHYSFS_RWFromFile( const char *filename, PHYSFS_RWmode mode )
{
	PHYSFS_File *file = NULL;
	switch( mode )
	{
	case PHYSFS_RWMODE_APPEND: file = PHYSFS_openAppend(filename); break;
	case PHYSFS_RWMODE_READ: file = PHYSFS_openRead(filename); break;
	case PHYSFS_RWMODE_WRITE: file = PHYSFS_openWrite(filename); break;
	}
	if( !file )
	{
		return NULL;
	}

	SDL_RWops *io = SDL_AllocRW();
	if ( !io )
	{
		PHYSFS_close(file);
		return NULL;
	}

	io->size = RWsize;
	io->seek = RWseek;
	io->read = RWread;
	io->write = RWwrite;
	io->close = RWclose;
	io->type = SDL_RWOPS_UNKNOWN;
	io->hidden.unknown.data1 = file;
	return io;
}