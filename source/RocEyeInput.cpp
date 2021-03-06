#include "RocEyeInput.h"

RocEyeInput::RocEyeInput(int width, int height) : BaseAppInput(),
	mWindowWidth(width),
	mWindowHeight(height),
	mHandler(NULL),
	mWasKeyDownC(false),
	mWasKeyDownG(false),
	mWasKeyDownH(false),
	mWasKeyDownL(false),
	mWasKeyDownO(false),
	mWasKeyDownT(false),
	mWasKeyDownENTER(false)
{
}

RocEyeInput::~RocEyeInput(void)
{
}

void RocEyeInput::setHandler(InputHandler* handler)
{
	mHandler = handler;
}

KeyArray* RocEyeInput::handleKeyboard(KeyArray* keys)
{
	if (!BaseAppInput::handleKeyboard(keys))
	{
		return NULL;
	}
	
	
	if (!keys)
	{
		keys = SDL_GetKeyState(NULL);
	}
	
	
	if (mHandler->isConsoleActive())
	{
		return keys;
	}
	
	
	if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
	{
		mHandler->changeSpeed(1);
	}
	if (keys[SDLK_LCTRL] || keys[SDLK_RCTRL])
	{
		mHandler->changeSpeed(-1);
	}
	
	if (keys[SDLK_w])
	{
		mHandler->translate(InputHandler::DIR_FORWARD);
	}
	if (keys[SDLK_s])
	{
		mHandler->translate(InputHandler::DIR_BACKWARD);
	}
	if (keys[SDLK_a])
	{
		mHandler->translate(InputHandler::DIR_LEFT);
	}
	if (keys[SDLK_d])
	{
		mHandler->translate(InputHandler::DIR_RIGHT);
	}
	if (keys[SDLK_r])
	{
		mHandler->translate(InputHandler::DIR_UP);
	}
	if (keys[SDLK_f])
	{
		mHandler->translate(InputHandler::DIR_DOWN);
	}
	
	if (keys[SDLK_e])
	{
		mHandler->rotate(InputHandler::ROT_CW, false);
	}
	if (keys[SDLK_q])
	{
		mHandler->rotate(InputHandler::ROT_CCW, false);
	}
	
	if (keys[SDLK_DOWN])
	{
		mHandler->rotate(InputHandler::ROT_DOWN, false);
	}
	if (keys[SDLK_UP])
	{
		mHandler->rotate(InputHandler::ROT_UP, false);
	}
	if (keys[SDLK_RIGHT])
	{
		mHandler->rotate(InputHandler::ROT_RIGHT, false);
	}
	if (keys[SDLK_LEFT])
	{
		mHandler->rotate(InputHandler::ROT_LEFT, false);
	}
	
	
	return keys;
}

bool RocEyeInput::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	bool shift = mod & (KMOD_LSHIFT | KMOD_RSHIFT);
	
	static const SDLKey consoleToggle = SDLK_F1;
	
	if (mHandler->isConsoleActive() && sym != consoleToggle) //MAKE SURE that you catch the consoleToggle here
	{
		return mHandler->sendConsoleMessage(SDL_GetKeyName(sym)); //return so that we don't send spurious signals to the input handler
	}
	
	switch (sym)
	{
		case SDLK_c:
		{
			if (shift)
			{
				mHandler->toggleObjectLock();
			}
			else
			{
				mHandler->centerObject();
			}
			break;
		}
		case SDLK_g:
		{
			mHandler->toggleGridLines(shift);
			break;
		}
		case SDLK_h:
		{
			if (shift)
			{
				mHandler->toggleHorizonLock();
			}
			else
			{
				mHandler->levelHorizon();
			}
			break;
		}
		case SDLK_l:
		{
			mHandler->toggleSnapToGrid();
			break;
		}
		case SDLK_o:
		{
			mHandler->toggleObjectMode();
			break;
		}
		case SDLK_t:
		{
			mHandler->toggleMovementTarget();
			break;
		}
		case SDLK_RETURN:
		{
			mHandler->onPrimaryPointerDown();
			break;
		}
		case SDLK_SPACE:
		{
			mHandler->resetCamera();
			break;
		}
		case SDLK_SLASH:
		{
			mHandler->activateConsole();
			mHandler->sendConsoleMessage(SDL_GetKeyName(sym)); //be sure to inject the keypress to the console, as well
			break;
		}
		case consoleToggle:
		{
			mHandler->toggleConsole();
			break;
		}
		default:
		{
			//un-implemented key if we get here
			break;
		}
	}
	
	return true;
}

bool RocEyeInput::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	switch (sym)
	{
		case SDLK_RETURN:
		{
			mHandler->onPrimaryPointerUp();
			break;
		}
		default:
		{
			//un-implemented key
			break;
		}
	}
	
	return true;
}

bool RocEyeInput::handleMouse(void)
{
	if (! BaseAppInput::handleMouse() )
	{
		return false;
	}
	
	const int CENTER_X = mWindowWidth/2;
	const int CENTER_Y = mWindowHeight/2;
	
	int x, y;
	SDL_GetMouseState(&x,&y);
	
	//deltas
	int dx = x - CENTER_X;
	int dy = y - CENTER_Y;
	
	
	mHandler->rotate(InputHandler::ROT_RIGHT, true, dx);
    mHandler->rotate(InputHandler::ROT_UP, true, -dy);	
    
	
	//ideally, we would tell SDL to use relative mouse mode directly. Unfortunately, that seems not to be present in 1.2.15? At least, the Linux header I have doesn't seem to have SDL_SetRelativeMouseMode.
	if (dx || dy)
	{
		SDL_WarpMouse(CENTER_X, CENTER_Y);
	}
	return true;
}

bool RocEyeInput::fireLeftMouseDown(void)
{
	return mHandler->onPrimaryPointerDown();	
}

bool RocEyeInput::fireLeftMouseUp(void)
{
	return mHandler->onPrimaryPointerUp();
}

void RocEyeInput::frameDone(void)
{
	BaseAppInput::frameDone();
	
	mHandler->execute(); //need to tell our input handler to actually run what we told it to do
}
