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
	inline HINSTANCE			const& instanceHandle()		const { return mInstance; }
	inline DisplaySettings		const& display()			const { return mDisplay; }
	inline HWND					const& WindowHandle()		const { return mWindowHandle; }
	inline LPPOINT				const& WindowCenterPoint()	const { return mWindowCenterPoint; }
	//inline LPPOINT				const& LastMousePoint()		const { return mLastMousePoint; } // UNUSED AT THE MOMENT
	inline LPPOINT				const& ActMousePoint()		const { return mActMousePoint; }

	static std::shared_ptr<CWindow> Create(HINSTANCE aInstance);

	static bool CreateAppWindow(std::shared_ptr<CWindow> Window);


protected:
	CWindow(HINSTANCE aInstance, DisplaySettings const& aDisplay);

private:

	HINSTANCE		 mInstance;
	HWND			 mWindowHandle;
	DisplaySettings mDisplay;
	LPPOINT 		 mWindowCenterPoint;
	// LPPOINT 		 mLastMousePoint;  UNUSED AT THE MOMENT
	LPPOINT 		 mActMousePoint;
	
};
