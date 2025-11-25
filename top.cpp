#include"top.h"
using namespace std;


//全局变量
top_list t;
extern lable lp;
//容器类实现
top_list::top_list()
	{

	}
	
bool top_list::AddWindow(HWND hwnd)
	{
		status[hwnd] = true;
		return true;
	}

bool top_list::FindWindow(HWND hwnd)
	{
		if (status.count(hwnd) >0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

bool top_list::Remove(HWND hwnd)
	{
		if (status.erase(hwnd) > 0)
		{
			return true;
		}
		else
		{
			cout << "未找到对应窗口" << endl;
			return false;
		}
	}





//判断是否（该/能）置顶
bool is_enable_top(HWND hwnd)
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

//判端是否置顶重复，并取消置顶
bool is_repeatedly_top(HWND hwnd)
{
	// 实时检查窗口当前是否置顶
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	bool is_repeatedly = (exStyle & WS_EX_TOPMOST) != 0;
	if (is_repeatedly)
	{
		cout << "已经置顶，现取消置顶" << endl;
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		t.Remove(hwnd);//更新置顶容器
		lp.delete_lable(hwnd);//删除对应标签
		return true;
	}
	return false;
}
//置顶实现
int wintop()
{
	HWND current_win = GetForegroundWindow();

	if (!is_enable_top(current_win))
	{
		return -1;
	}

	if (is_repeatedly_top(current_win))
	{
		return 1;
	}

	bool top_success = SetWindowPos(current_win, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (!top_success)
	{
		cout << "置顶失败！" << endl;
		return -1;
	}
	//成功后加入置顶容器
	t.AddWindow(current_win);
	//创建标签
	lp.creat_lable(current_win);
	cout << "置顶成功" << endl;
	return 1;
}
//取消全部置顶
bool clean_all_top()
{
	for (const auto& p : t.status)
	{
		HWND hwnd = p.first;
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		lp.delete_lable(hwnd);
	}
	t.status.clear();
	cout << "取消所有置顶成功!" << endl;
	return 1;
}
