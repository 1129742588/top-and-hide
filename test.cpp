#include<Windows.h>
#include<iostream>
using namespace std;

HHOOK keyboardHook1;
HWND lable_hwnd = NULL;
const wchar_t* LABEL_CLASS_NAME1 = L"TopLable";



void re(HWND new_parent);


// 窗口消息处理函数
LRESULT CALLBACK LabelWindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        cout << "ceshi" << endl;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        // 使用内存DC避免闪烁
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        SelectObject(hdcMem, hBitmap);

        // 绘制背景
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdcMem, &rect, hBrush);
        DeleteObject(hBrush);

        // 绘制文字
        SetBkMode(hdcMem, TRANSPARENT);
        SetTextColor(hdcMem, RGB(255, 0, 0));
        DrawText(hdcMem, L"置顶", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // 复制到屏幕
        BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

        // 清理
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SETCURSOR:
        // 关键：确保显示箭头光标
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return TRUE;

    case WM_MOUSEACTIVATE:
        // 防止窗口被激活
        return MA_NOACTIVATE;

    case WM_DESTROY:
        return 0;
    }



    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//钩子消息处理
void CALLBACK HandleFocusEvent1(
    HWINEVENTHOOK hWinEventHook,  // 事件钩子句柄
    DWORD event,                  // 事件类型
    HWND hwnd,                    // 发生事件的窗口句柄
    LONG idObject,                // 对象ID
    LONG idChild,                 // 子ID
    DWORD dwEventThread,          // 事件线程ID
    DWORD dwmsEventTime)// 事件时间
{
    // 只处理窗口对象的事件
    if (idObject != OBJID_WINDOW || idChild != CHILDID_SELF) {
        return;
    }

    // 关键：过滤掉标签窗口自身的事件
    wchar_t className[256];
    GetClassNameW(hwnd, className, 256);
    if (wcscmp(className, LABEL_CLASS_NAME1) == 0) {
        return; // 忽略标签窗口自身的事件
    }

    // 过滤掉无效窗口
    if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) {
        return;
    }

    // 过滤掉太小或不重要的窗口
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        if (width < 100 || height < 50) {
            return;
        }
    }

    switch (event)
    {
    case EVENT_SYSTEM_FOREGROUND:
        HWND new_parent = GetForegroundWindow();
        cout << "窗口焦点变化" << endl;
        //re(new_parent);
        return;
    }

}



// 键盘钩子处理函数
LRESULT CALLBACK KeyboardProc1(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;

        // 检查是否按下Alt+T
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000); // 检查Alt键
            bool tPressed = (kbdStruct->vkCode == 'T');

            if (altPressed && tPressed)
            {
                std::cout << "Alt+T detected via hook!" << std::endl;
                // 在这里执行你的功能
                re(GetForegroundWindow());

                // 返回1可以阻止该按键传递给其他应用程序
                // return 1;
            }
        }
    }

    return CallNextHookEx(keyboardHook1, nCode, wParam, lParam);
}

//注册窗口
bool register_win_class1()
{
    // 注册窗口类
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = LabelWindowProc1;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"TopLable";
    return RegisterClassEx(&wc) != 0;
}

//创建窗口
HWND creat_lable1(HWND parent)
{
    HWND lable_win = NULL;
    lable_win = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,  // 多个扩展样式
        L"TopLable",  // 不同的类名
        L"",  // 空标题
        WS_POPUP | WS_VISIBLE,  // 弹出窗口样式
        0, 0,
        80, 25,  // 更小的尺寸
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    SetParent(lable_win, parent);// 建立父子关系
    SetLayeredWindowAttributes(lable_win, RGB(0, 0, 0), 0, LWA_COLORKEY);//设置指定颜色透明

    if (!lable_win)
    {
        cout << "创建窗口失败！" << std::endl;
        return 0;
    }
    return lable_win;
}

//重绑定父类
void re(HWND new_parent)
{
    SetParent(lable_hwnd, new_parent);
}


//int main()
//{
//    register_win_class1();
//    HWND hwnd = GetForegroundWindow();
//    lable_hwnd = creat_lable1(hwnd);
//
//    // 设置Windows事件钩子
//    HWINEVENTHOOK hook = SetWinEventHook(
//        EVENT_SYSTEM_FOREGROUND,  // 监听的事件类型：前景窗口变化
//        EVENT_OBJECT_LOCATIONCHANGE,  // 监听的事件范围结束（同开始，表示只监听这一种事件）
//        NULL,                     // 监听所有进程
//        HandleFocusEvent1,         // 事件发生时的回调函数
//        0, 0,                     // 监听所有进程和线程
//        WINEVENT_OUTOFCONTEXT     // 在发出事件的进程上下文之外调用
//    );
//
//    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
//
//
//    // 消息循环 - 必须要有，否则程序会立即退出
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    return 0;
//}
