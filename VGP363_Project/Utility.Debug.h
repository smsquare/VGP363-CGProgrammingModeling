// Utility.Debug.h
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdio.h>

#define DEBUG

class CDebug {
	public:
		static void ErrorLog(const char* a_errorMsg);
		static void MsgLog(const char* a_msg);
};