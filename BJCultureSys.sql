SELECT GameType, GameCName, ProcessName, GameVersion, GameSampling,
convert(varchar(32),EditDate,112) as EditDate from  Dic_ForbidedGames 
where datediff(dd,EditDate,getdate())=0 


--getdate()取得当前日期--
--datediff(dd,EditDate,getdate())按日期比较返回两个日期之间相差的天数--

SELECT * FROM Dic_ForbidedGames

select GameType,GameCName,ProcessName,GameVersion,GameSampling,
				   convert(varchar(32),EditDate,112) as EditDate
				   from  Dic_ForbidedGames where datediff(dd,EditDate,getdate())=0
		
select * from  Dic_ForbidedGames where datediff(dd,EditDate,getdate())=0

select max(EditDate) as EditDate from Dic_ForbidedGames


select * from  Dic_ForbidedGames where datediff(dd,EditDate,getdate())=0


select max(EditDate) as EditDate from Dic_ForbidedGames