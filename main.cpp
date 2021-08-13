#include "./src/utils/utils.cpp"
#include "./src/elementRegistry.cpp"

// static s32 testForGUIs();

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
		RECT rect = {};

		GetClientRect(hwnd, &rect);
		s32 width = (rect.right - rect.left);
		s32 height = (rect.bottom - rect.top);

		delete &rect;

		globalVariables.screenWidth = width;
		globalVariables.screenHeight = height;

		fixGUIs();

		scale(2, 2);

		s64 buffer_size = (s64)width * (s64)height * sizeof(u32);

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
			cout << "GUI: " << id << endl;
			globalVariables.currentMatID = id;
		}
		else
		{
			cout << "No GUI" << endl;
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
		Vector tile = mouseToTile(globalVariables.mouse);
		POINT p = {tile.x, tile.y};

		globalVariables.shower.id = globalVariables.currentMatID;
		globalVariables.shower.pos = p;
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
	s32 offX = 16;	 // 16
	s32 offY = 39;	 // 39
	s32 w = 256 * 2; // 256 * 3
	s32 h = 256 * 2; // 256 * 3
	HWND window = CreateWindow(window_class.lpszClassName, _T("My First Game!"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, w + offX, h + offY, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	// GUIs
	generateAllGUIs();

	// FPS Things
	clock_t current_ticks, delta_ticks;
	s64 fps = 0;
	while (globalVariables.gameIsRunning)
	{
		// Calculate FPS
		current_ticks = clock();

		// Update Frame
		globalVariables.currentFrame++;

		// Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);

			POINT p = {};
			GetCursorPos(&p);
			ScreenToClient(window, &p);

			globalVariables.oldMouse.x = globalVariables.mouse.x;
			globalVariables.oldMouse.y = globalVariables.mouse.y;

			globalVariables.mouse.x = p.x;
			globalVariables.mouse.y = p.y;

			delete &p;
		}

		// Mouse Input
		if (globalVariables.holdingLeft)
		{
			Vector oldTile = mouseToTile(globalVariables.oldMouse);
			Vector tile = mouseToTile(globalVariables.mouse);
			s32 id = globalVariables.currentMatID;
			if (id != STONE)
			{
				if (globalVariables.currentFrame % 6 == 0)
				{
					sandWorld.setCirclePixel(tile.x, tile.y, 10, create_particle(id));
				}
			}
			else
			{
				sandWorld.setCircleLine(oldTile.x, oldTile.y, tile.x, tile.y, 2, create_particle(id));
			}
		}

		if (globalVariables.shower.id != -1 && globalVariables.currentFrame % 6 == 0)
		{
			s32 id = globalVariables.shower.id;
			s32 x = globalVariables.shower.pos.x;
			s32 y = globalVariables.shower.pos.y;
			sandWorld.setPixel(x, y, create_particle(id));
		}

		// Clean
		// clearScreen(0x000000);

		// Update World
		sandWorld.update();
		sandWorld.render();

		// Render
		StretchDIBits(hdc, 0, 0, globalVariables.screenWidth, globalVariables.screenHeight, 0, 0, globalVariables.screenWidth, globalVariables.screenHeight, globalVariables.memory, &globalVariables.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		// Calculate FPS
		delta_ticks = clock() - current_ticks;
		if (delta_ticks > 0)
		{
			fps = CLOCKS_PER_SEC / delta_ticks;
			// cout << "Delta Ticks: " << delta_ticks << " FPS: " << fps << endl;
		}
		else
		{
			fps = INFINITE;
		}
	}
	return 0;
}