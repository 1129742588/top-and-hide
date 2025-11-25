#pragma once
#include<Windows.h>
#include<iostream>
#include<unordered_map>

class hide_win
{
public:
	std::unordered_map<HWND, bool> hide_list;
	void hide();

	void restore();


};
