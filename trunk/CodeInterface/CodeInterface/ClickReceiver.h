#ifndef _ClickReceiver_H
#define _ClickReceiver_H

//点击接收者
class ClickReceiver 
{ 
public:
	ClickReceiver()	{};
	~ClickReceiver(){};

public: 
	virtual void OnClick() = 0; 
}; 


#else
#endif