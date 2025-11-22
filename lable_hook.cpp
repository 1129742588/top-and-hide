#include"lable_hook.h"

using namespace std;




lable lp;
const wchar_t* LABEL_CLASS_NAME = L"TopLable";
extern top_list t;



//************************************************
//注册窗口
bool lable::register_win_class()
    {
        // 注册窗口类
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = LabelWindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"TopLable";
        return RegisterClassEx(&wc) != 0;
    }

//创建窗口
HWND lable::creat_lable(HWND parent)
{
        //判断是否已经创建了置顶标签
        if (lp.lable_list.count(parent) != 0)
        {
            return lp.lable_list[parent];
        }
        HWND lable_win=NULL;
        lable_win = CreateWindowEx(
            WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,  // 多个扩展样式
            L"TopLable",  // 不同的类名
            L"",  // 空标题
            WS_POPUP | WS_VISIBLE,  // 弹出窗口样式
            0, 0,
            80, 25,  // 更小的尺寸
            NULL, NULL, GetModuleHandle(NULL), NULL
        );
        //这里的绑定必须在颜色透明之前，不然显示不出窗口
        SetParent(lable_win, parent);// 建立父子关系
        SetLayeredWindowAttributes(lable_win, RGB(0, 0, 0), 0, LWA_COLORKEY);//设置指定颜色透明
        

        if (!lable_win)
        {
            cout << "创建窗口失败！" << std::endl;
            return 0;
        }
        lable_list[parent] = lable_win;
        return lable_win;
}

//隐藏所有标签
void lable::hide_all()
{
    for (const auto& p : lable_list)
    {
        HWND hwnd = p.second;
        ShowWindow(hwnd, SW_HIDE);
    }
};

//删除标签
void  lable::delete_lable(HWND parent)
{
    DestroyWindow(lable_list[parent]);
    if (lable_list.erase(parent) > 0)
    {
        cout << "删除标签容器" << endl;
        return;
    }
}

//更新标签容器
void lable::update()
{
    
}
//*************************************************





// 窗口消息处理函数
LRESULT CALLBACK LabelWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
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

void CALLBACK HandleFocusEvent(
        HWINEVENTHOOK hWinEventHook,  // 事件钩子句柄
        DWORD event,                  // 事件类型
        HWND hwnd,                    // 发生事件的窗口句柄
        LONG idObject,                // 对象ID
        LONG idChild,                 // 子ID
        DWORD dwEventThread,          // 事件线程ID
        DWORD dwmsEventTime           )// 事件时间
{
    // 只处理窗口对象的事件
    if (idObject != OBJID_WINDOW || idChild != CHILDID_SELF) {
        return;
    }

    // 关键：过滤掉标签窗口自身的事件
    wchar_t className[256];
    GetClassNameW(hwnd, className, 256);
    if (wcscmp(className, LABEL_CLASS_NAME) == 0) {
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

    switch(event)
    {
        case EVENT_SYSTEM_FOREGROUND:
            //更新置顶容器和标签容器，排除无效窗口
            
            //判断当前窗口是否有绑定的标签
            HWND current_win = GetForegroundWindow();
            if(lp.lable_list.count(current_win)>0)
            {
                lp.hide_all();
                ShowWindow(lp.lable_list[current_win], SW_SHOW);
                cout << "窗口焦点变化" << endl;
                cout << "当前置顶容器数量：" << t.status.size() << endl;
                cout << "当前标签容器数量：" << lp.lable_list.size() << endl;

                return;
            }
            lp.hide_all();
            return;
    }

}

int run_lable()
{
    lp.register_win_class();
    // 设置Windows事件钩子
    HWINEVENTHOOK hook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,  // 监听的事件类型：前景窗口变化
        EVENT_OBJECT_LOCATIONCHANGE,  // 监听的事件范围结束（同开始，表示只监听这一种事件）
        NULL,                     // 监听所有进程
        HandleFocusEvent,         // 事件发生时的回调函数
        0, 0,                     // 监听所有进程和线程
        WINEVENT_OUTOFCONTEXT     // 在发出事件的进程上下文之外调用
    );
    // 检查钩子是否设置成功
    if (!hook) {
        std::cout << "错误：无法设置事件钩子" << std::endl;
        return 1;
    }
    std::cout << "钩子设置成功，开始监听焦点变化..." << std::endl;
    std::cout << "按任意键退出程序" << std::endl;

    // 消息循环 - 必须要有，否则程序会立即退出
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 程序退出前卸载钩子
    UnhookWinEvent(hook);
    std::cout << "程序已退出" << std::endl;

    return 0;
}
