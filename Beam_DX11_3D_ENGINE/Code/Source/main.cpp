#pragma once
#include <iostream>
#include <Windows.h>
#include "../Include/Base/Timer.h"
#include "../Include/Base/Window.h"
#include "../Include/Base/Input.h"
#include "../Include/DirectX/DXIntegration.h"
#include "../Include/Engine/Engine.h"
#include "../Include/Rendering/Renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		//START WINDOW
		std::shared_ptr<CWindow> Window = CWindow::Create(hInstance);
		if (!CWindow::CreateAppWindow(Window))
		{
			throw 0;
		}
		ShowWindow(Window->WindowHandle(), true);
		//WINDOW START SUCCESSFUL
		//START TIMER
		std::shared_ptr<CTimer> Timer = std::make_shared<CTimer>();
		Timer->initialize();
		//TIMER START SUCCESSFUL
		//INITIALIZE DIRECTX 11
		std::shared_ptr<CDXIntegration> CDXIntegration = CDXIntegration::Create(Window);
		//INITIALIZE DIRECTX 11 COMPLETE
		//START INPUT
		CInput inputState = { };
		//INPUT START SUCCESSFUL
		//START TIMER
		Timer->initialize();
		//TIMER START SUCCESSFUL
		//INITIALIZE RENDERER
		std::shared_ptr<CRenderer> renderer = std::make_shared<CRenderer>();
		renderer->Initialize(CDXIntegration);
		//INITIALIZE RENDERER COMPLETE
		//INITIALIZE ENGINE
		std::shared_ptr<CEngine> Engine = std::make_shared<CEngine>(renderer, CDXIntegration);
		Engine->Initialize();
		//INITIALIZE ENGINE COMPLETE


		//INITIALIZE GAME LOOP
		MSG msg = {};

		//START OF THE GAME LOOP
		do
		{
			inputState.reset();

			BOOL getMessageReturnCode = false;
			while (0 != (getMessageReturnCode = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)))
			{
				// msg.message:
				//   contains a value identified by the WM_-defines for each 
				//   message type of the windows message system.
				if (WM_KEYDOWN == msg.message)
				{
					inputState.setPressed(msg.wParam, true, (GetKeyState(VK_LCONTROL) & 0x8000));
				}

				if (WM_KEYUP == msg.message)
				{
					inputState.setPressed(msg.wParam, false);
				}

				// Handle window close events to have the application shut 
				// down on "x"-Button press or any other WM_QUIT event!

				TranslateMessage(&msg);
				LRESULT const dispatchReturnCode = DispatchMessage(&msg);
			}

			// Custom stuff
			Timer->update();

			CTimer::State const timerState = Timer->state();

			Engine->Update(timerState, inputState);
			Engine->Render();

			Sleep(15);
		} while (WM_QUIT != msg.message);
		//END OF THE GAME LOOP

		Timer->finalize();
		return 0;
	}
	catch (const std::exception&)
	{

	}
}
