INSERT dbo.test
SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130411\1604\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\V_FILE_REMOTECONTROL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130411\1604\Screenshot.rc.err') AS a

UPDATE  V_FILE_REMOTECONTROL
SET     V_FILE_REMOTECONTROL.CaptureImg = test.CaptureImg
FROM    dbo.test
WHERE   V_FILE_REMOTECONTROL.ID = test.ID

SELECT * FROM dbo.test
SELECT * FROM V_FILE_REMOTECONTROL
--%s
INSERT dbo.test
SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130415\1727\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\dbo.test.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130415\1727\Screenshot.rc.err') AS a
UPDATE  V_FILE_REMOTECONTROL
SET     V_FILE_REMOTECONTROL.CaptureImg = test.CaptureImg
FROM    dbo.test
WHERE   V_FILE_REMOTECONTROL.ID = test.ID


--��ձ�
DELETE FROM dbo.test
TRUNCATE TABLE dbo.test

UPDATE  V_FILE_REMOTECONTROL
SET     V_FILE_REMOTECONTROL.CaptureImg = NULL


--������� + �����ļ����ѯ
INSERT dbo.test
SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130411\1604\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\V_FILE_REMOTECONTROL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130411\1604\Screenshot.rc.err') AS a



INSERT %s
SELECT a.* FROM OPENROWSET( BULK '%s', FORMATFILE='%s\%s.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='%s') AS a

--���ر����Զ�����ӷ�������ͼ
UPDATE  V_FILE_REMOTECONTROL
SET     V_FILE_REMOTECONTROL.CaptureImg = test.CaptureImg
FROM    dbo.test
WHERE   V_FILE_REMOTECONTROL.ID = test.ID

