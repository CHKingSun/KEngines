//
// Created by KingSun on 2018/11/05
//

#ifndef KENGINES_EVENT_CALLBACK_H
#define KENGINES_EVENT_CALLBACK_H

#include "../KHeader.h"

namespace KEngines { namespace KObject {
	class EventCallback {
	public:
		virtual ~EventCallback() = default;

		virtual void keyEvent(Kint key, Kint action) = 0;
		virtual void mouseEvent(Kint button, Kint action) = 0;
		virtual void mouseWheelEvent(Kdouble yoffset) = 0;
		virtual void cursorEvent(Kdouble xpos, Kdouble ypos) = 0;
	};
} }

#endif // !KENGINES_EVENT_CALLBACK_H

