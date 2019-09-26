#pragma once

#include "Event/InternalEventChannel.h"

#include "InformationTracker/ISystemTracker.h"

namespace SteelEngine { namespace Event {

	class GlobalEvent
	{
	private:
		template <typename tMessage>
		inline static InternalChannel<tMessage>* Get()
		{
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
			size_t typeID = typeid(tMessage).hash_code();

			if (!db->m_EventGlobalHandlers[typeID])
			{
				db->m_EventGlobalHandlers[typeID] = &InternalChannel<tMessage>::Instance();
			}

			return (InternalChannel<tMessage>*)db->m_EventGlobalHandlers[typeID];
		}

		template <typename tMessage>
		inline static InternalChannel<tMessage>* Get_()
		{
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");
			size_t typeID = typeid(tMessage*).hash_code();

			if (!db->m_EventGlobalHandlers[typeID])
			{
				db->m_EventGlobalHandlers[typeID] = &InternalChannel<tMessage>::Instance();
			}

			return (InternalChannel<tMessage>*)db->m_EventGlobalHandlers[typeID];
		}

		static void Test(void* object, const std::string& name, size_t typeID, const std::string& name2, size_t eventTypeID);

		static ISystemTracker* m_Tracker;

	public:
		template <typename tMessage, typename tHandler>
		inline static void Add(tHandler* handler)
		{
			InternalChannel<tMessage>* channel = Get<tMessage>();

			channel->Add(handler);

			Test(
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
			InternalChannel<tMessage>* channel = Get_<tMessage>();

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
			InternalChannel<tMessage>* channel = Get_<tMessage>();

			channel->Broadcast_(message);
		}
	};

}}