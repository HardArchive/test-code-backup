 
--MAX ��������ʱ��
select  name,max(date) date From table group by name order by date
--DISTINCT ���ز�ͬ��Ψһֵ
SELECT DISTINCT Company FROM Orders 
insert into finally select (��ID������ֶ�) from customers_1 where id in (select mid from tmp)
select * from TB where ID in (select max(ID) from TB group by A)

SELECT * FROM dbo.Bar_TerminalInfo  
  
SELECT MAX(LastUpdate) FROM dbo.Bar_TerminalInfo

SELECT DISTINCT BarCode, HostIP FROM dbo.Bar_TerminalInfo

SELECT * FROM (SELECT ID, DISTINCT BarCode, HostIP FROM dbo.Bar_TerminalInfo AS a) a

--GROUP BY ������ڽ�Ϻϼƺ���������һ�������жԽ�������з��顣
--����������˼����  ��ѯBarCode, HostIP��ͬ�ļ�¼ ��������ID����IDֵ�г��� 
SELECT MAX(ID) FROM dbo.Bar_TerminalInfo GROUP BY BarCode, HostIP
--��ѯ���е�ID��������һ����ѯ��¼�еļ�¼
SELECT * FROM Bar_TerminalInfo WHERE ID IN (SELECT MAX(ID) FROM dbo.Bar_TerminalInfo GROUP BY BarCode, HostIP)
DELETE FROM dbo.Bar_TerminalInfo WHERE BarCode = '1101010000'

SELECT * FROM dbo.Bar_TerminalInfo

SELECT * FROM Bar_TerminalInfo WHERE LastUpdate IN (
SELECT BarCode, HostIP, MAX(LastUpdate) FROM dbo.Bar_TerminalInfo GROUP BY BarCode, HostIP)

SELECT * FROM Bar_TerminalInfo WHERE BarCode = aBarcod, HostIP = aHostIP, LastUpdate= aLastUpdate
(SELECT BarCode AS aBarCode, HostIP AS aHostIP, MAX(LastUpdate)AS aLastUpdate FROM dbo.Bar_TerminalInfo GROUP BY BarCode, HostIP) 

SELECT * FROM Bar_TerminalInfo WHERE LastUpdate IN 
(SELECT MAX(LastUpdate)AS LastUpdate FROM dbo.Bar_TerminalInfo GROUP BY BarCode, HostIP)


UPDATE dbo.Bar_TerminalInfo SET  LastUpdate = NULL 


  SELECT * FROM dbo.Bar_TerminalInfo
  RealTerminalQuantity
  UPDATE  dbo.Bar_BaseInfo SET RealTerminalQuantity = '%d' WHERE BarCode = '%s'
 DELETE FROM dbo.Bar_TerminalInfo WHERE BarCode = '%s'; 