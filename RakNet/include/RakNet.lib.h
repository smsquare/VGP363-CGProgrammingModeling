// RakNet.lib.h
// This is a wrapper library to store all of the RakNet source.
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _RAKNET_LIB_H
#define _RAKNET_LIB_H

// Standard Library Includes
#include <stdio.h>
#include <string.h>

// RakNet Library Includes
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" // Message ID


// Global Defines
#define MAX_CLIENTS 4
#define SERVER_PORT 60000

// Global Game Messages for Client and Server communication
enum class GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

namespace NetworkSystem {


	// Checks if instance is server
	bool isAuthority();

	// Initializes Net Framework Components
	void InitializeNetwork();

	// Prompts user if instance is Client or Server
	bool ServerCheck();

	// Prompts user for Server IP
	void GetServerIP(char str[]);

	void RunNetwork();
	void DestroyNetwork();
}// End of namespace NetworkSystem
#endif