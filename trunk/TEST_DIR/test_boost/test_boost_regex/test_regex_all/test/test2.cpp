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


int main(int argc, char* argv[])

{    //( 1 )   ((  3  )  2 )((  5 )4)(    6    )   
	//(\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	//^Э��://��ַ(x.x...x)/·��(n��\�ִ�)/��ҳ�ļ�(xxx.xxx)
	//\w  ƥ������»��ߵ��κε����ַ����ȼ���'[A-Za-z0-9_]'�� /W��֮�෴ ƥ���κηǵ����ַ����ȼ��� '[^A-Za-z0-9_]'
	//+ ƥ��ǰ����ӱ��ʽһ�λ��Ρ�( ) ���һ���ӱ��ʽ�Ŀ�ʼ�ͽ���λ��   * ƥ��ǰ����ӱ��ʽ��λ��� 
	//? ƥ��ǰ����ӱ��ʽ��λ�һ��(�ȼ��� {0,1})����ָ��һ����̰���޶����� ʲô�Ƿ�̰���޶��� ? �� 

	//                   (\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	const char *szReg = "(\\w+)://((\\w+\\.)*\\w+)((/\\w*)*)(/\\w+\\.\\w+)?";
	const char *szStr = "http://www.cppprog.com/2009/0112/48.html";

	{    //�ַ���ƥ��
		boost::regex reg( szReg );
		bool r=boost::regex_match( szStr , reg);
		assert(r);
	}

	{    //��ȡ�Ӵ�
		boost::cmatch mat;
		boost::regex reg( szReg );
		bool r=boost::regex_match( szStr, mat, reg);
		if(r) //���ƥ��ɹ�
		{
			//��ʾ�����Ӵ�
			for(boost::cmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr)
			{
				//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
				cout << itr->first-szStr << ' ' << itr->second-szStr << ' ' << *itr << endl;
			}
		}
		//Ҳ��ֱ��ȡָ��λ����Ϣ
		if(mat[4].matched) cout << "Path is" << mat[4] << endl;
	}


	{ //����
		const char *szReg1 = "(\\d+)";
		std::string szStr1 = "http://www.cppprog.com/2009/0112/48.html";
		boost::cmatch mat;
		boost::smatch smt;
		boost::regex reg(szReg1);    //�����ַ����������
		std::string::const_iterator start = szStr1.begin();
		std::string::const_iterator end = szStr1.end();
		while(boost::regex_search(start, end, smt, reg))
		{
			//cout << "searched:" << mat[0] << endl;
			//��ʾ�����Ӵ�
			//for(boost::cmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr)
			//{
			//	//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
			//	cout << "searched:" << *itr << endl;
			//}
			std::cout<< "Have digit��" ; 
			std::string msg(smt[1].first, smt[1].second);
			std::cout<< msg.c_str() << std::endl;
			start = smt[0].second;
		}
	}

	{ //�滻
		boost::regex reg( szReg );
		string s = boost::regex_replace( string(szStr), reg, "ftp://$2$5");
		cout << "ftp site:"<< s << endl;
	}

	{ //�滻2����<>&ת������ҳ�ַ�
		string s1 = "(<)|(>)|(&)";
		string s2 = "(?1&lt;)(?2&gt;)(?3&amp;)";
		boost::regex reg( s1 );
		string s = boost::regex_replace( string("cout << a&b << endl;"), reg, s2, boost::match_default | boost::format_all);
		cout << "HTML:"<< s << endl;
	}

	{ //ʹ�õ������ҳ���������
		boost::regex reg( "\\d+" );    //�����ַ����������
		boost::cregex_iterator itrBegin = make_regex_iterator(szStr,reg); //(szStr, szStr+strlen(szStr), reg);
		boost::cregex_iterator itrEnd;
		for(boost::cregex_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
			cout << (*itr)[0].first-szStr << ' ' << (*itr)[0].second-szStr << ' ' << *itr << endl;
		}
	}

	{ //ʹ�õ���������ַ���
		boost::regex reg("/");  //��/������ַ���
		boost::cregex_token_iterator itrBegin = make_regex_token_iterator(szStr,reg,-1); //ʹ��-1����ʱ��֣�ʹ����������ʱ��ʾȡ�ڼ����Ӵ�����ʹ������ȡ�����
		boost::cregex_token_iterator itrEnd;
		for(boost::cregex_token_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			cout << *itr << endl;
		}
	}

	{ //ʹ�õ���������ַ���2
		boost::regex reg("(.)/(.)");  //ȡ/��ǰһ�ַ��ͺ�һ�ַ�������ַ�������ò���е�а��-_-��
		int subs[] = {1,2};        // ��һ�Ӵ��͵ڶ��Ӵ�
		boost::cregex_token_iterator itrBegin = make_regex_token_iterator(szStr,reg,subs); //ʹ��-1����ʱ��֣�ʹ����������ʱ��ʾȡ�ڼ����Ӵ�����ʹ������ȡ�����
		boost::cregex_token_iterator itrEnd;
		for(boost::cregex_token_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			cout << *itr << endl;
		}
	}

	cin.get();
	return 0;

} 
