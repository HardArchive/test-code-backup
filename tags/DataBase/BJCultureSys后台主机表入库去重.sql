USE [BJCultureSys]
GO
/****** Object:  StoredProcedure [dbo].[job_TransBar_TerminalInfo]    Script Date: 06/19/2013 15:00:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER  proc [dbo].[job_TransBar_TerminalInfo]
as
BEGIN

--��Bar_TerminalInfotmp��ȡ�����ݲ�����ʱ��#tmp  with(nolock)��ѯ�ǲ�������,�Ӷ��ﵽ��߲�ѯ�ٶȵ�Ŀ�ġ�
SELECT * INTO #tmp FROM Bar_TerminalInfotmp WITH ( NOLOCK )

--����ʱ��#tmp��ȡ��BarCode, HostIP ��ͬ�ļ�¼������һ��ʱ��#tmp3
SELECT * INTO #tmp3	FROM #tmp WHERE #tmp.ID IN 
(SELECT MAX(ID) AS ID FROM #tmp GROUP BY BarCode, HostIP)

--����ʱ��#tmp3���ݸ���Bar_TerminalInfo ֻ���� BarCode, HostIP ���ڵļ�¼ 
UPDATE  Bar_TerminalInfo
SET     HostMac = #tmp3.HostMac ,
        HostName = #tmp3.HostName ,
        HostNum = #tmp3.HostNum ,
        IsCamra = #tmp3.IsCamra ,
        CurrentStatus = #tmp3.CurrentStatus ,
        LastUpdate = #tmp3.LastUpdate ,
        ProvinceID = #tmp3.ProvinceID ,
        CityID = #tmp3.CityID ,
        DistrictID = #tmp3.DistrictID ,
        ClientVersion = #tmp3.ClientVersion ,
        BarCusName = #tmp3.BarCusName ,
        BarCusCertNO = #tmp3.BarCusCertNO ,
        BarCusCertType = #tmp3.BarCusCertType ,
        BarUserNO = #tmp3.BarUserNO ,
        BarCusLoginTime = #tmp3.BarCusLoginTime
FROM    #tmp3
WHERE   Bar_TerminalInfo.BarCode = #tmp3.BarCode
        AND Bar_TerminalInfo.HostIP = #tmp3.HostIP

--����ʱ��#tmp3���ݲ���Bar_TerminalInfo ֻ���� BarCode, HostIP ���ڵļ�¼ 
INSERT INTO Bar_TerminalInfo( BarCode, HostIP, HostMac, HostName, HostNum, IsCamra, CurrentStatus, LastUpdate, ProvinceID, CityID, DistrictID, ClientVersion, BarCusName, BarCusCertNO, BarCusCertType, BarUserNO, BarCusLoginTime)
SELECT BarCode, HostIP, HostMac, HostName, HostNum, IsCamra, CurrentStatus, LastUpdate, ProvinceID, CityID, DistrictID, ClientVersion, BarCusName, BarCusCertNO, BarCusCertType, BarUserNO, BarCusLoginTime
	FROM #tmp3
	WHERE not exists(
		SELECT 1  FROM Bar_TerminalInfo 
		WHERE Bar_TerminalInfo.BarCode = #tmp3.BarCode AND Bar_TerminalInfo.HostIP = #tmp3.HostIP)

    --print 'INSERT INTO Bar_TerminalInfo';
--���ɾ�����  ɾ�� Bar_TerminalInfotmp�����Ѿ�������µ�����
    DECLARE @count INT
    DECLARE @maxid BIGINT
    DECLARE @minid BIGINT
    DECLARE @sql VARCHAR(5000)
	
	SELECT @maxid=max(ID) FROM #tmp
	SELECT @minid=min(ID) FROM #tmp
    SELECT @count=count(*) FROM #tmp
    SET @sql ='DELETE  TOP ('+ cast(@count as varchar(50)) +')
    FROM Bar_TerminalInfotmp
    WHERE id >=' + cast(@minid as varchar(50)) 
		+ ' AND id<=' +cast(@maxid as varchar(50)) 
	--PRINT @sql
	EXEC (@sql)
	
--ɾ����ʱ��
	DROP TABLE #tmp
	DROP TABLE #tmp3	
END



