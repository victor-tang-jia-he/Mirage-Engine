#pragma once
#include <vector>
#include <functional>
#include <map>
#include <string>

namespace MirageEngine
{
	namespace Core
	{
		namespace EventSystem
		{
			class IEvent
			{
			public:
				virtual const std::string& GetName() const = 0;
			};
			template <typename ..._args>
			class Event : public IEvent
			{
			public:
				//Alias for template callback
				using _callback = std::function<void(_args...)>;

				//Ctor & Dtor
				//template< typename T_CB >
				explicit Event(const std::string& name, const _callback& cb) : _name(name), _cbFunc(cb) {}
				~Event() {}

				//Accessors
				virtual const std::string& GetName() const override { return this->_name; }

				//Methods
				void trigger(_args... a) { this->_cbFunc(a...); }
			private:
				//EventSystem identifier
				std::string _name;
				//EventSystem callback - can't be changed inside event.
				_callback const _cbFunc;
			};
			class EventDispatcher
			{
			public:
				EventDispatcher() {}
				~EventDispatcher()
				{
					for (auto& el : _eventList)
					{
						for (auto* e : el.second) 
							delete e;
					}
				}

				void Add(IEvent* event)
				{
					if (event)
						_eventList[event->GetName()].push_back(event);
				}

				template <typename ..._args>
				void Invoke(const std::string& eventName, _args...a)
				{
					auto it_eventList = _eventList.find(eventName);
					if (it_eventList == _eventList.end())
						return;
					for (auto ie : it_eventList->second)
					{
						if (Event<_args...>* event = dynamic_cast<Event<_args...>*>(ie))
							event->trigger(a...);
					}
				}

			private:
				std::map<std::string, std::vector<IEvent*>> _eventList;
			};
		}
	}
}