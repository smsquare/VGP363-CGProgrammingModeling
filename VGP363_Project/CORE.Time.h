// CORE.Time.h
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _CORE_TIME_H
#define _CORE_TIME_H

namespace CORE {
	namespace TIME {
		extern unsigned long GlobalTime;
		extern unsigned long PreviousTime;
		extern unsigned long DeltaTicks;
		extern float DeltaTime;

		void Update();
	}
}

#endif