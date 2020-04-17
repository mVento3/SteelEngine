#pragma once

#include "vector"

namespace SteelEngine {

    template <typename T>
	class Vector
	{
		typedef std::vector<T*> VectorType;

	private:
		const VectorType* m_OriginalVector;

	public:
		typedef typename VectorType::size_type SizeType;

		Vector() :
			m_OriginalVector(0)
		{

		}

		Vector(const VectorType& vecRef) :
			m_OriginalVector(&vecRef)
		{

		}

		const T* operator[](SizeType idx) const
		{
			return m_OriginalVector->at(idx);
		}

		bool Empty() const
		{
			return m_OriginalVector->empty();
		}

		SizeType Size() const
		{
			return m_OriginalVector->size();
		}
	};

    struct IReflectionData;
    struct IReflectionInheritance;

    struct IReflectionDataHelper
    {
        void ProcessInheritance(const std::vector<IReflectionData*>& res, const std::vector<IReflectionInheritance*>& inheritance, void* createdObject, const IReflectionData* data) const;
        bool Subprocess(const IReflectionData* data) const;
    };
        
}