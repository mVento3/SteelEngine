#pragma once

#include "Event/internaleventchannel.h"

namespace SteelEngine
{
	namespace Event
	{
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

		public:
			template <typename tMessage, typename tHandler>
			inline static void Add(tHandler* handler)
			{
				InternalChannel<tMessage>* channel = Get<tMessage>();
			
				channel->Add(handler);
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
		};
	}
}