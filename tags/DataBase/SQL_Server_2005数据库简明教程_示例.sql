/*例8-1　 创建一个带有SELECT语句的简单过程，该存储过程返回所有员工姓名，Email地址，电话等。该存储过程不使用任何参数*/
--无参数不需返回值
--创建
CREATE PROCEDURE au_infor_all
AS 
    SELECT  lastname ,
            firstname ,
            emailaddress ,
            phone
    FROM    person.contact
GO
--执行
EXEC au_infor_all
--删除
DROP PROCEDURE dbo.au_infor_all

/*例8-2 创建一个存储过程，以简化对sc表的数据添加工作，使得在执行该存储过程时，其参数值作为数据添加到表中*/
--这里没有相对应的sc表-----
--创建
CREATE PROCEDURE [dbo].[ pr1_sc_ins]
    @Param1 CHAR(10) ,
    @Param2 CHAR(2) ,
    @Param3 REAL
AS 
    BEGIN
        INSERT  INTO sc
                ( sno, cno, score )
        VALUES  ( @Param1, @Param2, @Param3 )
    END
--执行
    EXEC pr1_sc_ins '3130010211', 'c1', 85
GO
/*或在执行过程中变量用显式命名*/
EXEC pr1_sc_ins @Param1 = '3130010211', @Param2 = 'c1', @Param3 = 85
--删除
DROP PROCEDURE [dbo].[ pr1_sc_ins]


/*例8-3　创建一个带有参数的简单存储过程，从视图中返回指定的雇员（提供名和姓）及其职务和部门名称，该存储过程接受与传递的参数精确匹配的值*/
--创建
USE AdventureWorks;
GO
CREATE PROCEDURE GetEmployees
    @lastname VARCHAR(40) ,
    @firstname VARCHAR(20)
AS 
    SELECT  LastName ,
            FirstName ,
            JobTitle ,
            Department
    FROM    HumanResources.vEmployeeDepartment
    WHERE   FirstName = @firstname
            AND LastName = @lastname;
GO
--执行
EXEC GetEmployees 'Walters', 'Rob'
GO
/*或者*/
EXEC GetEmployees @lastname = 'Walters', @firstname = 'Rob'
GO
/*或者*/
EXEC GetEmployees @firstname = 'Rob', @lastname = 'Walters'
--删除
DROP PROCEDURE dbo.GetEmployees


--带参数的存储过程 
--在表person.contact中查找第一个名字为@firstname，最后一个名字为@lastname
--LIKE可以采用通配形式eg 'N%'
USE AdventureWorks;
GO
CREATE PROCEDURE au_infor2
    @lastname VARCHAR(40) = 'D%' ,
    @firstname VARCHAR(20) = '%'
AS 
    SELECT  firstname ,
            lastname ,
            phone
    FROM    person.contact
    WHERE   firstname LIKE @firstname
            AND lastname LIKE @lastname
GO

--查询过程
USE [AdventureWorks]
GO
DECLARE @return_value INT
EXEC @return_value = [dbo].[au_infor2] @lastname = N'Bl%', @firstname = N'Mi%'
SELECT  'Return Value' = @return_value
GO
/*或者*/
USE [AdventureWorks]
GO
DECLARE @return_value INT
EXEC @return_value = [dbo].[au_infor2] @lastname = N'Blythe',
    @firstname = N'Michael'
SELECT  'Return Value' = @return_value
GO
--删除
DROP PROCEDURE [dbo].[au_infor2]

/*例8-5 以下示例显示有一个输入参数和一个输出参数的存储过程。
存储过程中的第一个参数@sname将接收由调用程序指定的输入值(学生姓名)，
第二个参数@sscore（成绩）将用于将该值返回调用程序。
SELECT 语句使用@sname参数获取正确的	@sscore值，并将该值分配给输出参数。*/
--无相对应表 多表联合查询
CREATE PROCEDURE s_score
    @sname CHAR(8) ,
    @sscore REAL OUTPUT
AS 
    SELECT  @sscore = score
    FROM    sc
            JOIN s ON s.sno = sc.sno
    WHERE   sn = @sname
GO



/*例8-9*/
/* 创建了一个名为proc_person 的存储过程，该存储过程包含姓名和Email地址信息。然后，用ALTER PROCEDURE重新定义了该存储过程，使之只包含姓名信息，并使用ENCRYPTION关键字使之无法通过查看syscomments表来查看存储过程的内容。*/
/*程序清单如下*/

USE adventureworks
GO
/*创建一个存储过程，该存储过程包含姓名和Email地址信息*/
CREATE PROCEDURE proc_person
AS 
    SELECT  firstname ,
            lastname ,
            emailaddress
    FROM    person.contact
    ORDER BY lastname ,
            firstname
GO
/*查看该存储过程的源代码。*/
SELECT  o.id ,
        c.text
FROM    sysobjects o
        INNER JOIN syscomments c ON o.id = c.id
WHERE   o.type = 'P'
        AND o.name = 'proc_person'
/*下面对该存储过程进行重新定义。*/
ALTER PROCEDURE proc_person
    WITH ENCRYPTION
AS 
    SELECT  firstname ,
            lastname
    FROM    person.contact
    ORDER BY lastname ,
            firstname
GO
/*查看该存储过程的具体源代码。*/
SELECT  o.id ,
        c.text
FROM    sysobjects o
        INNER JOIN syscomments c ON o.id = c.id
WHERE   o.type = 'P'
        AND o.name = 'proc_person'
GO
