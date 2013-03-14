--3.2 管理中心数据交互过程（02）  
--表名	Mgt_ManageCenterInfo
--SQL语句
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

--3.3 场所信息数据交互过程（03） 
--表名	Bar_BaseInfo
--SQL语句	
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
        '101010101' AS BarStatus ,
        LicenseNum ,
        IssueOrg ,
        CONVERT(VARCHAR(32), LicenseExpiredData, 112) AS LicenseExpiredData
FROM    Bar_BaseInfo
WHERE   1 = 1

--3.4 场所顾客信息数据交互过程（04） 
--表名	Cus_CustomerInfo
--SQL语句	
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


--3.5 场所日志数据交互过程（05）  
--表名 Bar_CustomerLoginOutLog
--SQL语句
SELECT  BarCode ,
        CusBarCode ,
        CusName ,
        CAST(CardType AS VARCHAR) AS CardType ,
        CertNo ,
        IssueORG ,
        ( REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(32), LoginTime, 120), ' ',
                                  ''), '-', ''), ':', '') ) AS LoginTime ,
        ( REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(32), LogoutTime, 120), ' ',
                                  ''), '-', ''), ':', '') ) AS LogoutTime ,
        HostIP
FROM    Bar_CustomerLoginOutLog
WHERE   1 = 1

--3.6 场所告警数据交互过程（06）  
--表名 Mgt_TacticsMatchRecord	
--SQL语句
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

--3.7 场所处罚数据交互过程（07） 
--表名 Mgt_PunishNotes
--SQL语句
SELECT  BarCode ,
        CONVERT(VARCHAR(32), PunishDate, 112) AS PunishDate ,
        PunishResult ,
        CAST(PunishReason AS VARCHAR) AS PunishReason ,
        EmpName
FROM    Mgt_PunishNotes
WHERE   1 = 1

--3.8 网吧在线数据交互过程（08） 
--表名 Bar_BaseInfo Bar_BarCountStatus
--SQL语句
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

--3.9 网吧终端安装在线数据交互过程（09） 
--表名 Bar_TerminalInfo Bar_TerminalInfo
--SQL语句
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

--3.10 下发禁止网址数据过程（51） 
--表名 Dic_ForbidedWebs
--SQL语句
--说明 此处的参数'%s' 是由文化传过来的数据赋值的  注意对照数据库文档查看
INSERT  INTO Dic_ForbidedWebs
        ( SiteType ,
          SiteName ,
          URL ,
          SiteIP ,
          CreateDate
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s'
        )

--3.11 下发禁止游戏数据过程（52） 
--表名 
--SQL语句
INSERT  INTO Dic_ForbidedGames
        ( GameType ,
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

--3.12 读取禁止网址数据过程（53） 
--表名 Dic_ForbidedWebs
--SQL语句
SELECT  SiteType ,
        SiteName ,
        Url ,
        SiteIP ,
        CONVERT(VARCHAR(32), EditDate, 112) AS EditDate
FROM    Dic_ForbidedWebs
WHERE   DATEDIFF(dd, EditDate, GETDATE()) = 0



--3.13 读取禁止游戏数据过程（54） 
--表名 Dic_ForbidedGames
--SQL语句
SELECT  GameType ,
        GameCName ,
        GameFileName ,
        GameVersion ,
        GameSampling ,
        CONVERT(VARCHAR(32), EditDate, 112) AS EditDate
FROM    Dic_ForbidedGames
WHERE   DATEDIFF(dd, EditDate, GETDATE()) = 0


--3.14 网吧服务器当前状态数据交互过程（71） 
--表名 Bar_BaseInfo
--SQL语句
SELECT  BarCode ,
        BarName ,
        BarAddress ,
        ( CASE IsOnLine
            WHEN 1 THEN 'Y'
            ELSE 'N'
          END ) AS IsOnLine
FROM    Bar_BaseInfo
WHERE   1 = 1


--3.15 网吧机器当前状态数据交互过程（72） 
--表名 Bar_TerminalInfo Bar_BaseInfo
--SQL语句
SELECT  a.BarCode ,
        b.BarName ,
        b.BarAddress ,
        b.BarIP ,
        REPLACE(STR(a.HostNum, 10, 0), ' ', '0') AS HostNum ,
        a.HostIP ,
        ( CASE a.CurrentStatus
            WHEN 1 THEN 'Y'
            ELSE 'N'
          END ) AS CurrentStatus
FROM    Bar_TerminalInfo a
        INNER JOIN Bar_BaseInfo b ON a.BarCode = b.BarCode
WHERE   1 = 1


--3.16 客户端远程控制截屏交互过程（73） 
--表名 Mgt_RemoteControl
--SQL语句
SET nocount ON;
INSERT  INTO Mgt_RemoteControl
        ( ExeDate ,
          CommandType ,
          PlaceIP ,
          BarHostIP ,
          CommandReason ,
          [Desc] ,
          CommandSource ,
          Creator
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        );
SELECT  @@Identity AS ID


--3.17 客户端远程控制重启交互过程（74） 
--表名 Mgt_RemoteControl
--SQL语句
INSERT  INTO Mgt_RemoteControl
        ( ExeDate ,
          CommandType ,
          PlaceIP ,
          BarHostIP ,
          CommandReason ,
          [Desc] ,
          CommandSource ,
          Creator
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        )



--3.18 客户端远程控制关机交互过程（75） 
--表名 Mgt_RemoteControl
--SQL语句
INSERT  INTO Mgt_RemoteControl
        ( ExeDate ,
          CommandType ,
          PlaceIP ,
          BarHostIP ,
          CommandReason ,
          [Desc] ,
          CommandSource ,
          Creator
        )
VALUES  ( '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          '%s' ,
          0 ,
          0
        )



--3.20 省级向部级平台发送信息（92） 
--表名 Mgt_MessageForEmp
--SQL语句
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



--3.19 部级向省级平台发送信息（91）
--表名 Mgt_MessageForEmp
--SQL语句	
SELECT TOP 1
        ID ,
        Title ,
        [Content] ,
        ExtPath
FROM    Mgt_MessageForEmp
WHERE   Receiver = 0
        AND Sender != 0
        AND IsUpload = 0
ORDER BY ID 


--3.20 省级向部级平台发送信息（92） 
--表名 Mgt_MessageForEmp
--SQL语句	
SELECT TOP 1
        ID ,
        Title ,
        [Content] ,
        ExtPath
FROM    Mgt_MessageForEmp
WHERE   Receiver = 0
        AND Sender != 0
        AND IsUpload = 0
ORDER BY ID 