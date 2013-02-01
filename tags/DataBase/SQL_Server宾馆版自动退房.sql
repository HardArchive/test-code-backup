--取到sDay不等于零的 sUserID
SELECT  sDay ,
        dtLastModify1 ,
        sUserID
FROM    dbo.tRoom
WHERE   sDay != 0

SELECT *
FROM dbo.tRoom

SELECT *
FROM dbo.tUser

150429800509501
--查询时间
SELECT  sUserID ,
        sDay
FROM    dbo.tRoom 

--插入tRoomUserInfo数据
INSERT  INTO dbo.tRoomUserInfo
        SELECT  sRoomID ,
                sRoomName ,
                dbo.tUser.sUserID ,
                dtValidEndTime = GETDATE() ,
                sIDNo ,
                sPassword ,
                sDay ,
                sIDCode ,
                sComputerKey ,
                sUserName
        FROM    dbo.tRoom ,
                dbo.tUser
        WHERE   dbo.tUser.sUserID = dbo.tRoom.sUserID
                AND dbo.tUser.sUserID = '6501021000998820130201105252_1'
--更新tRoomUserInfo数据
UPDATE  dbo.tRoomUserInfo
SET     dbo.tRoomUserInfo.sRoomID = dbo.tRoom.sRoomID ,
        dbo.tRoomUserInfo.sRoomName = dbo.tRoom.sRoomID ,
        dbo.tRoomUserInfo.sUserID = dbo.tUser.sUserID ,
        dbo.tRoomUserInfo.dtValidEndTime = GETDATE() ,
        dbo.tRoomUserInfo.sIDNo = dbo.tUser.sIDNo ,
        dbo.tRoomUserInfo.sPassword = dbo.tRoom.sPassword ,
        dbo.tRoomUserInfo.sDay = dbo.tRoom.sDay ,
        dbo.tRoomUserInfo.sIDCode = dbo.tUser.sIDCode ,
        dbo.tRoomUserInfo.sComputerKey = dbo.tRoom.sComputerKey ,
        dbo.tRoomUserInfo.sUserName = tuser.sUserName
FROM    dbo.tUser ,
        dbo.tRoom
WHERE   dbo.tUser.sUserID = '6501021000998820130201105252_1'
        AND dbo.tUser.sUserID = dbo.tRoom.sUserID


--取得下机数据    
SELECT  dbo.tComputer.sComputerIP ,
        dbo.tComputer.sComputerMac ,
        dbo.tComputer.sComputerNo ,
        dbo.tComputer.sComputerName ,
        dbo.tComputer.sKey ,
        dbo.tUser.sUserID ,
        sUserName ,
        sIDCode ,
        sIDNo ,
        sCardOutNum
FROM    dbo.tComputer ,
        dbo.tUser
WHERE   dbo.tUser.sUserID = dbo.tComputer.sUserID
        AND dbo.tUser.sUserID = '6501021000998820130129104027_1'


SELECT  dbo.tComputer.sComputerIP ,
        dbo.tComputer.sComputerMac ,
        dbo.tComputer.sComputerNo ,
        dbo.tComputer.sComputerName ,
        dbo.tComputer.sKey ,
        dbo.tUser.sUserID ,
        sUserName ,
        sIDCode ,
        sIDNo ,
        dbo.tComputer.bIsVip
FROM    dbo.tComputer
        INNER JOIN dbo.tUser ON dbo.tUser.sUserID = dbo.tComputer.sUserID
                                AND dbo.tUser.sUserID = '6501021000998820130129104027_1'



SELECT  dbo.tComputer.sComputerIP ,
        dbo.tComputer.sComputerMac ,
        dbo.tComputer.sComputerNo ,
        dbo.tComputer.sComputerName
FROM    dbo.tComputer

     
SELECT  dbo.tComputer.sComputerIP ,
        dbo.tComputer.sComputerMac ,
        dbo.tComputer.sComputerNo ,
        dbo.tComputer.sComputerName ,
        dbo.tComputer.sKey ,
        dbo.tUser.sUserID ,
        sUserName ,
        sIDCode ,
        sIDNo ,
        dbo.tComputer.bIsVip
FROM    dbo.tComputer
        INNER JOIN dbo.tUser ON dbo.tUser.sUserID = dbo.tComputer.sUserID
                                AND dbo.tUser.sUserID = '6501021000998820130129174616_1'
                                
--下线后清除表
UPDATE  dbo.tComputer
SET     bHasSent = 0 ,
        bHasSent1 = 0 ,
        sUserID = NULL ,
        sComputerIP = NULL ,
        sComputerMac = NULL ,
        bIsVip = 0 ,
        sAccount = NULL
WHERE   sUserID = '6501021000998820130129174616_1'

UPDATE  dbo.tUser
SET     bUserStatus = 0 ,
        dtLastModify = GETDATE() ,
        bHasSent = 0 ,
        bHasSent1 = 0 ,
        dtOfflineTime = GETDATE()
WHERE   sUserID = '6501021000998820130129174616_1'

UPDATE  dbo.tRoom
SET     dtLastModify = GETDATE() ,
        dtLastModify1 = GETDATE() ,
        bHasSent = 0 ,
        bHasSent1 = 0 ,
        sUserID = NULL ,
        sComputerKey = NULL ,
        sDay = 0
WHERE   sUserID = '6501021000998820130129174616_1'






SELECT  dbo.tComputer.sComputerIP ,
        dbo.tComputer.sComputerMac ,
        dbo.tComputer.sComputerNo ,
        dbo.tComputer.sComputerName ,
        dbo.tComputer.sKey ,
        dbo.tUser.sUserID ,
        sUserName ,
        sIDCode ,
        sIDNo ,
        dbo.tComputer.bIsVip
FROM    dbo.tComputer
        INNER JOIN dbo.tUser ON dbo.tUser.sUserID = dbo.tComputer.sUserID
                                AND dbo.tUser.sUserID = '6501021000998820130129174616_1'
                                
SELECT *FROM dbo.tUser WHERE dbo.tUser.sUserID = '6501021000998820130129174616_1'	
SELECT * FROM dbo.tComputer WHERE dbo.tComputer.sUserID = '6501021000998820130129174616_1'	


--自动下机后清除表
INSERT  INTO dbo.tUserInOutLog
        ( sUserID ,
          sUserName ,
          bSex ,
          sIDCode ,
          sIDNo ,
          dtLogTime ,
          sComputerIP ,
          sComputerName ,
          sComputerNo ,
          sComputerMac ,
          dtOfflineTime ,
          sAccount ,
          bAction ,
          iStatus ,
          bHasSent ,
          bHasSent1
        )
        SELECT  dbo.tUser.sUserID ,
                dbo.tUser.sUserName ,
                dbo.tUser.bSex ,
                dbo.tUser.sIDCode ,
                dbo.tUser.sIDNo ,
                dbo.tUser.dtLoginTime ,
                dbo.tComputer.sComputerIP ,
                dbo.tComputer.sComputerName ,
                dbo.tComputer.sComputerNo ,
                dbo.tComputer.sComputerMac ,
                GETDATE() ,
                dbo.tComputer.sAccount ,
                1 ,
                2 ,
                0 ,
                0
        FROM    dbo.tComputer ,
                dbo.tUser
        WHERE   dbo.tUser.sUserID = dbo.tComputer.sUserID
                AND dbo.tUser.sUserID = '6501021000998820130130154827_1'
        
SELECT * FROM dbo.tUserInOutLog WHERE sUserID = '6501021000998820130130171849_1'
DELETE dbo.tUserInOutLog WHERE sUserID = '6501021000998820130130171849_1'



--产生6们随机数
SELECT RIGHT(('000000'+cast(ceiling(rand() * 1000000) as VARCHAR(6))), 6) 
SELECT RIGHT(('000000'+cast(floor(rand() * 1000000) as VARCHAR(6))), 6) 

--随机数更新密码语句
 DECLARE @RandomNum VARCHAR(6) 
 SET @RandomNum = RIGHT(( '000000'
                          + CAST(CEILING(RAND() * 1000000) AS VARCHAR(6)) ), 6)  
 UPDATE dbo.tRoom
 SET    dbo.tRoom.sPassword = @RandomNum
 WHERE  dbo.tRoom.sPassword != @RandomNum
        AND dbo.tRoom.sRoomID = '6501021000998820130131162059_1'
        
 SELECT dbo.tRoom.sPassword
 FROM   dbo.tRoom
