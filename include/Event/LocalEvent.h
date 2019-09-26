#pragma once

#include "Event/InternalEventChannel.h"

namespace SteelEngine { namespace Event {

	template <typename tMessage>
	class LocalEvent
	{
	private:
		InternalChannel<tMessage> m_Event;

	public:
		template <typename tHandler>
		inline void Add(tHandler* handler)
		{
			m_Event.Add(handler);
		}

		template <typename tHandler>
		inline void Add_(tHandler* handler)
		{
			m_Event.Add_(handler);
		}

		template <typename tHandler>
		inline void Remove(tHandler* handler)
		{
			m_Event.Remove(handler);
		}

		inline void Broadcast(const tMessage& message)
		{
			m_Event.Broadcast(message);
		}

		template <typename ...Args>
		inline void BroadcastVaradic(Args... args)
		{
			m_Event.Broadcast(tMessage(args...));
		}

		inline void Broadcast_(tMessage* message)
		{
			m_Event.Broadcast_(message);
		}
	};

}}