#pragma once

#include "Utils/Tuple.h"

namespace SteelEngine {

    struct TupleMaker : public ITuple
	{
	private:
		ITuple* m_Tuple;

	public:
		template <typename... Args>
		TupleMaker(Args... args)
		{
			m_Tuple = new Tuple<Args...>(args...);
		}

		~TupleMaker()
		{

		}

		ITuple* GetTuple() override
		{
			return m_Tuple;
		}
	};

}