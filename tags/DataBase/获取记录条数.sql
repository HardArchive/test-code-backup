select rows from sysindexes where id = object_id(BJCultureSys.Dic_ForbidedWebs) and indid in (0,1) 

--获取记录条数
SELECT COUNT(*) AS RecordNum FROM dbo.Dic_ForbidedGames

SELECT COUNT(*) AS RecordCount FROM dbo.Dic_ForbidedWebs

SELECT COUNT(*) AS RecordCount FROM dbo.Dic_ForbidedWebs WHERE HarmType = 3
SELECT * FROM dbo.Dic_ForbidedWebs WHERE HarmType = 3