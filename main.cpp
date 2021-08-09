#include <tchar.h>
#include <windows.h>
#include <vector>
#include <thread>

#include <iostream>
using namespace std;

#include "./src/utils/variables.cpp"
#include "./maths.cpp"

// TODO: Remove this
#include "./src/elements/element.cpp"

// IDs
#define mat_id_empty 0
#define mat_id_sand 1
#define mat_id_water 2
#define mat_id_stone 3
// TODO: Remove this

#include "./renderer.cpp"
#include "./materials.cpp"

#include "./src/elementRegistry.cpp"

#include "./updates.cpp"

static s32 testForGUIs();

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	{
		globalVariables.gameIsRunning = false;
	}
	break;

	case WM_SIZE:
	{
		RECT rect = globalVariables.screenRect;
		GetClientRect(hwnd, &rect);
		s32 width = (rect.right - rect.left);
		s32 height = (rect.bottom - rect.top);
		s32 gridAmount = globalVariables.gridAmount;

		globalVariables.screenWidth = width;
		globalVariables.screenHeight = height;

		scale(width / gridAmount, height / gridAmount);

		s64 buffer_size = (s64)width * (s64)height * sizeof(u32);

		// GUIs
		s32 guiW = 50;
		s32 guiH = 50;
		s32 guiOff = 10;

		gui_t materials[] = {create_gui(mat_id_sand), create_gui(mat_id_water), create_gui(mat_id_stone)};
		for (s32 i = 0; i < globalVariables.totalParticleTypes; i++)
		{
			s32 x0 = globalVariables.screenWidth - (guiW + guiOff);
			s32 y0 = guiOff + i * (guiH + guiOff);
			s32 x1 = x0 + guiW;
			s32 y1 = y0 + guiW;
			materials[i].rect = {x0, y0, x1, y1};

			*((gui_t *)globalVariables.matGUIs + i) = materials[i];
		}

		// Chunks
		s32 chunkAmount = globalVariables.chunkAmount;
		s32 chunkW = gridAmount / chunkAmount;
		s32 chunkH = gridAmount / chunkAmount;
		for (s32 j = 0; j < chunkAmount; j++)
		{
			for (s32 i = 0; i < chunkAmount; i++)
			{
				chunk_t *chunk = (chunk_t *)globalVariables.chunks + (i + j * chunkAmount);
				s32 x = i * chunkW;
				s32 y = j * chunkH;
				RECT rect = {x, y, x + chunkW, y + chunkH};
				chunk->bounds = rect;
				RECT dirty = {x, y, x + chunkW, y + chunkH};
				chunk->dirtyRect = dirty;
			}
		}

		if (globalVariables.memory)
			VirtualFree(globalVariables.memory, 0, MEM_RELEASE);
		globalVariables.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		globalVariables.bitmap_info.bmiHeader.biSize = sizeof(globalVariables.bitmap_info.bmiHeader);
		globalVariables.bitmap_info.bmiHeader.biWidth = width;
		globalVariables.bitmap_info.bmiHeader.biHeight = height;
		globalVariables.bitmap_info.bmiHeader.biPlanes = 1;
		globalVariables.bitmap_info.bmiHeader.biBitCount = 32;
		globalVariables.bitmap_info.bmiHeader.biCompression = BI_RGB;
	}
	break;

	case WM_LBUTTONDOWN:
	{
		s32 id = testForGUIs();
		if (id != -1)
		{
			globalVariables.currentMatID = id;
		}
		else
		{
			globalVariables.holdingLeft = true;
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		globalVariables.holdingLeft = false;
	}
	break;

	case WM_RBUTTONDOWN:
	{
		POINT tile = mouseToTile(globalVariables.mouse);

		globalVariables.shower.id = globalVariables.currentMatID;
		globalVariables.shower.pos = tile;
	}
	break;

	default:
	{
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	break;
	}
	return result;
}

s32 WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = _T("Game Window Class");
	window_class.lpfnWndProc = window_callback;

	// Register Class
	RegisterClass(&window_class);

	// Create Window
	s32 offX = 16;
	s32 offY = 39;
	HWND window = CreateWindow(window_class.lpszClassName, _T("My First Game!"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 768 + offX, 768 + offY, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	// Create World Buffer
	s32 world_buffer_size = globalVariables.worldSize * sizeof(Element);
	globalVariables.worldBuffer = VirtualAlloc(0, world_buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	fillWorld(mat_id_empty);

	POINT point = {};
	while (globalVariables.gameIsRunning)
	{
		// Update Frame
		globalVariables.currentFrame++;

		// Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);

			GetCursorPos(&globalVariables.mouse);
			ScreenToClient(window, &globalVariables.mouse);

			// GetCursorPos(&point);
			// calculate_mouse(point);
		}

		// Mouse Input
		if (globalVariables.holdingLeft)
		{
			POINT tile = mouseToTile(globalVariables.mouse);
			if (globalVariables.currentMatID != mat_id_stone)
			{
				if (globalVariables.currentFrame % 6 == 0)
				{
					placeCircleParticle(tile.x, tile.y, 10, globalVariables.currentMatID, true);
				}
			}
			else
			{
				placeCircleParticle(tile.x, tile.y, 2, globalVariables.currentMatID, false);
			}
		}

		if (globalVariables.shower.id != -1)
		{
			if (globalVariables.currentFrame % 6 == 0)
			{
				s32 id = globalVariables.shower.id;
				s32 x = globalVariables.shower.pos.x;
				s32 y = globalVariables.shower.pos.y;
				placeCircleParticle(x, y, 1, id, false);
			}
		}

		// POINT tile = {globalVariables.gridAmount / 2, 10};
		// if (globalVariables.currentFrame % 10 == 0)
		// {
		// 	placeCircleParticle(tile.x, tile.y, 10, globalVariables.currentMatID, true);
		// }

		// Clean
		clearScreen(0x000000);

		// Update World
		prepareBoard();
		updateBoard();
		renderBoard();

		// Update GUIs
		for (s32 i = 0; i < globalVariables.totalParticleTypes; i++)
		{
			gui_t gui = *((gui_t *)globalVariables.matGUIs + i);
			RECT rect = gui.rect;
			fillRectWithoutScale(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, gui.color);
		}

		// Chunks
		for (s32 i = 0; i < globalVariables.totalChunkSize; i++)
		{
			chunk_t *chunk = (chunk_t *)globalVariables.chunks + i;
			u32 color = 0x00ff00;

			if (chunk->particleAmount == 0)
				continue;
			RECT rect = chunk->bounds;
			RECT dirty = chunk->dirtyRect;
			strokeRect(dirty.left, dirty.top, dirty.right - dirty.left, dirty.bottom - dirty.top, 0xffffff);
			// strokeRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, color);
		}
		// cout << chunk_string << endl;

		// Render
		StretchDIBits(hdc, 0, 0, globalVariables.screenWidth, globalVariables.screenHeight, 0, 0, globalVariables.screenWidth, globalVariables.screenHeight, globalVariables.memory, &globalVariables.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
	return 0;
}

static s32 testForGUIs()
{
	RECT bound = {};
	RECT first = ((gui_t *)globalVariables.matGUIs)->rect;
	RECT last = ((gui_t *)globalVariables.matGUIs + globalVariables.totalParticleTypes - 1)->rect;
	bound.left = first.left;
	bound.top = first.top;
	bound.right = last.right;
	bound.bottom = last.bottom;

	POINT m = globalVariables.mouse;
	if (m.x < bound.left || m.x > bound.right || m.y < bound.top || m.y > bound.bottom)
		return -1;
	for (s32 i = 0; i < globalVariables.totalParticleTypes; i++)
	{
		gui_t gui = globalVariables.matGUIs[i];
		RECT rect = gui.rect;
		if (m.x > rect.left && m.x < rect.right && m.y > rect.top && m.y < rect.bottom)
			return gui.id;
	}

	return globalVariables.currentMatID;
}