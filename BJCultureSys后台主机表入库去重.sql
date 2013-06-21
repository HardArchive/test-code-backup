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

--从Bar_TerminalInfotmp表取出数据插入临时表#tmp  with(nolock)查询是不锁定表,从而达到提高查询速度的目的。
SELECT * INTO #tmp FROM Bar_TerminalInfotmp WITH ( NOLOCK )

--从临时表#tmp中取出BarCode, HostIP 不同的记录插入另一临时表#tmp3
SELECT * INTO #tmp3	FROM #tmp WHERE #tmp.ID IN 
(SELECT MAX(ID) AS ID FROM #tmp GROUP BY BarCode, HostIP)

--用临时表#tmp3数据更新Bar_TerminalInfo 只更新 BarCode, HostIP 存在的记录 
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

--用临时表#tmp3数据插入Bar_TerminalInfo 只插入 BarCode, HostIP 存在的记录 
INSERT INTO Bar_TerminalInfo( BarCode, HostIP, HostMac, HostName, HostNum, IsCamra, CurrentStatus, LastUpdate, ProvinceID, CityID, DistrictID, ClientVersion, BarCusName, BarCusCertNO, BarCusCertType, BarUserNO, BarCusLoginTime)
SELECT BarCode, HostIP, HostMac, HostName, HostNum, IsCamra, CurrentStatus, LastUpdate, ProvinceID, CityID, DistrictID, ClientVersion, BarCusName, BarCusCertNO, BarCusCertType, BarUserNO, BarCusLoginTime
	FROM #tmp3
	WHERE not exists(
		SELECT 1  FROM Bar_TerminalInfo 
		WHERE Bar_TerminalInfo.BarCode = #tmp3.BarCode AND Bar_TerminalInfo.HostIP = #tmp3.HostIP)

    --print 'INSERT INTO Bar_TerminalInfo';
--组合删除语句  删除 Bar_TerminalInfotmp表中已经插入更新的数据
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
	
--删除临时表
	DROP TABLE #tmp
	DROP TABLE #tmp3	
END



