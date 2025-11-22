#pragma once
#include<Windows.h>
#include<shellapi.h>
#include"hide.h"
#include"top.h"

// 常量定义
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAYICON 1001
#define IDM_EXIT 1002

bool SetTrayIconFromFile(HWND hwnd, const wchar_t* iconPath);

void RemoveTrayIcon();

void CreateTrayMenu();

void ShowTrayMenu(HWND hwnd);

void creat_tray();
