#include"top.h"
#include"hide.h"
#include"tray_set.h"
#include"lable_hook.h"
#include<thread>
using namespace std;

//全局变量****************************
#define DELAY 2000
extern top_list t;
extern lable lp;
extern hide_win h;
HHOOK keyboardHook;
//************************************

// 键盘钩子处理函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;

        // 检查是否按下Alt+T
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000); // 检查Alt键
            bool tPressed = (kbdStruct->vkCode == 'T');
            bool yPressed = (kbdStruct->vkCode == 'Y');
            bool hPressed = (kbdStruct->vkCode == 'H');
            bool gPressed = (kbdStruct->vkCode == 'G');
            //置顶
            if (altPressed && tPressed)
            {
                cout << "检测到快捷键" << std::endl;
                // 在这里执行你的功能
                wintop();
                // 返回1可以阻止该按键传递给其他应用程序
                // return 1;
            }
            //取消所有置顶
            if (altPressed && yPressed)
            {
                cout << "检测到快捷键" << std::endl;
                // 在这里执行你的功能
                clean_all_top();
                // 返回1可以阻止该按键传递给其他应用程序
                // return 1;
            }
            //隐藏
            if (altPressed && hPressed)
            {
                cout << "检测到快捷键" << std::endl;
                // 在这里执行你的功能
                h.hide();
                // 返回1可以阻止该按键传递给其他应用程序
                // return 1;
            }
            //取消所有隐藏
            if (altPressed && gPressed)
            {
                cout << "检测到快捷键" << std::endl;
                // 在这里执行你的功能
                h.restore();
                // 返回1可以阻止该按键传递给其他应用程序
                // return 1;
            }
            
        }
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //注册窗口类
    lp.register_win_class();
    thread(run_lable).detach();
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    cout << "创建窗口" << endl;
    //托盘处理
    creat_tray();





    // 消息循环 - 必须要有，否则程序会立即退出
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
