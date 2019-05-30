#pragma once

#include "Event/InternalEventChannel.h"

// Define the local event handler
#define H_LEVENT(type) void operator()(const type& event);

// Define and declare the local event handler
#define H_LEVENT_D(type, ...) void operator()(const type& event) \
{																\
__VA_ARGS__														\
}

// Define the local event
#define LEVENT(structName, ...)				\
struct structName							\
{											\
	__VA_ARGS__								\
};											\
SteelEngine::Event::LocalEvent<structName> m_ ## structName;

// Broadcast the local event in local space
#define B_LEVENT_L(structName, ...) m_ ## structName.Broadcast(structName{ __VA_ARGS__ });

namespace SteelEngine
{
	namespace Event
	{
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

			inline void Broadcast_(tMessage* message)
			{
				m_Event.Broadcast_(message);
			}
		};
	}
}