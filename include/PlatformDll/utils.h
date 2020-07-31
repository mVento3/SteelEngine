#pragma once

// Big thanks to John Jackson: https://github.com/MrFrenik

#include "PlatformDll/types.h"

#include <stdarg.h>
#include <stdio.h>

	// Helper macro for compiling to nothing
#define se_empty_instruction( ... )

#define se_array_size( arr ) sizeof( arr ) / sizeof( arr[ 0 ] )

#define assert( x ) if ( !( x ) ) {\
	printf( "assertion failed: ( %s ), function %s, file %s, line %d.\n", #x, __func__, __FILE__, __LINE__ );\
	abort();\
}

// Helper macro for an in place for-range loop
#define se_for_range_i( count )\
	for ( u32 i = 0; i < count; ++i )

// Helper macro for an in place for-range loop
#define se_for_range_j( count )\
	for ( u32 j = 0; j < count; ++j )

#define se_for_range( iter_type, iter_name, iter_end )\
	for( iter_type iter_name = 0; iter_name < iter_end; ++iter_name )

#define se_max( a, b ) ( (a) > (b) ? (a) : (b) )

#define se_min( a, b ) ( (a) < (b) ? (a) : (b) )

#define se_clamp( v, min, max ) ( (v) > (max) ? (max) : (v) < (min) ? (min) : (v) )

// Helpful macro for casting one type to another
#define se_cast( a, b ) ( ( a* )(b) )

// Helpful marco for calculating offset ( in bytes ) of an element from a given structure type
#define se_offset( type, element ) ( ( usize )( &( ( ( type* )( 0 ) )->element ) ) )

// macro for turning any given type into a const char* of itself
#define se_to_str( type ) ((const char*)#type)

#define se_print_enabled	0

#if se_print_enabled
#define se_measure_time( label, ... )\
		do {\
			u32 __st = platform_ticks();\
			__VA_AR_\
			println( "%s: %d", label, platform_ticks() - __st );\
		} while ( 0 )
#else
#define se_measure_time( label, ... )\
		__VA_AR_
#endif

#define se_timed_action( interval, ... )\
	do {\
		static u32 __t = 0;\
		if ( __t++ > interval ) {\
			__t = 0;\
			__VA_AR_\
		}\
	} while ( 0 )		

	_force_inline void*
		_se_malloc_init_impl(usize sz)
	{
		void* data = malloc(sz);
		memset(data, 0, sz);
		return data;
	}

	// Helper functions for heap allocations
#define se_malloc( sz ) 			malloc( sz )
#define se_free( mem )				free( mem )
#define se_realloc( mem, sz )		realloc( mem, sz )
#define se_malloc_init( type )	( type* )_se_malloc_init_impl( sizeof( type ) )