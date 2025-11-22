#pragma once
#include<Windows.h>
#include<iostream>
#include<unordered_map>
#include"top.h"

//类
class lable
{
public:
    std::unordered_map<HWND, HWND> lable_list;//前者为父窗口，后者为标签子窗口

    //注册窗口
    bool register_win_class();
    //创建窗口
    HWND creat_lable(HWND parent);
    //隐藏所有标签
    void hide_all();
    //删除标签
    void delete_lable(HWND parent);
    //更新标签容器
    void update();
};


// 窗口消息处理函数
LRESULT CALLBACK LabelWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CALLBACK HandleFocusEvent(
    HWINEVENTHOOK hWinEventHook,  // 事件钩子句柄
    DWORD event,                  // 事件类型
    HWND hwnd,                    // 发生事件的窗口句柄
    LONG idObject,                // 对象ID
    LONG idChild,                 // 子ID
    DWORD dwEventThread,          // 事件线程ID
    DWORD dwmsEventTime);

//运行函数
int run_lable();