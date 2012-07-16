set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go


-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
ALTER PROCEDURE [dbo].[dt_Save400Phone]
	-- Add the parameters for the stored procedure here
	@sPhone varchar(50),
	@sPass varchar(50)
AS
BEGIN
	if exists(select sAccount from tUser where sAccount = @sPhone) 
	begin
		update tUser set sPwd=@sPass, dtLastTime=getdate() where sAccount = @sPhone
	end
	else
	begin
		insert tUser(sAccount, sPwd) values(@sPhone, @sPass)
	end
END
