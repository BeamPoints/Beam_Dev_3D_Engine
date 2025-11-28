#pragma once
#include "../../Include/Base/Input.h"
#include <map>

// Korrigierter Konstruktor
CInput::CInput() : mKeyStates(), mMouseStates() 
{
}

bool CInput::reset()
{
	for (auto &[code, state] : mKeyStates)
	{
		state = KeyState::Released;
	}

	return true;
}

LPPOINT CInput::MouseMoved(LPPOINT WindowCenter, LPPOINT NewMousePos)
{
	if(WindowCenter && NewMousePos)
	{
		LPPOINT deltaPos = new POINT();
		deltaPos->x = NewMousePos->x - WindowCenter->x;
		deltaPos->y = NewMousePos->y - WindowCenter->y;
		return deltaPos;
	}

	return nullptr;
}

void CInput::setPressed(KeyCode const &aKeyCode,bool aPressed,bool aIsAlt)
{
	if (aPressed)
	{
		mKeyStates[aKeyCode] = KeyState::Pressed;
		if (aIsAlt)
		{
			mKeyStates[aKeyCode] = KeyState::StrgPressed;
		}
	}
	else
	{
		mKeyStates[aKeyCode] = KeyState::Released;
	}
}

void CInput::setPressed(uint8_t const &aKeyCode,bool aPressed,bool aIsAlt)
{
	setPressed(MapWinApiVKeyToKeyCode(aKeyCode), aPressed, aIsAlt);
}

void CInput::setMPressed(MouseEvents const& aMouseEvent, bool aPressed)
{
	if (aPressed)
	{
		mMouseStates[aMouseEvent] = aMouseEvent;
	}
	else
	{
		mMouseStates[aMouseEvent] = MouseEvents::MouseIsMoving;
	}
}

static uint8_t stateValue(KeyState const &aState)
{
	return static_cast<std::underlying_type_t<KeyState>>(aState);
}

bool CInput::getPressed(KeyCode const &aKeyCode) const
{
	if (mKeyStates.end() == mKeyStates.find(aKeyCode))
	{
		return false;
	}
	return (stateValue(mKeyStates.at(aKeyCode)) & stateValue(KeyState::Pressed));
}

bool CInput::getMPressed(MouseEvents const& aMouseEvent) const
{
	if(mMouseStates.end() == mMouseStates.find(aMouseEvent))
	{
		return false;
	}
	return (mMouseStates.at(aMouseEvent) == aMouseEvent);
}

bool CInput::getPressedWithCtrl(KeyCode const &aKeyCode) const
{
	if (mKeyStates.end() == mKeyStates.find(aKeyCode))
	{
		return false;
	}
	int value = stateValue(mKeyStates.at(aKeyCode));
	int press = stateValue(KeyState::Pressed);
	int mod = stateValue(KeyState::Strg);
	int modPress = press | mod;
	return ((modPress & value) == modPress);
}

bool CInput::getPressedWithShift(KeyCode const &aKeyCode) const
{
	if (mKeyStates.end() == mKeyStates.find(aKeyCode))
	{
		return false;
	}
	return (stateValue(mKeyStates.at(aKeyCode)) & stateValue(KeyState::Pressed)) && (stateValue(mKeyStates.at(aKeyCode)) & stateValue(KeyState::Shift));
}

