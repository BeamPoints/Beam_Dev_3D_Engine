#pragma once
#include "../../Include/Base/Window.h"
#include <Windows.h>


std::shared_ptr<CWindow> CWindow::Create(HINSTANCE aInstance)
{
	DisplaySettings display = {};
	display.width = GetSystemMetrics(SM_CXSCREEN);
	display.height = GetSystemMetrics(SM_CYSCREEN);

	std::shared_ptr<CWindow> wind = std::shared_ptr<CWindow>(new CWindow(aInstance, display));

	return wind;
}

CWindow::CWindow(HINSTANCE aInstance, DisplaySettings const &aDisplay)
{
	mInstance = aInstance;
	mWindowHandle = 0;
	mDisplay = aDisplay;
}

LRESULT CALLBACK __defaultClass_wndProc(HWND aWindowHandle, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
{
	switch (aMessage)
	{
	case WM_CLOSE:
		DestroyWindow(aWindowHandle);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return -1;
	default:
		return DefWindowProc(aWindowHandle, aMessage, aWParam, aLParam);
	}
}

 bool CWindow::CreateAppWindow(std::shared_ptr<CWindow> Window)
{
	std::string windowClassName = "Beams_DX11_3D_Class";
	std::string applicationName = "Beams_DX11_3D_ENGINE";
	WNDCLASSEX windowClass		= {};
	windowClass.cbSize			= sizeof(WNDCLASSEX);
	windowClass.lpszClassName	= windowClassName.c_str();
	windowClass.lpszMenuName	= nullptr;
	windowClass.hInstance		= Window->instanceHandle();
	windowClass.lpfnWndProc		= __defaultClass_wndProc;
	windowClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowClass.style			= (CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW);
	
	if (!RegisterClassEx(&windowClass))
	{
		return false;
	}

	DWORD windowStyle = 0;
	windowStyle |= WS_OVERLAPPEDWINDOW;
	windowStyle |= WS_VISIBLE;

	HWND windowHandle = CreateWindowEx(
		0,
		windowClassName.c_str(),
		applicationName.c_str(),
		windowStyle,
		0,
		0,
		Window->mDisplay.width,
		Window->mDisplay.height,
		NULL,
		NULL,
		Window->mInstance,
		nullptr);
	if (!windowHandle)
	{
		return false;
	}
	
	Window->mWindowHandle = windowHandle;
	
	return true;
}