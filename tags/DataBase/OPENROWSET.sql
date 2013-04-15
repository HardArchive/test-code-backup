update %s set 
CaptureImg = a.CaptureImg
from (SELECT a.* FROM OPENROWSET( BULK '%s', FORMATFILE='%s\%s.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='%s') AS a) a,
%s AS b
where a.ID = b.ID

update V_FILE_REMOTECONTROL set 
 V_FILE_REMOTECONTROL.CaptureImg = a.CaptureImg
 from (SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130411\1604\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\V_FILE_REMOTECONTROL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130411\1604\Screenshot.rc.err') AS a) a
 where V_FILE_REMOTECONTROL.ID = a.ID
 
SELECT * FROM  V_FILE_REMOTECONTROL  WHERE  ID = 16

UPDATE V_FILE_REMOTECONTROL
SET    CaptureImg = NULL
WHERE  ID = 16
-- update T2 
--set 要更新的字段 = T1.对应的字段　--可以输入多个用逗号分开
--from T1 
--where T2.ID = T1.ID
 
  SELECT * FROM V_FILE_CUSTOMERINFO
  
 update V_FILE_REMOTECONTROL set 
 CaptureImg = a.CaptureImg
 from (SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130412\1656\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\V_FILE_REMOTECONTROL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130412\1656\Screenshot.rc.err') AS a) a
 where EXISTS (SELECT * FROM V_FILE_REMOTECONTROL AS b WHERE a.ID = b.ID)

 
WHERE NOT EXISTS (SELECT * FROM V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode AND a.HostIP = b.HostIP)
SELECT *
from (SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130412\1656\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\V_FILE_REMOTECONTROL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130412\1656\Screenshot.rc.err') AS a) a,
 V_FILE_REMOTECONTROL AS b



update V_FILE_REMOTECONTROL set 
CaptureImg = a.CaptureImg
from (SELECT a.* FROM OPENROWSET( BULK 'E:\Culture\Source\20130412\1801\Screenshot.rc', FORMATFILE='D:\Program Files\RainSoft09\Culture\bin\RSDataToDb\V_FILE_REMOTECONTROL.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE='E:\Culture\Error\20130412\1801\Screenshot.rc.err') AS a) a,
V_FILE_REMOTECONTROL AS b
where a.ID = b.ID



--IS OK
INSERT  INTO %s
(BarCode, HostIP, HostMac, HostName, HostNum, IsCamra, CurrentStatus, LastUpdate, DistrictID, CityID, ProvinceID, ClientVersion, BarCusName, BarCusCertNO, BarCusCertType, BarUserNO ,BarCusLoginTime)
SELECT * FROM (SELECT a.* FROM OPENROWSET(BULK '%s', FORMATFILE= '%s\%s.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE= '%s') AS a ) a
WHERE NOT EXISTS (SELECT * FROM V_FILE_TERMINALINFO AS b WHERE  a.BarCode = b.BarCode AND a.HostIP = b.HostIP)


UPDATE  %s SET
BarCode = a.BarCode, HostIP = a.HostIP, HostMac = a.HostMac, HostName = a.HostName, HostNum = a.HostNum, IsCamra = a.IsCamra, CurrentStatus = a.CurrentStatus, LastUpdate = a.LastUpdate, DistrictID = a.DistrictID, CityID = a.CityID, ProvinceID = a.ProvinceID, ClientVersion = a.ClientVersion, BarCusName = a.BarCusName, BarCusCertNO = a.BarCusCertNO, BarCusCertType = a.BarCusCertType, BarUserNO = a.BarUserNO, BarCusLoginTime=a.BarCusLoginTime
FROM (SELECT a.* FROM OPENROWSET(BULK '%s', FORMATFILE= '%s\%s.fmt', MAXERRORS=10000000, ROWS_PER_BATCH=10000000, ERRORFILE= '%s') AS a ) a , V_FILE_TERMINALINFO AS b
WHERE a.BarCode = b.BarCode AND a.HostIP = b.HostIP
