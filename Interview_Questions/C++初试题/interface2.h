 #pragma once

interface IController
{
	virtual void draw(HWND wnd, RECT & rc) PURE;
	virtual void on_btn_up_pressed() PURE;
	virtual void on_btn_left_pressed() PURE;
	virtual void on_btn_right_pressed() PURE;
	virtual void on_btn_down_pressed() PURE;
};

//IBox接口由我们提供的exe实现，在调用你导出函数时，把IBox指针传给你
//你需要实现IController
interface IBox 
{
	virtual HWND getWND() PURE;
	virtual RECT getDrawRect() PURE;
};
//dll提供的导出函数原型
typedef bool (*dll_entry_type)(IBox * p_in/*IN*/, IController ** p_out/*OUT*/) ;

extern "C" __declspec(dllexport) bool dll_init(IBox * p_in/*IN*/, IController ** p_out/*OUT*/);