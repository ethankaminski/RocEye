#ifndef BASE_APP_INPUT_H
#define BASE_APP_INPUT_H

#ifdef WIN32
//Necessary to tell the mouse events to go to this window
#if (_WIN32_WINNT < 0x0501)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
// Necessary to get the Window Handle of the window
// OGRE created, so SDL can grab its input.
#include "windows.h"
#include "SDL_getenv.h"
#endif

#ifdef WIN32
//the Windows SDL package seems to use a slightly different path
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_keyboard.h>
#else
// SDL initialization:
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_mouse.h>
#endif

typedef Uint8 KeyArray; // to make return values readable

class BaseAppInput
{
public:
	BaseAppInput(void);
	virtual ~BaseAppInput(void);
	virtual void setup(void);
	virtual bool runFrame(void);
protected:
	
	virtual bool processEvent(SDL_Event* evt);
	
	virtual void frameDone(void);
	virtual KeyArray* handleKeyboard(KeyArray*); // Do not free the return value.
	virtual bool handleMouse(void);
	
	virtual bool fireLeftMouseDown(void);
	virtual bool fireLeftMouseUp(void);
	virtual bool fireRightMouseDown(void);
	virtual bool fireRightMouseUp(void);
	virtual bool fireMiddleMouseDown(void);
	virtual bool fireMiddleMouseUp(void);
	
	//the functions below are all pure virtual
	virtual bool onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) { return true; };
	virtual bool onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) { return true; };
	
	
	bool mWasLeftDown;
	bool mWasRightDown;
	bool mWasMiddleDown;
};

#endif
