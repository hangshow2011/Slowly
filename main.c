#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#define OPENGL_PAINT (WM_USER + 100)
#define OPENGL_LBUTTONDOWN (WM_USER + 101)

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_PAINT) {
        //-----------------------
        // PostMessage(HWND_BROADCAST, WM_USER + 100, (WPARAM)hwnd, 0);
        // PostMessage(hwnd, WM_USER + 100, 0, 0);
        // PostMessage(hwnd, WM_USER + 100, 0, 0);
       PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 确认发送消息

    // SendMessage(hwnd, WM_USER + 100, 0, 0);
    PostMessage(hwnd, WM_USER + 100, 0, 0);
    PostMessage(HWND_BROADCAST, WM_USER + 100, 0, 0);
    
    SwapBuffers(GetDC(hwnd));
    EndPaint(hwnd, &ps);
        //--------------------
    }
    if (msg == WM_LBUTTONDOWN) 
    {
         PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);  
        //-----------------------
        PostMessage(hwnd, WM_USER + 101, 0, 0);
        // PostMessage(HWND_BROADCAST, OPENGL_LBUTTONDOWN, (WPARAM)hwnd, 0);
        //--------------------
        SwapBuffers(GetDC(hwnd));
        EndPaint(hwnd, &ps);
        // InvalidateRect(hwnd, NULL, TRUE);
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "MSVCWindow";
    RegisterClass(&wc);
    
    HWND hwnd = CreateWindow("MSVCWindow", "MSVC OpenGL", WS_OVERLAPPEDWINDOW, 
                             100, 100, 800, 600, NULL, NULL, hInst, NULL);
    
    // 初始化OpenGL
    PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER};
    HDC hdc = GetDC(hwnd);
    SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
    wglMakeCurrent(hdc, wglCreateContext(hdc));
    
    ShowWindow(hwnd, nShow);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}