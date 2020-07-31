#include "Serialization/byte_buffer.h"

#include "PlatformDll/utils.h"

void se_byte_buffer_init(se_byte_buffer* buffer)
{
	buffer->buffer = (u8*)se_malloc(se_byte_buffer_default_capacity);
	buffer->capacity = se_byte_buffer_default_capacity;
	buffer->size = 0;
	buffer->position = 0;
}

se_byte_buffer se_byte_buffer_new()
{
	se_byte_buffer buffer;
	se_byte_buffer_init(&buffer);
	return buffer;
}

void se_byte_buffer_free(se_byte_buffer* buffer)
{
	if (buffer && buffer->buffer) {
		se_free(buffer->buffer);
	}
}

void se_byte_buffer_clear(se_byte_buffer* buffer)
{
	buffer->size = 0;
	buffer->position = 0;
}

void se_byte_buffer_resize(se_byte_buffer* buffer, usize sz)
{
	u8* data = (u8*)se_realloc(buffer->buffer, sz);

	if (data == NULL) {
		return;
	}

	buffer->buffer = data;
	buffer->capacity = sz;
}

void se_byte_buffer_seek_to_beg(se_byte_buffer* buffer)
{
	buffer->position = 0;
}

void se_byte_buffer_seek_to_end(se_byte_buffer* buffer)
{
	buffer->position = buffer->size;
}

void se_byte_buffer_advance_position(se_byte_buffer* buffer, usize sz)
{
	buffer->position += sz;
}

void se_byte_buffer_bulk_write(se_byte_buffer* buffer, const void* src, u32 size)
{
	// Check for necessary resize
	u32 total_write_size = buffer->position + size;
	if (total_write_size >= buffer->capacity)
	{
		usize capacity = buffer->capacity * 2;
		while (capacity <= total_write_size)
		{
			capacity *= 2;
		}

		se_byte_buffer_resize(buffer, capacity);
	}

	// memcpy data
	memcpy((buffer->buffer + buffer->position), src, size);

	buffer->size += size;
	buffer->position += size;
}

void se_byte_buffer_bulk_read(se_byte_buffer* buffer, void* dst, u32 size)
{
	memcpy(dst, (buffer->buffer + buffer->position), size);
	buffer->position += size;
}

// "Generic" read function
#define __se_byte_buffer_read_func( T )\
	__se_byte_buffer_func( T )\
	{\
		se_byte_buffer* _buffer = bb;\
		usize sz = sizeof( T );\
		T val = *( T* )( _buffer->buffer + _buffer->position );\
		_buffer->position += sz;\
		return val;\
	}

__se_byte_buffer_read_func(usize);
__se_byte_buffer_read_func(s8);
__se_byte_buffer_read_func(u8);
__se_byte_buffer_read_func(u16);
__se_byte_buffer_read_func(s16);
__se_byte_buffer_read_func(s32);
__se_byte_buffer_read_func(u32);
__se_byte_buffer_read_func(b32);
__se_byte_buffer_read_func(s64);
__se_byte_buffer_read_func(u64);
__se_byte_buffer_read_func(f32);
__se_byte_buffer_read_func(f64);
__se_byte_buffer_read_func(vec4);
__se_byte_buffer_read_func(vec3);
__se_byte_buffer_read_func(vec2);