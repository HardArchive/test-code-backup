
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// debug_corp.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "k_mysql.h"

int main(int argc, char* argv[])
{
	//------------------------------------------------------------------------
	K_Database a;
	int i;
	// -----------------------------------------------------------------

	printf("��������һ�����  --���ݿ�����-- \n");
	i=a.Connect("10.0.100.202", "root", "tiptop", "confidentialcirculation", 3306, 0);
	if(i!=0) 
	{
		printf("���ݿ�����ʧ��\n");
		return 0;
	}
	else
	{
		printf("�Ѿ��ɹ����ӵ��ƶ������ݿ�\n");
	}
	K_recordSet sa;
	a.ExecQueryGetRecord("select * from leadership",sa);
	for(i=0;i<sa.GetRecordCount();i++)
	{
		for(int j=0;j<sa.GetFieldNum();j++)
		{
			printf("%s\t",sa[i][j].c_str ());
		}
		printf("\n");
	}


	k_sqlfactory ss;
	ss.tablename ="fgdf";
	ss.where ="1";
	ss.add_value ("gf","fsdfffffs");
	ss.add_value ("gdf","2004-4-5");
	printf("%s",ss.get_add ().c_str ());
	printf("\n%s",ss.get_edit ().c_str ());

	a.ExecQuery (ss.get_add ());
	a.ExecQuery (ss.get_edit ());
	//---------------------------------------------------------------

	a.DisConnect ();
	//---------------------------------------------------------------------------

	return 0;
}

