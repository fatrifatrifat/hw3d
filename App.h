#pragma once
#include <algorithm>
#include <optional>
#include <cmath>
#include <vector>
#include <memory>
#include <set>
#include "D3DApp.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "GameTimer.h"
#include "PointLight.h"
#include "Camera.h"
#include "Mesh.h"
#include "ImguiManager.h"
#include "imgui\imgui_impl_win32.h"

class App
{
public:
	App(HINSTANCE hInst, int width, int height);
	App(const App& rhs) = delete;
	App& operator=(const App& rhs) = delete;

	HWND GetHWnd() const;
	HINSTANCE GetInst() const;
	float AspectRatio() const;

	virtual void InitApp();
	int Go();
	void Shutdown();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWin();

	virtual void UpdateScene(float dt);
	virtual void Draw();

	void EnableCursor() noexcept;
	void DisableCursor() noexcept;

	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowCursor() noexcept;
	void HideCursor() noexcept;
	void EnableImGuiMouse() noexcept;
	void DisableImGuiMouse() noexcept;

	static std::optional<int> ProcessMessage();

protected:
	HWND hMainWnd;
	HINSTANCE hInstance;
	GameTimer gm;

	std::unique_ptr<D3DApp> d3dApp;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	int width;
	int height;

	bool cursorEnabled = false;

	ImguiManager imgui;
	float speed_factor = 1.0f;
	Camera cam;
	std::unique_ptr<PointLight> light;

	std::unique_ptr<Model> nano;
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};