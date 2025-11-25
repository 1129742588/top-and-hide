#pragma once
#include<Windows.h>
#include<iostream>
#include<vector>
#include<unordered_map>
#include"lable_hook.h"


class top_list
{
public:
	std::unordered_map<HWND, bool> status;

	top_list();

	bool AddWindow(HWND hwnd);

	bool FindWindow(HWND hwnd);

	bool Remove(HWND hwnd);
};

int wintop();

bool is_enable_top(HWND hwnd);

bool clean_all_top();