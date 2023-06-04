#include <windows.h>
#include "util.cpp"

global_variable bool running = true;
// variable for the infinite game loop

struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// callback function
	// this is used to interact with the window messages

	LRESULT result = 0;
	// not sure

	switch (uMsg) {
		// switch for different cases of the window message
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;
		// if the process is killed or the window was closed, turn off the game loop


		case WM_SIZE: {
			// if the window is resized do something

			RECT rect;
			GetClientRect(hwnd, &rect);
			// get the window size

			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;
			// calc the buffer hieght and width depending on the windows rect valules

			int size = render_state.width * render_state.height * sizeof(unsigned int);
			// calc buffer size based on height and width


			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			// allocate this new buffers memory space, using render_state.size

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
			// gather information for the bitmap info

		} break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			// if nothing happens to the window return result, which has all the 'caught' params values
		}
	}
	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// create a window class
	WNDCLASS window_class = {};
	// new window class
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	// style config
	window_class.lpszClassName = "Game Window Class";
	// name for the window class
	window_class.lpfnWndProc = window_callback;
	// establish callback function

	// reg class
	RegisterClass(&window_class);
	// not sure

	// create window
	HWND window = CreateWindowA(window_class.lpszClassName, "My First Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	while (running) {

		// input
		MSG message;

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// sim
		// render_background();
		// clear_screen(0x00FF00);
		draw_rect_pixels(50, 50, 200, 500, 0xff0000);



		// render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

	}
}

https://www.youtube.com/watch?v=PJmpOhk9GXw&list=PL7Ej6SUky135IAAR3PFCFyiVwanauRqj3&index=3