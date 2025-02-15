#pragma once
#include <Windows.h>
#include <algorithm>
#include <optional>
#include <cmath>
#include <vector>
#include <memory>
#include "D3DApp.h"
#include "GeometryGenerator.h"
#include "Scene.h"
#include "GameTimer.h"
#include "Utils.h"
#include "InputClass.h"
#include "Box.h"
#include "Skull.h"
#include "Melon.h"
#include "Pyramid.h"
#include "Sheet.h"
#include "SkinnedBox.h"

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
	static constexpr size_t nDrawables = 180;
};

