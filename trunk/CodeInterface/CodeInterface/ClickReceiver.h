#ifndef _ClickReceiver_H
#define _ClickReceiver_H

//���������
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