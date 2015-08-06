// CORE.Time.cpp
//////////////////////////////////////////////////////////////////////////

#include "main.h"
#include <time.h>

namespace CORE {
	namespace TIME {
		unsigned long GlobalTime = (unsigned long)0.0;
		unsigned long PreviousTime = (unsigned long)0.0;
		unsigned long DeltaTicks = (unsigned long)0.0;
		float DeltaTime = 0.0f;

		void Update() {
			PreviousTime = GlobalTime;
			clock_t t = clock();
			GlobalTime = t;
			DeltaTicks = GlobalTime - PreviousTime;
			DeltaTime = DeltaTicks / 1000.0f;
		}
	}
}