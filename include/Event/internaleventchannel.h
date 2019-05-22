#pragma once

#include <vector>
#include <algorithm>
#include <mutex>
#include <iostream>

#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "ModuleManager/ModuleManager.h"

namespace SteelEngine { namespace Event {

	template <typename tMessage>
	class InternalChannel
	{
	public:
		typedef std::function<void(const tMessage&)> Handler;
		typedef std::function<void(tMessage*)> Handler_;

	private:
		std::mutex				m_Mutex;
		std::vector<Handler>	m_Handlers;
		std::vector<Handler_>	m_Handlers_;
		std::vector<void*>		m_OriginalPtrs;

	public:
		inline static InternalChannel& Instance()
		{
			static InternalChannel result;

			return result;
		}

		template <typename tHandler>
		inline void Add(tHandler* handler)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			m_Handlers.push_back([handler](const tMessage& msg) { (*handler)(msg); });
			m_OriginalPtrs.push_back(handler);
		}

		template <typename tHandler>
		inline void Add_(tHandler* handler)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			m_Handlers_.push_back([handler](tMessage* msg) { (*handler)(msg); });
			m_OriginalPtrs.push_back(handler);
		}

		template <typename tHandler>
		inline void Remove(tHandler* handler)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			auto it = std::find(m_OriginalPtrs.begin(), m_OriginalPtrs.end(), handler);

			if (it == m_OriginalPtrs.end())
				//throw std::runtime_error("Tried to remove a handler that was not in the handler list");
			//gLogError << "Tried to remove a handler that was not in the handler list" << std::endl;
			{

			}
			else
			{
				auto idx = it - m_OriginalPtrs.begin();

				m_Handlers.erase(m_Handlers.begin() + idx);
				m_OriginalPtrs.erase(it);
			}
		}

		inline void Broadcast(const tMessage& msg)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			for (auto& handler : m_Handlers)
				handler(msg);
		}

		inline void Broadcast_(tMessage* msg)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			for (auto& handler : m_Handlers_)
				handler(msg);
		}
	};

}}