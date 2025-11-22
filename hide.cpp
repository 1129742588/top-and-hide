#include"hide.h"
using namespace std;

//全局变量
hide_win h;

//判断是否（该/能）置顶
bool is_enable_hid(HWND hwnd)
{
	// 基本检查：窗口必须存在且可见
	if (!IsWindowVisible(hwnd))
	{
		cout << "窗口不可见" << endl;
		return false;
	}

	// 获取窗口样式
	LONG style = GetWindowLongW(hwnd, GWL_STYLE);
	LONG exStyle = GetWindowLongW(hwnd, GWL_EXSTYLE);

	// 排除子窗口
	if (style & WS_CHILD)
	{
		cout << "是为子窗口" << endl;
		return false;
	}

	// 排除工具窗口（除非明确需要）
	if (exStyle & WS_EX_TOOLWINDOW)
	{
		cout << "是工具窗口" << endl;
		return false;
	}
	return true;
}

//类实现*****************************************
void hide_win::hide()
{
	HWND current_win = GetForegroundWindow();
	if (!is_enable_hid(current_win))
	{
		return;
	}
	//隐藏
	bool is_success = ShowWindow(current_win, SW_HIDE);
	if (!is_success)
	{
		cout << "隐藏失败" << endl;
		return;
	}
	//成功后加入容器
	hide_list[current_win] = true;
	cout << "隐藏成功" << endl;
}

void hide_win::restore()
{
	for (const auto& p :h.hide_list )
	{
		HWND hwnd = p.first;
		ShowWindow(hwnd, SW_SHOW);
	}
	h.hide_list.clear();
	cout << "取消所有隐藏成功" << endl;
}


//***********************************************
