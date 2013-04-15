--02管理中心数据
SELECT  OperateType ,
        OrgCode ,
        OrgName ,
        OrgAddress ,
        ZipCode ,
        Manager ,
        Contactor ,
        Tel ,
        WebSite ,
        Email ,
        CAST(IsOnline AS VARCHAR) AS IsOnline
FROM    Mgt_ManageCenterInfo

--03场所信息数据
SELECT  RowType ,
        BarCode ,
        BarName ,
        Boss ,
        Manager ,
        NetAD ,
        BarAddress ,
        ManagerTel ,
        ADTel ,
        ServerQuantity ,
        RIGHT(CAST(( IsPermit * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOpened * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOnLine * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsInstalled * 10 + 100 ) AS VARCHAR(3)), 2) + '1' AS BarStatus2 ,
        LicenseNum ,
        IssueOrg ,
        CONVERT(VARCHAR(32), LicenseExpiredData, 112) AS LicenseExpiredData
FROM    Bar_BaseInfo
WHERE   1 = 1

--04场所顾客信息数据
SELECT  BarCode ,
        UserBarCode ,
        CusName ,
        CAST(CertType AS VARCHAR) AS CertType ,
        CertNo ,
        IssueOrg ,
        CusOfficeName ,
        Country ,
        InfoDesc
FROM    Cus_CustomerInfo
WHERE   1 = 1

--05场所日志数据
SELECT  BarCode ,
        CusBarCode ,
        CusName ,
        CAST(CardType AS VARCHAR) AS CardType ,
        CertNo ,
        \ IssueORG ,
        ( REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(32), LoginTime, 120), ' ',
                                  ''), '-', ''), ':', '') ) AS LoginTime ,
        ( REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(32), LogoutTime, 120), ' ',
                                  ''), '-', ''), ':', '') ) AS LogoutTime ,
        HostIP
FROM    Bar_CustomerLoginOutLog
WHERE   1 = 1

--06场所告警数据
SELECT  BarCode ,
        ( REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(32), CreateDate, 120), ' ',
                                  ''), '-', ''), ':', '') ) AS CreateDate ,
        BarHostIP ,
        UserBarCode ,
        CusName ,
        CusCertType ,
        CusCertNO ,
        IssueOrg ,
        AlarmType ,
        AlarmDesc
FROM    Mgt_TacticsMatchRecord
WHERE   1 = 1

--07场所处罚数据
SELECT  BarCode ,
        CONVERT(VARCHAR(32), PunishDate, 112) AS PunishDate ,
        PunishResult ,
        CAST(PunishReason AS VARCHAR) AS PunishReason ,
        EmpName
FROM    Mgt_PunishNotes
WHERE   1 = 1

--08网吧在线数据
SELECT  AreaLevel ,
        ProvCode ,
        CityCode ,
        CounCode ,
        REPLACE(STR(BarCount, 10, 0), ' ', '0') AS BarCount ,
        REPLACE(STR(OnlineCount, 10, 0), ' ', '0') AS OnlineCount ,
        REPLACE(STR(( BarCount - OnlineCount ), 10, 0), ' ', '0') AS OfflineCount ,
        REPLACE(STR(( CASE BarCount
                        WHEN 0 THEN 0
                        ELSE OnlineCount * 1.0 / BarCount
                      END ), 4, 1), ' ', '0') + '%' AS OnlinePer
FROM    ( SELECT    '02' AS AreaLevel ,
                    '01' AS ProvCode ,
                    '00' AS CityCode ,
                    '00' AS CounCode ,
                    ( SELECT    COUNT(*)
                      FROM      Bar_BaseInfo
                    ) AS BarCount ,
                    ( SELECT    COUNT(*)
                      FROM      Bar_BarCountStatus
                      WHERE     1 = 1
                    ) OnlineCount
        ) AS t
        
--09网吧终端安装在线数据
SELECT  AreaLevel ,
        ProvCode ,
        CityCode ,
        CounCode ,
        REPLACE(STR(ServerCount, 10, 0), ' ', '0') AS ServerCount ,
        ServerCount AS SoftCount ,
        '100.%' AS SoftInstallPer ,
        '100.%' AS SoftEffInstallPer ,
        REPLACE(STR(OnlineCusCount, 10, 0), ' ', '0') AS OnlineCusCount ,
        REPLACE(STR(( CASE ServerCount
                        WHEN 0 THEN 0
                        ELSE OnlineCusCount * 1.0 / ServerCount
                      END ), 4, 1), ' ', '0') + '%' AS OnlineCusPer
FROM    ( SELECT    '02' AS AreaLevel ,
                    '01' AS ProvCode ,
                    '00' AS CityCode ,
                    '00' AS CounCode ,
                    ( SELECT    COUNT(*)
                      FROM      Bar_TerminalInfo
                    ) AS ServerCount ,
                    ( SELECT    COUNT(*)
                      FROM      Bar_TerminalInfo
                      WHERE     CurrentStatus = 1
                    ) OnlineCusCount
        ) AS t        

--51下发禁止网址数据
INSERT  INTO Dic_ForbidedWebs
        ( SiteType ,
          SiteName ,
          URL ,
          SiteIP ,
          CreateDate ,
          AlarmType
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '00000003'
        )
        
--52下发禁址游戏数据
INSERT  INTO Dic_ForbidedGames
        ( AlarmType ,
          GameCName ,
          ProcessName ,
          GameVersion ,
          GameSampling ,
          CreateDate
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s'
        )
        
 --53读取禁止网址数据
 SELECT SiteType ,
        SiteName ,
        Url ,
        SiteIP ,
        CONVERT(VARCHAR(32), EditDate, 112) AS EditDate
 FROM   Dic_ForbidedWebs
 WHERE  DATEDIFF(dd, EditDate, GETDATE()) = 0
 
 --54读取禁止游戏数据
 SELECT GameType ,
        GameCName ,
        GameFileName ,
        GameVersion ,
        GameSampling ,
        CONVERT(VARCHAR(32), EditDate, 112) AS EditDate
 FROM   Dic_ForbidedGames
 WHERE  DATEDIFF(dd, EditDate, GETDATE()) = 0
 
 --网吧服务器当前状态数据
 SELECT BarCode ,
        BarName ,
        BarAddress ,
        ( CASE IsOnLine
            WHEN 1 THEN 'Y'
            ELSE 'N'
          END ) AS IsOnLine
 FROM   Bar_BaseInfo
 WHERE  1 = 1
 
 --72网吧机器当前状态数据
 SELECT a.BarCode ,
        b.BarName ,
        b.BarAddress ,
        b.BarIP ,
        REPLACE(STR(a.HostNum, 10, 0), ' ', '0') AS HostNum ,
        a.HostIP ,
        ( CASE a.CurrentStatus
            WHEN 1 THEN 'Y'
            ELSE 'N'
          END ) AS CurrentStatus
 FROM   Bar_TerminalInfo a
        INNER JOIN Bar_BaseInfo b ON a.BarCode = b.BarCode
 WHERE  1 = 1
					
 --73远程控制截屏
 SET nocount ON;
 INSERT INTO Mgt_RemoteControl
        ( ExeDate ,
          CommandType ,
          PlaceIP ,
          BarHostIP ,
          CommandReason ,
          [Desc] ,
          CommandSource ,
          Creator
        )
 VALUES ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        );
 SELECT @@Identity AS ID
 
 --74远程控制重启
 INSERT INTO Mgt_RemoteControl
        ( ExeDate ,
          CommandType ,
          PlaceIP ,
          BarHostIP ,
          CommandReason ,
          [Desc] ,
          CommandSource ,
          Creator
        )
 VALUES ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        )
        
 --75远程控制关机
 INSERT INTO Mgt_RemoteControl
        ( ExeDate ,
          CommandType ,
          PlaceIP ,
          BarHostIP ,
          CommandReason ,
          [Desc] ,
          CommandSource ,
          Creator
        )
 VALUES ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        )
        
--91部级下发信息
INSERT  INTO Mgt_MessageForEmp
        ( Title ,
          [Content] ,
          ExtPath ,
          SendDate ,
          Sender ,
          Receiver
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        )

--92