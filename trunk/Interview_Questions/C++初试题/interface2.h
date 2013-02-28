 #pragma once

interface IController
{
	virtual void draw(HWND wnd, RECT & rc) PURE;
	virtual void on_btn_up_pressed() PURE;
	virtual void on_btn_left_pressed() PURE;
	virtual void on_btn_right_pressed() PURE;
	virtual void on_btn_down_pressed() PURE;
};

//IBox�ӿ��������ṩ��exeʵ�֣��ڵ����㵼������ʱ����IBoxָ�봫����
//����Ҫʵ��IController
interface IBox 
{
	virtual HWND getWND() PURE;
	virtual RECT getDrawRect() PURE;
};
//dll�ṩ�ĵ�������ԭ��
typedef bool (*dll_entry_type)(IBox * p_in/*IN*/, IController ** p_out/*OUT*/) ;

extern "C" __declspec(dllexport) bool dll_init(IBox * p_in/*IN*/, IController ** p_out/*OUT*/);