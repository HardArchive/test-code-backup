--3.2 �����������ݽ������̣�02��  
--����	Mgt_ManageCenterInfo
--SQL���
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

--3.3 ������Ϣ���ݽ������̣�03�� 
--����	Bar_BaseInfo
--SQL���	
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

--3.4 �����˿���Ϣ���ݽ������̣�04�� 
--����	Cus_CustomerInfo
--SQL���	
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


--3.5 ������־���ݽ������̣�05��  
--���� Bar_CustomerLoginOutLog
--SQL���
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

--3.6 �����澯���ݽ������̣�06��  
--���� Mgt_TacticsMatchRecord	
--SQL���
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

--3.7 �����������ݽ������̣�07�� 
--���� Mgt_PunishNotes
--SQL���
SELECT  BarCode ,
        CONVERT(VARCHAR(32), PunishDate, 112) AS PunishDate ,
        PunishResult ,
        CAST(PunishReason AS VARCHAR) AS PunishReason ,
        EmpName
FROM    Mgt_PunishNotes
WHERE   1 = 1

--3.8 �����������ݽ������̣�08�� 
--���� Bar_BaseInfo Bar_BarCountStatus
--SQL���
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

--3.9 �����ն˰�װ�������ݽ������̣�09�� 
--���� Bar_TerminalInfo Bar_TerminalInfo
--SQL���
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

--3.10 �·���ֹ��ַ���ݹ��̣�51�� 
--���� Dic_ForbidedWebs
--SQL���
--˵�� �˴��Ĳ���'%s' �����Ļ������������ݸ�ֵ��  ע��������ݿ��ĵ��鿴
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

--3.11 �·���ֹ��Ϸ���ݹ��̣�52�� 
--���� 
--SQL���
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

--3.12 ��ȡ��ֹ��ַ���ݹ��̣�53�� 
--���� Dic_ForbidedWebs
--SQL���
SELECT  SiteType ,
        SiteName ,
        Url ,
        SiteIP ,
        CONVERT(VARCHAR(32), EditDate, 112) AS EditDate
FROM    Dic_ForbidedWebs
WHERE   DATEDIFF(dd, EditDate, GETDATE()) = 0



--3.13 ��ȡ��ֹ��Ϸ���ݹ��̣�54�� 
--���� Dic_ForbidedGames
--SQL���
SELECT  GameType ,
        GameCName ,
        GameFileName ,
        GameVersion ,
        GameSampling ,
        CONVERT(VARCHAR(32), EditDate, 112) AS EditDate
FROM    Dic_ForbidedGames
WHERE   DATEDIFF(dd, EditDate, GETDATE()) = 0


--3.14 ���ɷ�������ǰ״̬���ݽ������̣�71�� 
--���� Bar_BaseInfo
--SQL���
SELECT  BarCode ,
        BarName ,
        BarAddress ,
        ( CASE IsOnLine
            WHEN 1 THEN 'Y'
            ELSE 'N'
          END ) AS IsOnLine
FROM    Bar_BaseInfo
WHERE   1 = 1


--3.15 ���ɻ�����ǰ״̬���ݽ������̣�72�� 
--���� Bar_TerminalInfo Bar_BaseInfo
--SQL���
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


--3.16 �ͻ���Զ�̿��ƽ����������̣�73�� 
--���� Mgt_RemoteControl
--SQL���
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


--3.17 �ͻ���Զ�̿��������������̣�74�� 
--���� Mgt_RemoteControl
--SQL���
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



--3.18 �ͻ���Զ�̿��ƹػ��������̣�75�� 
--���� Mgt_RemoteControl
--SQL���
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



--3.20 ʡ���򲿼�ƽ̨������Ϣ��92�� 
--���� Mgt_MessageForEmp
--SQL���
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



--3.19 ������ʡ��ƽ̨������Ϣ��91��
--���� Mgt_MessageForEmp
--SQL���	
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


--3.20 ʡ���򲿼�ƽ̨������Ϣ��92�� 
--���� Mgt_MessageForEmp
--SQL���	
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