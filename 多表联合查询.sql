    SELECT TOP 1
            ID ,
            Receiver ,
            Title ,
            URL ,
            IsUrgency
    FROM    dbo.Mgt_MessageForBar
    WHERE   IsRead IS NULL
            OR IsRead = 0;


select BarIP from Bar_BaseInfo where BarCode = '1101010000'

SELECT  ID ,
        BarIP ,
        Title ,
        URL ,
        IsUrgency
FROM    dbo.Bar_BaseInfo
        INNER JOIN dbo.Mgt_MessageForBar ON dbo.Bar_BaseInfo.BarCode = dbo.Mgt_MessageForBar.Receiver
WHERE   IsRead IS NULL
        OR IsRead = 0;

--两表、三表联合查询
--
--假设有表 A B C
--
--A表的字段 Aid Bid Cid Aname
--B表的字段 Bid Bname
--C表的字段 Cid Cname
--
--需要查询结果为
--
--Aid Bname Cname
---------------------
--
--sql 语句为：
--
--select Aid Bname Cname 
--from A left join B on A.Bid = B.Bid inner join C on A.Cid = C.Cid 
--where 其他条件
