--������ȡ��Ϣ�洢����
CREATE PROCEDURE GetMessageForBar
AS
	SELECT ID, Title, URL, IsUrgency
	FROM dbo.Mgt_MessageForBar
	WHERE IsRead is NULL OR IsRead = 0;
GO

--ɾ���洢����
drop procedure dbo.GetMessageForBar

--���û�ȡ��Ϣ�洢����
USE [BJCultureSys]
GO
DECLARE	@return_value int
EXEC	@return_value = [dbo].[GetMessageForBar]
SELECT	'Return Value' = @return_value
GO


