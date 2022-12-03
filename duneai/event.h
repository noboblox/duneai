/*
 * event.h
 *
 *  Created on: 03.12.2022
 *      Author: Rene
 */

#ifndef EVENT_H_
#define EVENT_H_

class Event
{
public:
	enum EventType
	{
		ET_ACTION,
		ET_SYSTEM,
	};

	virtual ~Event() {}

	bool isAction() const noexcept { return mEventType == ET_ACTION; }
	bool isSystemEvent() const noexcept { return mEventType == ET_SYSTEM; }

	template <typename T>
	T& get() noexcept { return static_cast<T&> (*this); }

protected:
	explicit Event(EventType type) : mEventType(type) {}

private:
	EventType mEventType;
};

#endif /* EVENT_H_ */
