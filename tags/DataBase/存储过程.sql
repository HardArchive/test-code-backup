/*��8-1�� ����һ������SELECT���ļ򵥹��̣��ô洢���̷�������Ա��������Email��ַ���绰�ȡ��ô洢���̲�ʹ���κβ���*/
/*�����嵥����*/
USE adventureworks
GO
CREATE PROCEDURE au_infor_all
AS
SELECT lastname, firstname, emailaddress, phone
    FROM person.contact
GO

/*��8-3������һ�����в����ļ򵥴洢���̣�����ͼ�з���ָ���Ĺ�Ա���ṩ�����գ�����ְ��Ͳ������ƣ��ô洢���̽����봫�ݵĲ�����ȷƥ���ֵ*/
/*�����嵥����*/
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




--���ô洢����
USE [AdventureWorks]
GO
DECLARE	@return_value int
EXEC	@return_value = [dbo].[au_infor_all]
SELECT	'Return Value' = @return_value
GO

--ɾ���洢����
drop procedure [dbo].[au_infor_all]  


--�������Ĵ洢���� 
USE AdventureWorks;
GO
CREATE PROCEDURE GetEmployees --������
    @lastname varchar(40),    /*����1 ������2���Է���һ��*/
	@firstname varchar(20)    --����2
AS 
--����Ϊִ�����
    SELECT LastName, FirstName, JobTitle, Department
    FROM HumanResources.vEmployeeDepartment
    WHERE FirstName = @firstname AND LastName = @lastname;
GO

/*
--------------------�����﷨--------------------

һ.�����洢����
CREATE PROCEDURE procedure_name()
begin
.........
end

��.���ô洢����
1.�����﷨��call procedure_name()
ע�⣺�洢�������ƺ����������ţ����¸ô洢����û�в�������

��.ɾ���洢����
1.�����﷨��
drop procedure procedure_name//

2.ע������
(1)������һ���洢������ɾ����һ���洢���̣�ֻ�ܵ�����һ���洢����

��.������������

1.show procedure status
��ʾ���ݿ������д洢�Ĵ洢���̻�����Ϣ�������������ݿ⣬�洢�������ƣ�����ʱ���

2.show create procedure sp_name
��ʾĳһ��mysql�洢���̵���ϸ��Ϣ
*/