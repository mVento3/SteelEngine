#pragma once

#include "Utils/ITuple.h"

#include "tuple"

namespace SteelEngine {

    template <typename... Args>
	struct Tuple : public ITuple
	{
		Tuple(const std::tuple<Args...>& args) :
			m_Args(args)
		{

		}

		Tuple(Args... args)
		{
			m_Args = std::make_tuple(args...);
		}

		~Tuple()
		{

		}

		std::tuple<Args...> m_Args;

		ITuple* GetTuple() override
		{
			return 0;
		}
	};

}