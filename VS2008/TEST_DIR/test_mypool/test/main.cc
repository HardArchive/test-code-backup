#include "stdafx.h"

#include "contrainer.h"
#include "manage.h"
#pragma comment(lib,"libmySQL.lib")
int main()
{
	size_t t = 5;
	MYSQL *my_connection;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    int res;
    int field_cout;
    vector <string> coll;
    vector<vector<string> > result;
	
	using namespace pool;
	// input your mysql
	UserInfo ino = {
		"root", //user of your db
		"test", //name of your db
		"tiptop" //pwd of your db
	};
	
	WManage mage(ino);	
	my_connection = mage.openConnection();
	assert(my_connection);
	res = mysql_query(my_connection,"select * from food;");
	
	
	res_ptr = mysql_use_result(my_connection);
	
	//assert(!res && res_ptr && my_connection);

	
	while((sqlrow=mysql_fetch_row(res_ptr)))
	{
		unsigned int field_cout;
		field_cout=0;
		vector<string> colIterator;
		while(field_cout<mysql_field_count(my_connection))
		{
			colIterator .push_back(sqlrow[field_cout]);
			cout << sqlrow[field_cout] << " ";
			field_cout++;			
		}
		std::cout<<std::endl;
		result.push_back(colIterator);
	}
	
}
