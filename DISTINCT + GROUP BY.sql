 
--MAX 返回最新时间
select  name,max(date) date From table group by name order by date
--DISTINCT 还回不同的唯一值
SELECT DISTINCT Company FROM Orders 
insert into finally select (除ID以外的字段) from customers_1 where id in (select mid from tmp)
select * from TB where ID in (select max(ID) from TB group by A)

SELECT * FROM dbo.Bar_TerminalInfo  
  
SELECT MAX(LastUpdate) FROM dbo.Bar_TerminalInfo

SELECT DISTINCT BarCode, HostIP FROM dbo.Bar_TerminalInfo

SELECT * FROM (SELECT ID, DISTINCT BarCode, HostIP FROM dbo.Bar_TerminalInfo AS a) a

--GROUP BY 语句用于结合合计函数，根据一个或多个列对结果集进行分组。
--以下语句的意思就是  查询BarCode, HostIP相同的记录 并把其中ID最大的ID值列出来 
SELECT MAX(ID) FROM dbo.Bar_TerminalInfo GROUP BY BarCode, HostIP
--查询表中的ID存在于上一个查询记录中的记录
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