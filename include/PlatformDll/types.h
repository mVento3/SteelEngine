#pragma once

// Big thanks to John Jackson: https://github.com/MrFrenik

#include <stdlib.h>		// malloc, realloc, free ( for now )
#include <stdint.h>		// standard types
#include <limits.h>		// INT32_MAX, UINT32_MAX
#include <string.h> 	// memset
#include <float.h>		// FLT_MAX

#include "RuntimeReflection/Macro.h"

SE_DONT_PARSE

#define _inline 			static inline
#define _local_persist 		static
#define _global 			static

#if ( defined _WIN32 || defined _WIN64 )
#define _force_inline 		static __forceinline
#elif ( defined __APPLE__ || defined _APPLE )
#define _force_inline 		static __attribute__((always_inline))
#else
#define _force_inline 		_inline
#endif

/*============================================================
// Resource Declarations
============================================================*/

#define se_resource( type )\
	se_resource_##type

// Strongly typed declarations for resource handles (slot array handles)
#define se_declare_resource_type( type )\
	typedef struct se_resource( type ) {\
		u32 id;\
	} se_resource( type );\

/*============================================================
// Primitives
============================================================*/

#ifndef __cplusplus
#define false 		0
#define true 		1
#endif

typedef size_t		usize;

#ifdef __cplusplus
typedef bool 		b8;
#else
typedef _Bool 		b8;
#endif

typedef uint8_t 		u8;
typedef int8_t 			s8;
typedef uint16_t 		u16;
typedef int16_t 		s16;
typedef uint32_t 		u32;
typedef int32_t			s32;
typedef s32 			b32;
typedef uint64_t		u64;
typedef int64_t			s64;
typedef float 	 		f32;
typedef double			f64;
typedef const char* const_str;

#define u16_max 		UINT16_MAX
#define u32_max			UINT32_MAX
#define s32_max			INT32_MAX
#define f32_max 		FLT_MAX
#define f32_min 		FLT_MIN