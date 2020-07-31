#pragma once

// Big thanks to John Jackson: https://github.com/MrFrenik

#include "PlatformDll/types.h"
#include "PlatformDll/utils.h"

namespace SteelEngine {

/*===================================
// Dynamic Array
===================================*/

/*
	- HEAVILY lifted from Sean Barret's 'stretchy buffer' implementation

	TODO(john): illustrate basic usage example
*/

typedef struct
{
	s32 size;
	s32 capacity;
} se_dyn_array;

#define se_dyn_array_head( arr )\
	( ( se_dyn_array* )( ( u8* )arr - sizeof( se_dyn_array ) ) )

#define se_dyn_array_size( arr )\
	se_dyn_array_head( arr )->size

#define se_dyn_array_capacity( arr )\
	se_dyn_array_head( arr )->capacity

#define se_se_dyn_array_full( arr )\
	( ( se_dyn_array_size( arr ) == se_dyn_array_capacity( arr ) ) )	

_inline void* se_dyn_array_resize_impl(void* arr, usize sz, usize amount)
{
	usize capacity;

	if (arr) {
		capacity = amount;
	}
	else {
		capacity = 0;
	}

	// Create new dyn_array with just the header information
	se_dyn_array* data = (se_dyn_array*)se_realloc(arr ? se_dyn_array_head(arr) : 0, capacity * sz + sizeof(se_dyn_array));
	if (data)
	{
		// Set size
		if (!arr)
		{
			data->size = 0;
		}

		data->capacity = capacity;

		// Return actual data position in buffer
		return ((s32*)data + 2);
	}

	return NULL;
}

#define se_dyn_array_need_grow(arr, n)\
	((arr) == 0 || se_dyn_array_size(arr) + n >= se_dyn_array_capacity(arr))

#define se_dyn_array_grow( arr )\
	se_dyn_array_resize_impl( arr, sizeof( *( arr ) ), se_dyn_array_capacity( arr ) ? se_dyn_array_capacity( arr ) * 2 : 1 )

#define se_dyn_array_push( arr, val )\
	do {\
		if ( !( arr ) || ( ( arr ) && se_dyn_array_need_grow(arr, 1) ) ) {\
			*( ( void ** )&( arr ) ) = se_dyn_array_grow( arr ); \
		}\
		( arr )[ se_dyn_array_size( arr ) ] = ( val );\
		se_dyn_array_size( arr ) += 1;\
	} while( 0 )

#define se_dyn_array_reserve( arr, amount )\
	do {\
		if ( ( !arr ) || amount > se_dyn_array_capacity( arr ) ) {\
			*( ( void ** )&( arr ) ) = se_dyn_array_resize_impl( arr, sizeof( *arr ), amount );\
		}\
	} while( 0 )

#define se_dyn_array_empty( arr )\
	( arr && ( se_dyn_array_size( arr ) == 0 ) )

#define se_dyn_array_pop( arr )\
	do {\
		if ( arr && !se_dyn_array_empty( arr ) ) {\
			se_dyn_array_size( arr ) -= 1;\
		}\
	} while ( 0 )

#define se_dyn_array_back( arr )\
	( arr + ( se_dyn_array_size( arr ) ? se_dyn_array_size( arr ) - 1 : 0 ) )

#define se_dyn_array_for( arr, type, iter_name )\
	for ( type* iter_name = arr; iter_name != se_dyn_array_back( arr ); ++iter_name )

#define se_dyn_array_new( type )\
	( type* )se_dyn_array_resize_impl( NULL, sizeof( type ), 0 )

#define se_dyn_array_clear( arr )\
	se_dyn_array_size( arr ) = 0

#define se_dyn_array( type )	type*

#define se_dyn_array_free( arr )\
	do {\
		if ( arr ) {\
			free( se_dyn_array_head( arr ) );\
			arr = 0;\
		}\
	} while ( 0 )

/*===================================
// Slot Array
===================================*/

#define se_slot_array_invalid_handle 	u32_max

typedef struct se_slot_array_base
{
	se_dyn_array(u32)	handle_indices;
	se_dyn_array(u32)	reverindirection_indices;
	se_dyn_array(u32)	index_free_list;
} se_slot_array_base;

#define se_slot_array_decl( T )\
	typedef struct se_sa_##T\
	{\
		se_slot_array_base _base;\
		se_dyn_array( T ) data;\
		u32 ( * insert_func )( struct se_sa_##T*, T );\
	} se_sa_##T;\
\
	_force_inline u32\
	se_sa_##T##_insert_func( struct se_sa_##T* s, T v )\
	{\
		u32 free_idx = se_slot_array_find_next_available_index( ( se_slot_array_base* )s );\
		se_dyn_array_push( s->data, v );\
		se_dyn_array_push( s->_base.reverindirection_indices, free_idx );\
		s->_base.handle_indices[ free_idx ] = se_dyn_array_size( s->data ) - 1;\
		return free_idx;\
	}\
\
	_inline\
	se_sa_##T __sa_##T##_new()\
	{\
		se_sa_##T sa; \
			sa.data 								= se_dyn_array_new( T );\
			sa._base.handle_indices 				= se_dyn_array_new( u32 );\
			sa._base.reverindirection_indices 	= se_dyn_array_new( u32 );\
			sa._base.index_free_list 				= se_dyn_array_new( u32 );\
			sa.insert_func 							= &se_sa_##T##_insert_func;\
\
		return sa;\
	}

	_force_inline u32
		se_slot_array_find_next_available_index(se_slot_array_base* sa)
	{
		if (se_dyn_array_empty(sa->index_free_list)) {
			se_dyn_array_push(sa->handle_indices, 0);
			return (se_dyn_array_size(sa->handle_indices) - 1);
		}
		u32 idx = sa->index_free_list[0];
		if (se_dyn_array_size(sa->index_free_list) > 1) {
			u32 sz = se_dyn_array_size(sa->index_free_list);
			u32 tmp = sa->index_free_list[sz - 1];
			sa->index_free_list[0] = tmp;
			se_dyn_array_pop(sa->index_free_list);
		}
		else {
			se_dyn_array_clear(sa->index_free_list);
		}

		return idx;
	}

#define se_slot_array_clear( s )\
	do {\
		se_dyn_array_clear( s._base.handle_indices );\
		se_dyn_array_clear( s._base.index_free_list );\
		se_dyn_array_clear( s._base.reverindirection_indices );\
		se_dyn_array_clear( s.data );\
	} while ( 0 )

#define se_slot_array_size( s )\
	se_dyn_array_size( (s).data )

#define se_slot_array_new( type )\
	__sa_##type##_new()

#define se_slot_array_insert( s, v )\
	s.insert_func( &( s ), ( v ) )

#define se_slot_array_get( s, handle )\
	( s.data[ s._base.handle_indices[ handle ] ] )

#define se_slot_array_get_unsafe( s, handle )\
	( &s.data[ s._base.handle_indices[ handle ] ] )

#define se_slot_array_get_ptr( s, handle )\
	( &s.data[ s._base.handle_indices[ handle ] ] )

#define se_slot_array_handle_valid( s, handle )\
	( handle < se_slot_array_size( s ) )

#define se_slot_array( T )\
	se_sa_##T

}