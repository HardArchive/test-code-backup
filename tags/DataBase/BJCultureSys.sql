SELECT GameType, GameCName, ProcessName, GameVersion, GameSampling,
convert(varchar(32),EditDate,112) as EditDate from  Dic_ForbidedGames 
where datediff(dd,EditDate,getdate())=0 


--getdate()ȡ�õ�ǰ����--
--datediff(dd,EditDate,getdate())�����ڱȽϷ�����������֮����������--

SELECT * FROM Dic_ForbidedGames

select GameType,GameCName,ProcessName,GameVersion,GameSampling,
				   convert(varchar(32),EditDate,112) as EditDate
				   from  Dic_ForbidedGames where datediff(dd,EditDate,getdate())=0
		
select * from  Dic_ForbidedGames where datediff(dd,EditDate,getdate())=0

select max(EditDate) as EditDate from Dic_ForbidedGames


select * from  Dic_ForbidedGames where datediff(dd,EditDate,getdate())=0


select max(EditDate) as EditDate from Dic_ForbidedGames