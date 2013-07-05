--自动退房语句
--1 必执行
INSERT INTO tUserInOutLog
(sUserID, sUserName, bSex, sCardInNum, sCardOutNum, sCardType, dtCardRegTime, sIDCode, sIDNo, sIDUnit, sNationCode, dtLogTime, sComputerIP, sComputerName, sComputerNo, sComputerMac, bAction, bHasSent,  dtOfflineTime, iStatus, sAccount, bHasSent1)
	SELECT tUser.sUserID,sUserName, bSex, sCardInNum, sCardOutNum, sCardType, dtRegTime, sIDCode, sIDNo, sIDUnit, sNationCode, dtLoginTime, tComputer.sComputerIP, tComputer.sComputerName, tComputer.sComputerNo, tComputer.sComputerMac, 0, 0,  GETDATE(), 2, sAccount, 0
	FROM  tComputer,tUser
	WHERE tComputer.sUserID=tUser.sUserID
		AND (ISNULL(tComputer.bIsVip,0) <>1 AND  tComputer.dtActiveTime < DATEADD(HOUR,-2,GetDate()))

--2-1 m_dwUserRegisterMode<7执行
UPDATE  tRoom
SET     sComputerKey = '' ,
        sUserID = '' ,
        bHasSent = 0 ,
        bHasSent1 = 0
WHERE   sUserID IN ( SELECT sUserID
                     FROM   tUser
                     WHERE  dtLoginTime < DATEADD(HOUR, -2, GETDATE()))
                     
--2-2 m_dwUserRegisterMode>=7执行                     
UPDATE  tRoom
SET     sUserID = '' ,
        bHasSent = 0 ,
        bHasSent1 = 0
WHERE   sUserID IN ( SELECT sUserID
                     FROM   tUser
                     WHERE  dtLoginTime < DATEADD(HOUR, -2, GETDATE()) ) 

--3必执行
UPDATE  dbo.tComputer
SET     sUserID = '' ,
        bHasSent = 0 ,
        bHasSent1 = 0
WHERE   sUserID IN ( SELECT sUserID
                     FROM   tComputer
                     WHERE  dtActiveTime < DATEADD(HOUR, -2, GETDATE())
                            AND LEN(dbo.tComputer.sUserID) > 0 )


SELECT  *
FROM    tComputer
WHERE   dtActiveTime < DATEADD(HOUR, -2, GETDATE())
        AND LEN(dbo.tComputer.sUserID) > 0 