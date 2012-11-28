/*��8-1�� ����һ������SELECT���ļ򵥹��̣��ô洢���̷�������Ա��������Email��ַ���绰�ȡ��ô洢���̲�ʹ���κβ���*/
--�޲������践��ֵ
--����
CREATE PROCEDURE au_infor_all
AS 
    SELECT  lastname ,
            firstname ,
            emailaddress ,
            phone
    FROM    person.contact
GO
--ִ��
EXEC au_infor_all
--ɾ��
DROP PROCEDURE dbo.au_infor_all

/*��8-2 ����һ���洢���̣��Լ򻯶�sc���������ӹ�����ʹ����ִ�иô洢����ʱ�������ֵ��Ϊ������ӵ�����*/
--����û�����Ӧ��sc��-----
--����
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
--ִ��
    EXEC pr1_sc_ins '3130010211', 'c1', 85
GO
/*����ִ�й����б�������ʽ����*/
EXEC pr1_sc_ins @Param1 = '3130010211', @Param2 = 'c1', @Param3 = 85
--ɾ��
DROP PROCEDURE [dbo].[ pr1_sc_ins]


/*��8-3������һ�����в����ļ򵥴洢���̣�����ͼ�з���ָ���Ĺ�Ա���ṩ�����գ�����ְ��Ͳ������ƣ��ô洢���̽����봫�ݵĲ�����ȷƥ���ֵ*/
--����
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
--ִ��
EXEC GetEmployees 'Walters', 'Rob'
GO
/*����*/
EXEC GetEmployees @lastname = 'Walters', @firstname = 'Rob'
GO
/*����*/
EXEC GetEmployees @firstname = 'Rob', @lastname = 'Walters'
--ɾ��
DROP PROCEDURE dbo.GetEmployees


--�������Ĵ洢���� 
--�ڱ�person.contact�в��ҵ�һ������Ϊ@firstname�����һ������Ϊ@lastname
--LIKE���Բ���ͨ����ʽeg 'N%'
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

--��ѯ����
USE [AdventureWorks]
GO
DECLARE @return_value INT
EXEC @return_value = [dbo].[au_infor2] @lastname = N'Bl%', @firstname = N'Mi%'
SELECT  'Return Value' = @return_value
GO
/*����*/
USE [AdventureWorks]
GO
DECLARE @return_value INT
EXEC @return_value = [dbo].[au_infor2] @lastname = N'Blythe',
    @firstname = N'Michael'
SELECT  'Return Value' = @return_value
GO
--ɾ��
DROP PROCEDURE [dbo].[au_infor2]

/*��8-5 ����ʾ����ʾ��һ�����������һ����������Ĵ洢���̡�
�洢�����еĵ�һ������@sname�������ɵ��ó���ָ��������ֵ(ѧ������)��
�ڶ�������@sscore���ɼ��������ڽ���ֵ���ص��ó���
SELECT ���ʹ��@sname������ȡ��ȷ��	@sscoreֵ��������ֵ��������������*/
--�����Ӧ�� ������ϲ�ѯ
CREATE PROCEDURE s_score
    @sname CHAR(8) ,
    @sscore REAL OUTPUT
AS 
    SELECT  @sscore = score
    FROM    sc
            JOIN s ON s.sno = sc.sno
    WHERE   sn = @sname
GO



/*��8-9*/
/* ������һ����Ϊproc_person �Ĵ洢���̣��ô洢���̰���������Email��ַ��Ϣ��Ȼ����ALTER PROCEDURE���¶����˸ô洢���̣�ʹֻ֮����������Ϣ����ʹ��ENCRYPTION�ؼ���ʹ֮�޷�ͨ���鿴syscomments�����鿴�洢���̵����ݡ�*/
/*�����嵥����*/

USE adventureworks
GO
/*����һ���洢���̣��ô洢���̰���������Email��ַ��Ϣ*/
CREATE PROCEDURE proc_person
AS 
    SELECT  firstname ,
            lastname ,
            emailaddress
    FROM    person.contact
    ORDER BY lastname ,
            firstname
GO
/*�鿴�ô洢���̵�Դ���롣*/
SELECT  o.id ,
        c.text
FROM    sysobjects o
        INNER JOIN syscomments c ON o.id = c.id
WHERE   o.type = 'P'
        AND o.name = 'proc_person'
/*����Ըô洢���̽������¶��塣*/
ALTER PROCEDURE proc_person
    WITH ENCRYPTION
AS 
    SELECT  firstname ,
            lastname
    FROM    person.contact
    ORDER BY lastname ,
            firstname
GO
/*�鿴�ô洢���̵ľ���Դ���롣*/
SELECT  o.id ,
        c.text
FROM    sysobjects o
        INNER JOIN syscomments c ON o.id = c.id
WHERE   o.type = 'P'
        AND o.name = 'proc_person'
GO
