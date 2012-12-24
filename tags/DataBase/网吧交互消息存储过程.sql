USE [BJCultureSys]
GO
/****** 对象:  StoredProcedure [dbo].[GetMessageForBar]    脚本日期: 12/24/2012 17:22:52 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


ALTER PROCEDURE [dbo].[GetMessageForBar]
AS 
    SELECT  ID ,
            Title ,
            URL ,
            IsUrgency ,
            MessageType ,
            BarIP
    FROM    dbo.Bar_BaseInfo
            INNER JOIN dbo.Mgt_MessageForBar ON dbo.Bar_BaseInfo.BarCode = dbo.Mgt_MessageForBar.Receiver
    WHERE   IsRead IS NULL
            OR IsRead = 0;
    UPDATE  dbo.Mgt_MessageForBar
    SET     IsRead = 1
    WHERE   ID = ( SELECT TOP 1
                            ID
                   FROM     dbo.Mgt_MessageForBar
                   WHERE    IsRead IS NULL
                            OR IsRead = 0
                 );
		/*监控网吧交互消息通知表的IsRead值是否不为1，如果是则说明这条记录为新记录
	直接下发，然后将基标记为已下发*/
	
	