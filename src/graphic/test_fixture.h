#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <SDL.h>

namespace graphic
{

struct TestFixture {
	TestFixture() {
		SDL_Init(0);
		SDL_SetVideoMode(400, 300, 0, 0);
	}

	~TestFixture() {
		SDL_Quit();
	}

	static void process_events() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		}
	}
};

}
