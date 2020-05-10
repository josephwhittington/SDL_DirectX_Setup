#include <iostream>
#include <assert.h>
#include <d3d11.h>

#include "SDL.h"
#include "SDL_syswm.h"

#include "macros.h"

// Defines
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 800

// D3D11 Stuff
ID3D11Device* g_Device;
IDXGISwapChain* g_Swapchain;
ID3D11DeviceContext* g_DeviceContext;
float g_aspectRatio;
bool g_fullscreen = false;

// For drawing  -> New stuff right here
ID3D11RenderTargetView* g_RenderTargetView;
D3D11_VIEWPORT g_viewport;
// D3D11 Stuff

int main(int argc, char** argv)
{
	// Members
	bool RUNNING = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Initialization failed" << std::endl;
	}

	// Create window
	SDL_Window* m_window = SDL_CreateWindow("SDLTemplate",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	// Fullscreen
	if(g_fullscreen)
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

	if (!m_window)
	{
		std::cout << "Window initialization failed\n";
	}
	
	// Get window handle
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_window, &wmInfo);
	HWND hWnd = wmInfo.info.win.window;

	// D3d11 code here
	RECT rect;
	GetClientRect(hWnd, &rect);

	// Attach d3d to the window
	D3D_FEATURE_LEVEL DX11 = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swap;
	ZeroMemory(&swap, sizeof(DXGI_SWAP_CHAIN_DESC));
	swap.BufferCount = 1;
	swap.OutputWindow = hWnd;
	swap.Windowed = true;
	swap.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap.BufferDesc.Width = rect.right - rect.left;
	swap.BufferDesc.Height = rect.bottom - rect.top;
	swap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap.SampleDesc.Count = 1;

	g_aspectRatio = swap.BufferDesc.Width / (float)swap.BufferDesc.Height;

	HRESULT result;

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &DX11, 1, D3D11_SDK_VERSION, &swap, &g_Swapchain, &g_Device, 0, &g_DeviceContext);
	assert(!FAILED(result));

	ID3D11Resource* backbuffer;
	result = g_Swapchain->GetBuffer(0, __uuidof(backbuffer), (void**)&backbuffer);
	result = g_Device->CreateRenderTargetView(backbuffer, NULL, &g_RenderTargetView);
	assert(!FAILED(result));

	// Release the resource to decrement the counter by one
	// This is necessary to keep the buffer from leaking memory
	backbuffer->Release();

	// Setup viewport
	g_viewport.Width = swap.BufferDesc.Width;
	g_viewport.Height = swap.BufferDesc.Height;
	g_viewport.TopLeftY = g_viewport.TopLeftX = 0;
	g_viewport.MinDepth = 0;
	g_viewport.MaxDepth = 1;
	// D3d11 code here

	// Main loop
	while (RUNNING)
	{
		// Rendering Code
		ID3D11RenderTargetView* tempRTV[] = { g_RenderTargetView };
		g_DeviceContext->OMSetRenderTargets(1, tempRTV, nullptr);

		float color[4] = { 1, 0, 0, 1 };
		g_DeviceContext->ClearRenderTargetView(g_RenderTargetView, color);

		g_DeviceContext->RSSetViewports(1, &g_viewport);

		g_Swapchain->Present(0, 0);
		// Rendering Code

		// Event check
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				RUNNING = false;
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					RUNNING = false;
				else if (event.key.keysym.scancode == SDL_SCANCODE_F && g_fullscreen)
				{
					g_fullscreen = false;
					SDL_SetWindowFullscreen(m_window, 0);
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_F && !g_fullscreen)
				{
					g_fullscreen = true;
					SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
				}
			}
		}
	}

	// Delete
	D3DSAFERELEASE(g_Swapchain);
	D3DSAFERELEASE(g_DeviceContext);
	D3DSAFERELEASE(g_Device);
	D3DSAFERELEASE(g_RenderTargetView);
	// Delete

	// SDL shutdown
	if(m_window)
		SDL_DestroyWindow(m_window);
	SDL_Quit();

	return EXIT_SUCCESS;
}