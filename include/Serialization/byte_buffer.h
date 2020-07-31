#pragma once

// Big thanks to John Jackson: https://github.com/MrFrenik

#include "PlatformDll/types.h"

#include "glm/glm.hpp"

#ifdef EXPORT_DLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#define se_byte_buffer_default_capacity 	1024

/* Byte buffer */
typedef struct se_byte_buffer
{
	u8* buffer;					// Buffer that actually holds all relevant byte data
	u32 size;					// Current size of the stored buffer data
	u32 position;				// Current read/write position in the buffer
	u32 capacity;				// Current max capacity for the buffer
} se_byte_buffer;

	// Generic "read" function for a byte buffer
#define se_byte_buffer_read( bb, T )\
	__se_byte_buffer_read_##T( bb )

// Generic "write" function for a byte buffer
#define se_byte_buffer_write( bb, T, val )\
do {\
	se_byte_buffer* _buffer = bb;\
	usize sz = sizeof( T );\
	usize total_write_size = _buffer->position + sz;\
	if ( total_write_size >= _buffer->capacity )\
	{\
		usize capacity = _buffer->capacity * 2;\
		while( capacity < total_write_size )\
		{\
			capacity *= 2;\
		}\
		se_byte_buffer_resize( _buffer, capacity );\
	}\
	*( T* )( _buffer->buffer + _buffer->position ) = val;\
	_buffer->position += sz;\
	_buffer->size += sz;\
} while( 0 )

void se_byte_buffer_init(se_byte_buffer* buffer);
se_byte_buffer se_byte_buffer_new();
void se_byte_buffer_free(se_byte_buffer* buffer);
void se_byte_buffer_clear(se_byte_buffer* buffer);
void se_byte_buffer_resize(se_byte_buffer* buffer, usize sz);
void se_byte_buffer_seek_to_beg(se_byte_buffer* buffer);
void se_byte_buffer_seek_to_end(se_byte_buffer* buffer);
void se_byte_buffer_advance_position(se_byte_buffer* buffer, usize sz);
void se_byte_buffer_bulk_write(se_byte_buffer* buffer, const void* src, u32 sz);
void se_byte_buffer_bulk_read(se_byte_buffer* buffer, void* dst, u32 sz);

#define __se_byte_buffer_func(T)\
	T __se_byte_buffer_read_##T( se_byte_buffer* bb )

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

__se_byte_buffer_func(usize);
__se_byte_buffer_func(s8);
__se_byte_buffer_func(u8);
__se_byte_buffer_func(s16);
__se_byte_buffer_func(u16);
__se_byte_buffer_func(s32);
__se_byte_buffer_func(u32);
__se_byte_buffer_func(b32);
__se_byte_buffer_func(s64);
__se_byte_buffer_func(u64);
__se_byte_buffer_func(f32);
__se_byte_buffer_func(f64);
__se_byte_buffer_func(vec2);
__se_byte_buffer_func(vec3);
__se_byte_buffer_func(vec4);