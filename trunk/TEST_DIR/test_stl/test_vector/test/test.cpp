// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//������������,�Լ���ʼ��
	vector<int> intvec0;        //����һ��int���͵Ŀյ�vector����
	vector<int> intvec1(5);     //����һ������5��int�������ݵ�vector,//Ĭ��ֵ��0
	vector<int> intvec2(5, 6);  //����һ������5��int�������ݵ�vector�����Ҷ���ʼ��Ϊ6
	vector<int> intvec3(3, 1, intvec2.get_allocator()); //�������������ĸ�������
	vector<int> intvec4(intvec2);    //����һ��int��vector����
	vector<int> intvec5(intvec4.begin()+1, intvec4.begin()+2);  //ͨ������intvec4[_First,_Last]����v5

	//FOR  ѭ����� ������ֵ
	cout<<"FOR  ѭ�����intvec:   "<<endl;
	cout<<"intvec1:   ";
	for(int i=0; i<(int)intvec1.size(); i++)
		cout<<intvec1[i]<<"  ";
	cout<<"\n";

	cout<<"intvec2:   ";
	for(int i=0; i<(int)intvec2.size(); i++)
		cout<<intvec2[i]<<"  ";
	cout<<"\n";

	cout<<"intvec3:   ";
	for(int i=0; i<(int)intvec3.size(); i++)
		cout<<intvec3[i]<<"  ";
	cout<<"\n";

	cout<<"intvec4:   ";
	for(int i=0; i<(int)intvec4.size(); i++)
		cout<<intvec4[i]<<"  ";
	cout<<"\n";

	cout<<"intvec5:   ";
	for(int i=0; i<(int)intvec5.size(); i++)
		cout<<intvec5[i]<<"  ";
	cout<<"\n"<<"\n";
	//vector �еĲ���
	cout<<"vector intvec1�еĲ���:   "<<endl;
	intvec1.push_back(5);   //һ���������뵽vector��ĩβ
	intvec1.push_back(6);
	intvec1.push_back(7);
	intvec1.push_back(8);
	intvec1.push_back(9);

	cout<<"intvec��һ��Ԫ����:";
	cout<<intvec1.front()<<"\n";    //���ص�һ��Ԫ�ء�������������Ƿ�Ϊ�գ�

	cout<<"intvec1[1]�ڶ���Ԫ����:";
	cout<<intvec1[1]<<"\n";        //������ָ����λ���ϵ�Ԫ��

	cout<<"intvec1.at(2)������Ԫ����:";
	cout<<intvec1.at(2)<<"\n";     //������ָ����λ���ϵ�Ԫ��

	cout<<"intvec1.back()���ĸ�Ԫ����:";
	cout<<intvec1.back()<<"\n";   //�������һ��Ԫ�أ�����������Ƿ�Ϊ�գ�
	cout<<"\n"<<"\n";



	//������ 
	//�������ϵ���Щ������ָ���ϵ���Ӧ��������ͬ�ġ������*��Ϊ��Ŀ�����ʹ��ʱ����Ϊ�������������
	//vector�������һ��typedef iterator������һ��public��Ա��ͨ��iterator�������������������еĵ�������
	cout<<"������ѭ�����intvec1:   "<<endl;
	//��Ϊiterator��һ��������vector���е�typedef�����Ա���ʹ����������vector��������Ԫ�����ͺ����������ʹ��iterator
	vector<int>::iterator intiter;    //��intiter����Ϊint���͵���������������
	//++intiter��������intiter�ӣ���ʹ��ָ�������е���һ��Ԫ�ء����ʽ��*intiter���ص�ǰ������λ���ϵ�Ԫ�ء�
	//����������������Ա����begin��end������begin���������е�һ��Ԫ�ص�λ�ã�����end�������������һ��Ԫ�ص�λ�á�
	for(intiter = intvec1.begin(); intiter!=intvec1.end(); intiter++)
		cout<<*intiter<<"  "; 
	cout<<"\n"<<"\n";

	cout<<"��������������С�Ĳ���      "<<endl;
	cout<<"��ԭʼ������������      ";
	cout<<intvec1.size()<<"\t";     //��������intvec�е�ǰ�ĸ���
	cout<<intvec1.capacity()<<"\t"; //���ز����·���ռ���Բ��뵽����intvec�е�Ԫ�ص�������
	cout<<intvec1.max_size()<<"\t"; //���ؿ��Բ��뵽����intvec�е�Ԫ�ص�������
	cout<<"\n";

	cout<<"ִ��resize�������������";
	intvec1.resize(100);  //��Ԫ�ظ�����Ϊnum�����size()���ӣ�Ĭ�ϵĹ��캯�����𴴽���Щ��Ԫ��
	cout<<intvec1.size()<<"\t";
	cout<<intvec1.capacity()<<"\t";
	cout<<intvec1.max_size()<<"\t";
	cout<<"\n";

	cout<<"ִ��reserve�������������";
	intvec1.reserve(200);   //���������洢����С�ĳ���
	cout<<intvec1.size()<<"\t";
	cout<<intvec1.capacity()<<"\t";
	cout<<intvec1.max_size()<<"\t";
	cout<<"\n"<<"\n";

	cout<<"ԭʼ�����ĵ�ǰԪ�ظ���"<<intvec1.size()<<endl;
	cout<<"ԭʼ�����е����ݣ�"<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";


	intvec1.insert(intvec1.begin()+2,10); //��elem(10)��һ���������뵽��position(intvec1.begin()+2)ָ����λ���ϣ���������Ԫ�ص�λ��
	cout<<"ִ��insert�������ĵ�ǰԪ�ظ���"<<intvec1.size()<<endl;
	cout<<"ִ��insert�������е����ݣ�"<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";

	intvec1.erase(intvec1.begin()+1);//ɾ����position(intvec1.begin()+1)ָ����λ���ϵ�Ԫ��
	cout<<"ִ��erase�������ĵ�ǰԪ�ظ���"<<intvec1.size()<<endl;
	cout<<"ִ��erase�������е����ݣ�"<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";

	intvec1.pop_back();  //ɾ�����Ԫ��
	cout<<"ִ��pop_back�������ĵ�ǰԪ�ظ���"<<intvec1.size()<<endl;
	cout<<"ִ��pop_back�������е����ݣ�"<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";

	intvec1.clear();  //��������ɾ������Ԫ��
	cout<<"ִ��clear�������еĴ�С��";
	cout<<intvec1.size();
	getchar();
	getchar();
	return 0;
}

