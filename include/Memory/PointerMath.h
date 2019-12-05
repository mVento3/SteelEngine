#pragma once

#include "Core/Type.h"

namespace SteelEngine { namespace Memory { namespace PointerMath {

    inline Type::uint8 calculateForwardAdjustment(const Type::uptr& p, const Type::uint8& alignment)
    {
		Type::uint8 adjustment = alignment - (p & (Type::uptr)(alignment - 1));

		if(adjustment == alignment)
		{
			return 0;
		}

		return adjustment;
    }

	inline void* calculateForwardAddress(const Type::uptr& p, const Type::uint8& alignment)
    {
		Type::uint8 adjustment = alignment - (p & (Type::uptr)(alignment - 1));

		if(adjustment == alignment)
		{
			return (void*)p;
		}

		return (void*)(p + adjustment);
    }

	inline Type::uint8 calculateBackwardAdjustment(const Type::uptr& p, const Type::uint8& alignment)
	{
		Type::uint8 adjustment = p & (Type::uptr)(alignment - 1);

		if(adjustment == alignment)
		{
			return 0;
		}

		return adjustment;
	}

	template <typename A>
	inline Type::uint8 calculatePaddingWithHeader(const Type::uptr& p, Type::uint8 alignment)
	{
		if(__alignof(A) > alignment)
		{
			alignment = __alignof(A);
		}

		Type::uint8 adjustment = sizeof(A) + calculateForwardAdjustment(p + sizeof(A), alignment);

		return adjustment;

		// Type::uptr padding = calculatePadding(p, alignment);
		// size_t neededSpace = headerSize;

		// if(padding < neededSpace)
		// {
		// 	neededSpace -= padding;

		// 	if(neededSpace % alignment > 0)
		// 	{
		//         padding += alignment * (1 + (neededSpace / alignment));
        // 	}
		// 	else
		// 	{
		//         padding += alignment * (neededSpace / alignment);
        // 	}
		// }

		// return padding;
	}

    inline void* add(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<Type::uptr>(p) + x);
	}

	inline const void* add(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<Type::uptr>(p) + x);
	}

	inline void* subtract(void* p, size_t x)
	{
		return (void*)(reinterpret_cast<Type::uptr>(p) - x);
	}

	inline const void* subtract(const void* p, size_t x)
	{
		return (const void*)(reinterpret_cast<Type::uptr>(p) - x);
	}

}}}