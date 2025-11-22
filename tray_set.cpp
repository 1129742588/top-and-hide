#include"tray_set.h"
// 全局变量
NOTIFYICONDATA g_trayData;
HMENU g_trayMenu = NULL;
extern hide_win h;
//窗口托盘回调
LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // 初始化托盘
        SetTrayIconFromFile(hwnd, L"./ICO/ico.ico");
        CreateTrayMenu();
        break;

    case WM_TRAYICON:
        // 右键点击显示菜单
        if (lParam == WM_RBUTTONUP) 
        {
            ShowTrayMenu(hwnd);
        }
        break;

    case WM_COMMAND:
        // 处理退出命令
        if (LOWORD(wParam) == IDM_EXIT)
        {
            h.restore();
            clean_all_top();
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        break;

    case WM_DESTROY:
        // 清理资源
        RemoveTrayIcon();
        if (g_trayMenu)
        {
            DestroyMenu(g_trayMenu);
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

//加载并设置图标
bool SetTrayIconFromFile(HWND hwnd, const wchar_t* iconPath) 
{
    // 从文件加载图标
    HICON hIcon = (HICON)LoadImage(
        NULL,                   // 实例句柄，NULL表示从文件加载
        iconPath,               // 图标文件路径
        IMAGE_ICON,             // 加载类型为图标
        0, 0,                   // 使用图标原始大小
        LR_LOADFROMFILE | LR_DEFAULTSIZE // 从文件加载，使用默认大小
    );

    if (!hIcon) {
        // 如果加载失败，使用系统默认图标
        hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    // 设置托盘图标
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1001;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = hIcon;
    wcscpy_s(nid.szTip, L"top-and-hide");

    bool success = Shell_NotifyIcon(NIM_ADD, &nid);

    // 注意：不要销毁hIcon，系统会管理托盘图标的生命周期
    return success;
}

// 删除托盘图标
void RemoveTrayIcon()
{
    Shell_NotifyIcon(NIM_DELETE, &g_trayData);
}

// 创建托盘菜单
void CreateTrayMenu() 
{
    g_trayMenu = CreatePopupMenu();
    AppendMenu(g_trayMenu, MF_STRING, IDM_EXIT, L"退出");
}

// 显示托盘菜单
void ShowTrayMenu(HWND hwnd) 
{
    POINT pt;
    GetCursorPos(&pt);

    SetForegroundWindow(hwnd);
    TrackPopupMenu(g_trayMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);
}
//创建托盘
void creat_tray()
{
    //注册窗口
    WNDCLASS tray = { 0 };
    tray.lpfnWndProc = TrayWndProc;
    tray.hInstance = GetModuleHandle(NULL);
    tray.lpszClassName = L"Tray";
    tray.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&tray);

    // 创建隐藏窗口
    HWND hwnd = CreateWindow(
        tray.lpszClassName,
        L"",
        WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
}


