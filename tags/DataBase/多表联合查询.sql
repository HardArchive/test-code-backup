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

--�����������ϲ�ѯ
--
--�����б� A B C
--
--A����ֶ� Aid Bid Cid Aname
--B����ֶ� Bid Bname
--C����ֶ� Cid Cname
--
--��Ҫ��ѯ���Ϊ
--
--Aid Bname Cname
---------------------
--
--sql ���Ϊ��
--
--select Aid Bname Cname 
--from A left join B on A.Bid = B.Bid inner join C on A.Cid = C.Cid 
--where ��������
