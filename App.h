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
	float x = 0.f;
	float y = 0.f;
	float z = -5.f;

	float vx = 0.f;
	float vy = 0.f;
	float vz = 0.f;

	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<std::unique_ptr<Scene>>::iterator currScene;

	float mTheta = PI * 1.5f;
	float mPhi = PI * 0.25f;
	float mRadius = 5.0f;

	float dTheta = PI * 0.0001f;

};

