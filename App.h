#pragma once
#include <algorithm>
#include <optional>
#include <cmath>
#include <vector>
#include <memory>
#include "D3DApp.h"
#include "GameTimer.h"
#include "Box.h"
#include "Skull.h"
#include "Melon.h"
#include "Pyramid.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "PointLight.h"
#include "Cylinder.h"
#include "Camera.h"
#include "ImguiManager.h"
#include "imgui\imgui_impl_win32.h"

class App
{
public:
	App(HINSTANCE hInst);
	App(const App& rhs) = delete;
	App& operator=(const App& rhs) = delete;

	HWND GetHWnd() const;
	HINSTANCE GetInst() const;
	float AspectRatio() const;

	virtual void InitApp();
	int Go();
	void Shutdown();

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWin();

	virtual void UpdateScene(float dt);
	virtual void Draw();

	static std::optional<int> ProcessMessage();

protected:
	HWND hMainWnd;
	HINSTANCE hInstance;
	GameTimer gm;

	std::unique_ptr<D3DApp> d3dApp;

private:
	bool keys[256] = { false };
	
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 150;

	ImguiManager imgui;
	float speed_factor = 1.0f;
	Camera cam;
	std::unique_ptr<PointLight> light;
};

