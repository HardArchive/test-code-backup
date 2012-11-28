/*例8-1　 创建一个带有SELECT语句的简单过程，该存储过程返回所有员工姓名，Email地址，电话等。该存储过程不使用任何参数*/
/*程序清单如下*/
USE adventureworks
GO
CREATE PROCEDURE au_infor_all
AS
SELECT lastname, firstname, emailaddress, phone
    FROM person.contact
GO

/*例8-3　创建一个带有参数的简单存储过程，从视图中返回指定的雇员（提供名和姓）及其职务和部门名称，该存储过程接受与传递的参数精确匹配的值*/
/*程序清单如下*/
USE AdventureWorks;
GO
CREATE PROCEDURE GetEmployees 
    @lastname varchar(40), 
    @firstname varchar(20) 
AS 
    SELECT LastName, FirstName, JobTitle, Department
    FROM HumanResources.vEmployeeDepartment
    WHERE FirstName = @firstname AND LastName = @lastname;
GO




--调用存储过程
USE [AdventureWorks]
GO
DECLARE	@return_value int
EXEC	@return_value = [dbo].[au_infor_all]
SELECT	'Return Value' = @return_value
GO

--删除存储过程
drop procedure [dbo].[au_infor_all]  


--带参数的存储过程 
USE AdventureWorks;
GO
CREATE PROCEDURE GetEmployees --函数名
    @lastname varchar(40),    /*参数1 跟参数2可以放于一行*/
	@firstname varchar(20)    --参数2
AS 
--以下为执行语句
    SELECT LastName, FirstName, JobTitle, Department
    FROM HumanResources.vEmployeeDepartment
    WHERE FirstName = @firstname AND LastName = @lastname;
GO

/*
--------------------基本语法--------------------

一.创建存储过程
CREATE PROCEDURE procedure_name()
begin
.........
end

二.调用存储过程
1.基本语法：call procedure_name()
注意：存储过程名称后面必须加括号，哪怕该存储过程没有参数传递

三.删除存储过程
1.基本语法：
drop procedure procedure_name//

2.注意事项
(1)不能在一个存储过程中删除另一个存储过程，只能调用另一个存储过程

四.其他常用命令

1.show procedure status
显示数据库中所有存储的存储过程基本信息，包括所属数据库，存储过程名称，创建时间等

2.show create procedure sp_name
显示某一个mysql存储过程的详细信息
*/