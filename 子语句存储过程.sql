USE [BJCultureSys]
GO
/****** 对象:  StoredProcedure [dbo].[GetRemoteControl]    脚本日期: 12/21/2012 17:45:18 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[GetRemoteControl]
AS 
    SELECT TOP 1
            ID ,
            CommandType ,
            BarHostIP ,
            PlaceIP
    FROM    dbo.Mgt_RemoteControl
    WHERE   IsSuccess IS NULL
            OR IsSuccess = 0;
    UPDATE  dbo.Mgt_RemoteControl
    SET     IsSuccess = 1
    WHERE   ID = ( SELECT TOP 1
                            ID
                   FROM     dbo.Mgt_RemoteControl
                   WHERE    IsSuccess IS NULL
                            OR IsSuccess = 0
                 );
		/*远程控制命令通知表的IsSuccess值是否不为1，如果是则说明这条记录为新记录
	直接下发，然后将基标记为已下发*/