#include "App.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui\imgui.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "NormalMapTwerker.h"
#include <sstream>

GDIPlusManager gdipm;

namespace
{
	App* app = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return app->WndProc(hwnd, msg, wParam, lParam);
}

App::App(HINSTANCE hInst, int width, int height, const std::string& commandLine)
	: hInstance(hInst), width(width), height(height)
{
	app = this;	
	// makeshift cli for doing some preprocessing bullshit (so many hacks here)
	if (this->commandLine != "")
	{
		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);
		if (nArgs >= 4 && std::wstring(pArgs[1]) == L"--ntwerk-rotx180")
		{
			const std::wstring pathInWide = pArgs[2];
			const std::wstring pathOutWide = pArgs[3];
			NormalMapTwerker::RotateXAxis180(
				std::string(pathInWide.begin(), pathInWide.end()),
				std::string(pathOutWide.begin(), pathOutWide.end())
			);
			throw std::runtime_error("Normal map processed successfully. Just kidding about that whole runtime error thing.");
		}
	}
}

HWND App::GetHWnd() const
{
	return hMainWnd;
}

HINSTANCE App::GetInst() const
{
	return hInstance;
}

float App::AspectRatio() const
{
	return static_cast<float>(width / height);
}

void App::InitApp()
{
	if (!InitWin())
	{
		MessageBox(0, L"Window Initialization Failed", 0, 0);
		return;
	}

	d3dApp = std::make_unique<D3DApp>(width, height);

	if (!d3dApp->InitD3D(hMainWnd))
	{
		MessageBox(0, L"D3D Initialization Failed", 0, 0);
		return;
	}

	light = std::make_unique<PointLight>(*d3dApp);
	goblin = std::make_unique<Model>(*d3dApp, "Models\\gobber\\GoblinX.obj", 6.0f);
	wall = std::make_unique<Model>(*d3dApp, "Models\\brick_wall\\brick_wall.obj", 6.0f);
	tp = std::make_unique<TestPlane>(*d3dApp, 6.0f);
	nano = std::make_unique<Model>(*d3dApp, "Models\\nano_textured\\nanosuit.obj", 2.0f);

	wall->SetRootTransform(DirectX::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	tp->SetPos({ 12.0f,0.0f,0.0f });
	goblin->SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	nano->SetRootTransform(DirectX::XMMatrixTranslation(0.0f, -7.0f, 6.0f));

	d3dApp->SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	gm.Start();
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = App::ProcessMessage())
		{
			return *ecode;
		}
		Draw();
	}

	return 0;
}

void App::Shutdown()
{
	ImGui_ImplWin32_Shutdown();
	
	UnregisterClass(L"D3DWndClassName", hInstance);
}

bool App::InitWin()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	hMainWnd = CreateWindow(L"D3DWndClassName", L"Muhammed Berkay Katranci",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	ImGui_ImplWin32_Init(hMainWnd);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));

	return true;
}

void App::UpdateScene(float dt)
{
}

void App::Draw()
{
	float dt = gm.DeltaTime() * speed_factor / 3000.0f;

	d3dApp->BeginScene();
	d3dApp->SetCamera(cam.GetMatrix());
	light->Bind(*d3dApp, cam.GetMatrix());

	goblin->Draw(*d3dApp);
	wall->Draw(*d3dApp);
	tp->Draw(*d3dApp);
	nano->Draw(*d3dApp);
	light->Draw(*d3dApp);

	while (const auto e = kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (CursorEnabled())
			{
				DisableCursor();
				mouse.EnableRaw();
			}
			else
			{
				EnableCursor();
				mouse.DisableRaw();
			}
			break;
		}
	}

	if (!CursorEnabled())
	{
		// Camera control
		if (kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (kbd.KeyIsPressed('A'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (kbd.KeyIsPressed('D'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
		if (kbd.KeyIsPressed(VK_SHIFT))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
	}

	// Point light control
	if (kbd.KeyIsPressed(VK_UP))
	{
		light->Translate({ 0.0f,0.0f,-dt });
	}
	if (kbd.KeyIsPressed(VK_DOWN))
	{
		light->Translate({ 0.0f,0.0f,dt });
	}
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		light->Translate({ dt,0.0f,0.0f });
	}
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
		light->Translate({ -dt,0.0f,0.0f });
	}
	if (kbd.KeyIsPressed(VK_INSERT))
	{
		light->Translate({ 0.0f,-dt,0.0f });
	}
	if (kbd.KeyIsPressed(VK_DELETE))
	{
		light->Translate({ 0.0f,dt,0.0f });
	}

	while (const auto delta = mouse.ReadRawDelta())
	{
		if (!CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}

	cam.SpawnControlWindow();
	light->SpawnControlWindow();
	goblin->ShowWindow(*d3dApp, "gobber");
	wall->ShowWindow(*d3dApp, "wall");
	tp->SpawnControlWindow(*d3dApp);
	nano->ShowWindow(*d3dApp, "nanosuit");

	d3dApp->EndScene();
}

void App::EnableCursor() noexcept
{
	cursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

void App::DisableCursor() noexcept
{
	cursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}

bool App::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

void App::ConfineCursor() noexcept
{
	RECT rect;
	GetClientRect(hMainWnd, &rect);
	MapWindowPoints(hMainWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void App::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void App::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void App::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void App::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void App::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

std::optional<int> App::ProcessMessage()
{
	MSG msg = {};

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return {};
}

LRESULT App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
	const auto& imio = ImGui::GetIO();

	switch (msg)
	{
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		// clear keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if (!cursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;

		/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		/*********** END KEYBOARD MESSAGES ***********/

		/************* MOUSE MESSAGES ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		if (!cursorEnabled)
		{
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
				HideCursor();
			}
			break;
		}
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!cursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// stifle this mouse message if imgui wants to capture
		if (imio.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	case WM_INPUT:
	{
		if (!mouse.RawEnabled())
		{
			break;
		}
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			break;
		}
		rawBuffer.resize(size);
		// read in the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			// bail msg processing if error
			break;
		}
		// process the raw input data
		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}
	/************** END MOUSE MESSAGES **************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}