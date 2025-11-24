#include <windows.h>
#include <gl/gl.h>
#pragma comment(lib, "user32")
#pragma comment(lib, "opengl32")

static HWND g_mainHwnd = NULL;
static HHOOK g_hook = NULL;

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
     MSG* msg = (MSG*)lParam;
    if (msg->message == WM_USER + 100) {
        // MessageBox(g_mainHwnd, "DLL paint!", "Info", MB_OK);
        glColor3f(0.2f, 0.8f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(0.3f, 0.3f);
        glVertex2f(0.7f, 0.3f);
        glVertex2f(0.7f, 0.7f);
        glVertex2f(0.3f, 0.7f);
        glEnd();
        
        HDC hdc = GetDC(g_mainHwnd);
        SwapBuffers(hdc);
        ReleaseDC(g_mainHwnd, hdc);
    }
    return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

__declspec(dllexport) void HelloFromDLL(HWND hwnd) {
    g_mainHwnd = hwnd;
    
    g_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, 
                             GetModuleHandle("testdll.dll"), GetCurrentThreadId());
    
    // MessageBox(hwnd, "DLL message hook installed!", "Info", MB_OK);
    InvalidateRect(hwnd, NULL, TRUE);
}