#pragma once
#include <map>
#include <Windows.h>
#include <memory>

enum class KeyCode: uint8_t
{
	A = 'A',
	B = 'B',
	C = 'C',
	D = 'D',
	E = 'E',
	F = 'F',
	G = 'G',
	H = 'H',
	I = 'I',
	J = 'J',
	K = 'K',
	L = 'L',
	M = 'M',
	N = 'N',
	O = 'O',
	P = 'P',
	Q = 'Q',
	R = 'R',
	S = 'S',
	T = 'T',
	U = 'U',
	V = 'V',
	W = 'W',
	X = 'X',
	Y = 'Y',
	Z = 'Z',
	D0 = '0',
	D1 = '1',
	D2 = '2',
	D3 = '3',
	D4 = '4',
	D5 = '5',
	D6 = '6',
	D7 = '7',
	D8 = '8',
	D9 = '9',
	DNUM0 = 0x60 + 0,
	DNUM1 = 0x60 + 1,
	DNUM2 = 0x60 + 2,
	DNUM3 = 0x60 + 3,
	DNUM4 = 0x60 + 4,
	DNUM5 = 0x60 + 5,
	DNUM6 = 0x60 + 6,
	DNUM7 = 0x60 + 7,
	DNUM8 = 0x60 + 8,
	DNUM9 = 0x60 + 9,
};

enum class KeyState
{
	Released = 0,
	Pressed = 1,
	Strg = 4,
	StrgPressed = 5,
	Alt = 8,
	AltPressed = 9,
	StrgAltPressed = 13,
	Shift = 16,
	ShiftPressed = 17,
	StrgShiftPressed = 21,
	AltShiftPressed = 25,
	StrgAltShiftPressed = 29,
};

static uint8_t MapKeyCodeToWinApiVKey(KeyCode const &aKeyCode)
{
	return (uint8_t) static_cast<std::underlying_type_t<KeyCode>>(aKeyCode);
}

static KeyCode MapWinApiVKeyToKeyCode(uint8_t const &keyCode)
{
	return (KeyCode) static_cast<std::underlying_type_t<KeyCode>>(keyCode);
}

class CInput
{
public:
	CInput();

	void setPressed(KeyCode const &aKeyCode, bool aPressed, bool aIsAlt = false);

	void setPressed(uint8_t const &aKeyCode, bool aPressed, bool aIsAlt = false);

	bool getPressed(KeyCode const &aKeyCode) const;
	bool getPressedWithCtrl(KeyCode const &aKeyCode) const;
	bool getPressedWithShift(KeyCode const &aKeyCode) const;

	bool reset();

private:

	std::map< KeyCode, KeyState> mKeyStates;
	
};

