// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}


#include <iostream>
#include <boost/regex.hpp>
//#include "Regex1/regex.hpp"
using namespace std;

//�޶�������ָ��������ʽ��һ������ԭ�ӱ���Ҫ���ֶ��ٴβ�������ƥ�䡣
//�С�*��(��λ���)����+��(һ�λ���)��������(��λ�һ��)����{n}��(n ��)����{n,}��(����n ��)����{n��m}��(����ƥ�� n �������ƥ�� m ��)��6���޶�����
//����֮���������Ҫ���ظ�ƥ��Ĵ�����ͬ������"*"����+������{n��}���޶�������̰���� ����Ϊ���ǻᾡ���ܶ��ȥƥ�����֡�������ʾ��
//  \s  ƥ���κοհ��ַ��������ո��Ʊ������ҳ���ȵȡ� 
//   /a\s*b/     // "\s"��ʾ�հ�ԭ�ӣ�������ƥ��a b֮��û�пհס���һ�����߶���հ׵����
//   /a\d+b/     // "\d"��ʾһ�������ַ����ȼ��� [0-9]������ƥ��ab֮����һ�����߶�����ֵ����
//   /a\W?b/     // "\W"�ǵ����ַ����ȼ��� '[^A-Za-z0-9_]'����ƥ��ab֮����һ������û�������ַ� 
//   /ax{2��}b/  //����ƥ��ab֮��������2��x�ַ�
//Ĭ������£�������ʽʹ���ƥ��ԭ��Ҳ��̰��ƥ��ԭ�򣩡�


//̰��ƥ��
//Ĭ������£�������ʽʹ���ƥ��ԭ��Ҳ��̰��ƥ��ԭ�򣩡�
//���磺Ҫ��"zoom"��ƥ��"zo?"�Ĳ� ���滻��"r",�滻�ĵĽ����"rom"�����Ҫ��"zoom"��ƥ��"zo*"   �Ĳ����滻��"r",�滻��Ľ����"rm"��
//
//��̰��ƥ��
//���ַ������������޶�����*��+������{n}��{n,}��{n,m}��֮��ʱ��ƥ��ģʽ��������ƥ��ԭ��Ҳ �з�̰��ƥ��ԭ�򣩡�
//���磺���ַ���"fooood"�У�"fo+?"ֻƥ��"fo"���֣���"fo+"ƥ��"foooo���֡�

//��������ʽ�а����ܽ����ظ����޶���ʱ��ͨ������Ϊ�ǣ���ʹ�������ʽ�ܵõ�ƥ���ǰ���£�ƥ�価���ܶ���ַ���
//��������ʽΪ����a.*b��������ƥ�������a��ʼ����b�������ַ������������������aabab�Ļ�������ƥ�������ַ���aabab���ⱻ��Ϊ̰��ƥ�䡣
//��ʱ�����Ǹ���Ҫ����ƥ�䣬Ҳ����ƥ�価�����ٵ��ַ���ǰ��������޶��������Ա�ת��Ϊ����ƥ��ģʽ��ֻҪ�����������һ���ʺ�?��
//����.*?����ζ��ƥ�������������ظ�����������ʹ����ƥ��ɹ���ǰ����ʹ�����ٵ��ظ������ڿ������������Ӱɣ�
//a.*?b ƥ����̵ģ���a��ʼ����b�������ַ������������Ӧ����aabab�Ļ�������ƥ��aab��ab��

#include "QNA_File.h"
using namespace QNA;




int main3(int argc, char* argv[])

{    //( 1 )   ((  3  )  2 )((  5 )4)(    6    )   
	//(\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	//^Э��://��ַ(x.x...x)/·��(n��\�ִ�)/��ҳ�ļ�(xxx.xxx)
	//\w  ƥ������»��ߵ��κε����ַ����ȼ���'[A-Za-z0-9_]'�� /W��֮�෴ ƥ���κηǵ����ַ����ȼ��� '[^A-Za-z0-9_]'
	//+ ƥ��ǰ����ӱ��ʽһ�λ��Ρ�( ) ���һ���ӱ��ʽ�Ŀ�ʼ�ͽ���λ��   * ƥ��ǰ����ӱ��ʽ��λ��� 
	//? ƥ��ǰ����ӱ��ʽ��λ�һ��(�ȼ��� {0,1})����ָ��һ����̰���޶����� ʲô�Ƿ�̰���޶��� ? �� 

	//                   (\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	//const char *szReg = "(\\w+)://((\\w+\\.)*\\w+)((/\\w*)*)(/\\w+\\.\\w+)?";
	//const char *szStr = "http://www.cppprog.com/2009/0112/48.html";

	LPSTR pBuffer = NULL;
	//HTML�ļ���ȡ
	{
		char szFilename[MAX_PATH] = {"C:\\Lookup\\1.html"};
		CFile f;
		if( f.Open(szFilename)==TRUE ) 
		{
			FILETIME ftModified;
			f.GetFileTime(NULL, NULL, &ftModified);
			DWORD dwSize = f.GetSize();
			pBuffer = (LPSTR)new CHAR[dwSize + 1];
			f.Read(pBuffer, dwSize);
			pBuffer[dwSize] = '\0';
			f.Close();
			
		}

	}
	const char *szReg = "target=\"_blank\"\\>(.+?)\\</a\\>\\</td\\>";
	//try
	{    //�ַ���ƥ��
		boost::regex reg( szReg );
		bool r=boost::regex_match( pBuffer , reg);
		//boost::regex_error err;
		//cout<< err.what() <<endl;
		//assert(r);
	}
	//catch(boost::regex_error &err)
	//{
	//	cout<< err.what() <<endl;
	//	cin.get();
	//	return 0;
	//}
	//catch(...)
	//{
	//	cout<<"error"<<endl;
	//	cin.get();
	//	return 0;
	//}

	{    //��ȡ�Ӵ�
		boost::cmatch mat;
		boost::regex reg( szReg );
		bool r=boost::regex_match( pBuffer, mat, reg);
		if(r) //���ƥ��ɹ�
		{
			//��ʾ�����Ӵ�
			for(boost::cmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr)
			{
				//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
				cout << itr->first-pBuffer << ' ' << itr->second-pBuffer << ' ' << *itr << endl;
			}
		}
		//Ҳ��ֱ��ȡָ��λ����Ϣ
		if(mat[4].matched) cout << "Path is" << mat[4] << endl;
	}
	delete [] pBuffer;


	cin.get();
	return 0;

} 
