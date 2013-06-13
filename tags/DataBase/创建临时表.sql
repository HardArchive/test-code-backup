SELECT EditDate FROM dbo.Dic_ForbidedGames

SELECT * FROM between '20130529' and  CONVERT(datetime,'20130529 23:59:59')


INSERT  INTO %s
SELECT * FROM (SELECT a.* FROM OPENROWSET(BULK '%s', FORMATFILE= '%s\%s.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE= '%s') AS a ) a
WHERE NOT EXISTS (SELECT * FROM V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode AND a.HostIP = b.HostIP)


INSERT  INTO V_FILE_TERMINALINFOALL
SELECT * FROM (SELECT a.* FROM OPENROWSET(BULK 'D:\Culture\Source\20130606\0850\BarterminalInfoAll.btia', FORMATFILE= 'd:\bin\datatodb\V_FILE_TERMINALINFOALL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE= 'D:\Culture\Error\20130606\0850\BarterminalInfoAll.btia.err') AS a ) a
WHERE NOT EXISTS (SELECT * FROM V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode AND a.HostIP = b.HostIP)

[3904] ValueToIPZero IPValue:a8c0; IP:192.168.30.81!!! 
[3904] ^&^!����������Ϣ ������Ϣ���� stuTerminailAck.iCount:13 sizeof(BARTERMINALINFO):312 dWlen:4056������ 

V_FILE_TERMINALINFOALL
TerMinalInfoAll

INSERT  INTO %s
SELECT * FROM (SELECT a.* FROM OPENROWSET(BULK '%s', FORMATFILE= '%s\%s.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE= '%s') AS a ) a
WHERE NOT EXISTS (SELECT * FROM V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode AND a.HostIP = b.HostIP)


select sum(case t when 1 then 1 else 0 end) counttab1, sum(case t when 2 then 1 else 0 end) counttab2,
from (SELECT * FROM (SELECT *,1 as t FROM TAB1 UNION ALL SELECT *,2 FROM TAB2)) TAB3

DECLARE @BulkFIle VARCHAR(128)
DECLARE @FormatFile VARCHAR(128)
DECLARE @ErrorFile VARCHAR(128)
SET @BulkFIle = 'D:\Culture\Source\20130606\0850\BarterminalInfoAll.btia'
SET @BulkFIle = 'd:\bin\datatodb\V_FILE_TERMINALINFOALL.fmt'
SET @BulkFIle = 'D:\Culture\Error\20130606\0850\BarterminalInfoAll.btia.err'
PRINT @BulkFIle

--���£�SELECT * FROM (SELECT * FROM TAB1 UNION ALL SELECT * FROM TAB2)AS TAB3 �����ʾ�ڱ����
--�ٶ�����Ľ��������TAB3��ѯ���Ƿ���У�
--SELECT COUNT(*) FROM TAB3
--PS������Ҫ�Ľ�������������������յ�SELECT COUNT(*) FROM TAB3
--���µĽ���Ͳ��ûظ���
--SELECT COUNT��*�� FROM (SELECT * FROM TAB1 UNION ALL SELECT * FROM TAB2)AS TAB3

--TAB3�������ѯ���û�ˣ����洢
--��������ʱ��ﵽЧ��,�����ñ����ƣ�����ʱ��洢�� tempdb �У�������ʹ��ʱ���Զ�ɾ����
--SELECT * into #t1 FROM TAB1 UNION ALL SELECT * FROM TAB2



select * from dbo.V_FILE_TERMINALINFOALL 
DELETE FROM dbo.V_FILE_TERMINALINFOALL 

TemTerminalInfoAll

--������ʱ�� 
SELECT  * INTO    #TemTerminalInfoAll
FROM    OPENROWSET(BULK 'D:\Culture\Source\20130606\0850\BarterminalInfoAll.btia',
                   FORMATFILE= 'd:\bin\datatodb\V_FILE_TERMINALINFOALL.fmt',
                   MAXERRORS=10000000, ROWS_PER_BATCH=10000000,
                   ERRORFILE= 'D:\Culture\Error\20130606\0850\BarterminalInfoAll.btia.err')
        AS a
DELETE  FROM V_FILE_TERMINALINFOALL 
SELECT  * FROM    #TemTerminalInfoAll AS a
WHERE   EXISTS ( SELECT *  FROM   V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode )

INSERT  INTO V_FILE_TERMINALINFOALL  
SELECT  * FROM    #TemTerminalInfoAll AS a
--WHERE NOT EXISTS (SELECT * FROM V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode AND a.HostIP = b.HostIP)

IF OBJECT_ID('tempdb..#TemTerminalInfoAll') IS NOT NULL 
BEGIN DROP TABLE #TemTerminalInfoAll END

select * from #TemTerminalInfoAll