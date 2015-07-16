// Utility.Debug.cpp
//////////////////////////////////////////////////////////////////////////

#include "Utility.Debug.h"


void CDebug::ErrorLog(const char* a_errorMsg) {
	#ifdef DEBUG
		fprintf(stderr, a_errorMsg);
	#endif
}

void CDebug::MsgLog(const char* a_msg) {
	#ifdef DEBUG
		fprintf(stdout, a_msg);
	#endif
}