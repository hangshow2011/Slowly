#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>  

#define OPENGL_PAINT (WM_USER + 100)
#define OPENGL_LBUTTONDOWN (WM_USER + 101)

static HWND g_mainHwnd = NULL;
static HHOOK g_hook = NULL;

void button_on_paint(HWND hwnd) {
    
    glColor3f(1.0f, 0.0f, 0.0f);  
    
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(-0.2f, 0.0f);
    glEnd();
    
}

void button_on_lbutton(HWND hwnd) {
    glColor3f(0.9f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(-0.2f, 0.0f);
    glEnd();

     // 在绘制后交换缓冲区
    HDC hdc = GetDC(hwnd);
    SwapBuffers(hdc);
    ReleaseDC(hwnd, hdc);
    
    // system(compile_cmd);
    DeleteFile("testdll.dll");

    const char* compile_cmd = 
        "cmd /k \"\"F:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/Common7/Tools/VsDevCmd.bat\" && "
        "cl /LD /Fe:testdll.dll testdll.c user32.lib gdi32.lib\"";
    
    // const char* compile_cmd ="cl /LD /Fe:testdll.dll testdll.c user32.lib opengl32.lib > compile_log.txt 2>&1";
    system(compile_cmd);
    
    if (GetFileAttributes("testdll.dll") != INVALID_FILE_ATTRIBUTES) {
        HMODULE hDll = LoadLibrary("testdll.dll");
        if (hDll) {
            FARPROC func = GetProcAddress(hDll, "HelloFromDLL");
            if (func) {
                ((void(*)(HWND))func)(hwnd);
            }
            FreeLibrary(hDll);
        }
    }
}

LRESULT CALLBACK GetMsgProc2(int nCode, WPARAM wParam, LPARAM lParam) {
    MSG* msg = (MSG*)lParam;
    char debug[100];
    sprintf(debug, "Hook received message: 0x%X (%d)", msg->message, msg->message);
    OutputDebugString(debug);
    if (msg->message == WM_USER + 100) {
        button_on_paint(msg->hwnd);

        HDC hdc = GetDC(msg->hwnd);
        SwapBuffers(hdc);
        ReleaseDC(msg->hwnd, hdc);
    }
    if (msg->message == OPENGL_LBUTTONDOWN) {
        button_on_lbutton(msg->hwnd);

        
    }
    return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

void register_events(HWND hwnd) {
    g_mainHwnd = hwnd;

    DWORD mainThreadId = GetWindowThreadProcessId(hwnd, NULL);
    DWORD currentThreadId = GetCurrentThreadId();
    
    char debug[100];
    sprintf(debug, "Window thread: %d, Current thread: %d", mainThreadId, currentThreadId);
    MessageBox(hwnd, debug, "Thread Info", MB_OK);

    g_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc2, GetModuleHandle(NULL), GetCurrentThreadId());
    if (g_hook == NULL) {
        MessageBox(hwnd, "CallWndProc hook failed!", "Error", MB_OK);
    } 
}

#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) 
void (*_auto_reg_button)(void) = register_events;
