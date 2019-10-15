#pragma once

#include "Event/InternalEventChannel.h"

#include "InformationTracker/ISystemTracker.h"

namespace SteelEngine { namespace Event {

	class GlobalEvent
	{
	private:
		static void* Check(void* object, size_t typeID);

		template <typename tMessage>
		inline static InternalChannel<tMessage>* Get()
		{
			return (InternalChannel<tMessage>*)Check(&InternalChannel<tMessage>::Instance(), typeid(tMessage).hash_code());
		}

		static void TraceInfo(void* object, const std::string& name, size_t typeID, const std::string& name2, size_t eventTypeID);

		static ISystemTracker* m_Tracker;

	public:
		template <typename tMessage, typename tHandler>
		inline static void Add(tHandler* handler)
		{
			InternalChannel<tMessage>* channel = Get<tMessage>();

			channel->Add(handler);

			TraceInfo(
				handler,
				typeid(tHandler).name(),
				typeid(tHandler).hash_code(),
				typeid(tMessage).name(),
				typeid(tMessage).hash_code()
			);
		}

		template <typename tMessage, typename tHandler>
		inline static void Add_(tHandler* handler)
		{
			InternalChannel<tMessage>* channel = Get<tMessage>();

			channel->Add_(handler);
		}

		template <typename tMessage, typename tHandler>
		inline static void Remove(tHandler* handler)
		{
			InternalChannel<tMessage>* channel = Get<tMessage>();

			channel->Remove(handler);
		}

		template <typename tMessage>
		inline static void Broadcast(const tMessage& message)
		{
			InternalChannel<tMessage>* channel = Get<tMessage>();

			channel->Broadcast(message);
		}

		template <typename tMessage>
		inline static void Broadcast_(tMessage* message)
		{
			InternalChannel<tMessage>* channel = Get<tMessage>();

			channel->Broadcast_(message);
		}
	};

}}