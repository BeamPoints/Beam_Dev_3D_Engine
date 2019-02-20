#pragma once
#include <string>
#include <stdint.h>
#include <Windows.h>
#include <sstream>

struct DisplaySettings
{
	int32_t width;
	int32_t height;
};

class CWindow
{
public:

	inline HINSTANCE			const &instanceHandle()		const { return mInstance; }
	inline DisplaySettings		const &display()			const { return mDisplay; }
	inline HWND					const &WindowHandle()	const { return mWindowHandle; }

	static std::shared_ptr<CWindow> Create(HINSTANCE aInstance);

	static bool CreateAppWindow(std::shared_ptr<CWindow> Window);

protected:
	CWindow(HINSTANCE aInstance, DisplaySettings const &aDisplay);

private:

	 HINSTANCE		 mInstance;
	 HWND			 mWindowHandle;
	 DisplaySettings mDisplay;
};
