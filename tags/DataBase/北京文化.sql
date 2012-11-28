--创建获取消息存储过程
CREATE PROCEDURE GetMessageForBar
AS
	SELECT ID, Title, URL, IsUrgency
	FROM dbo.Mgt_MessageForBar
	WHERE IsRead is NULL OR IsRead = 0;
GO

--删除存储过程
drop procedure dbo.GetMessageForBar

--调用获取消息存储过程
USE [BJCultureSys]
GO
DECLARE	@return_value int
EXEC	@return_value = [dbo].[GetMessageForBar]
SELECT	'Return Value' = @return_value
GO


