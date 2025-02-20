#include "App.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui\imgui.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
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

App::App(HINSTANCE hInst)
	: hInstance(hInst)
{
	app = this;	
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
	return 800.0f / 600.0f;
}

void App::InitApp()
{
	if (!InitWin())
	{
		MessageBox(0, L"Window Initialization Failed", 0, 0);
		return;
	}

	d3dApp = std::make_unique<D3DApp>();

	if (!d3dApp->InitD3D(hMainWnd))
	{
		MessageBox(0, L"D3D Initialization Failed", 0, 0);
		return;
	}

	light = std::make_unique<PointLight>(*d3dApp);

	Assimp::Importer imp;
	auto model = imp.ReadFile("Models\\OBJ\\suzanne.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	class Factory
	{
	public:
		Factory(D3DApp& d3dApp)
			:
			d3dApp(d3dApp)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };

			return std::make_unique<AssTest>(
				d3dApp, rng, adist, ddist, odist,
				rdist, mat, 1.5f
			);

			/*switch (sdist(rng))
			{
			case 0:
				return std::make_unique<Box>(
					d3dApp, rng, adist, ddist,
					odist, rdist, bdist, mat
				);
			case 1:
				return std::make_unique<Cylinder>(
					d3dApp, rng, adist, ddist, odist,
					rdist, bdist, tdist
				);
			case 2:
				return std::make_unique<Pyramid>(
					d3dApp, rng, adist, ddist, odist,
					rdist, tdist
				);
			case 3:
				return std::make_unique<SkinnedBox>(
					d3dApp, rng, adist, ddist, odist,
					rdist
				);
			case 4:
				return std::make_unique<Skull>(
					d3dApp, rng, adist, ddist, odist,
					rdist, mat
				);
			default:
				assert(false && "impossible drawable option in factory");
				return {};
			}*/
		}
	private:
		D3DApp& d3dApp;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0,4 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> tdist{ 3,30 };
	};

	Factory f(*d3dApp);
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

	for (auto& pd : drawables)
	{
		if (auto pb = dynamic_cast<Box*>(pd.get()))
		{
			boxes.push_back(pb);
		}
	}

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

LRESULT App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		keys[wParam] = true;
		return 0;
	case WM_KEYUP:
		keys[wParam] = false;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
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
	RECT R = { 0, 0, 800, 600 };
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

	for (auto& d : drawables)
	{
		d->Update(keys[VK_SPACE] ? 0.0f : dt);
		d->Draw(*d3dApp);
	}
	light->Draw(*d3dApp);

	SpawnSimulationWindow();
	cam.SpawnControlWindow();
	light->SpawnControlWindow();
	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();

	d3dApp->EndScene();
}

void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", keys[VK_SPACE] ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

void App::SpawnBoxWindowManagerWindow() noexcept
{
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				const bool selected = comboBoxIndex.has_value() && *comboBoxIndex == i;
				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboBoxIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
		{
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}

void App::SpawnBoxWindows() noexcept
{
	for (auto i = boxControlIds.begin(); i != boxControlIds.end(); )
	{
		if (!boxes[*i]->SpawnControlWindow(*i, *d3dApp))
		{
			i = boxControlIds.erase(i);
		}
		else
		{
			i++;
		}
	}
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