SELECT  RowType ,
        BarCode ,
        BarName ,
        Boss ,
        Manager ,
        NetAD ,
        BarAddress ,
        ManagerTel ,
        ADTel ,
        ServerQuantity ,
        '101010101' AS BarStatus ,
        LicenseNum ,
        IssueOrg ,
        CONVERT(VARCHAR(32), LicenseExpiredData, 112) AS LicenseExpiredData
FROM    Bar_BaseInfo
WHERE   1 = 1

SELECT  IsPermit ,
        IsOpened ,
        IsOnLine ,
        IsInstalled
FROM    Bar_BaseInfo


--STUFF删除指定长度的字符，并在指定的起点处插入另一组字符。
--STUFF ( character_expression , start , length ,character_expression )
--以下示例在第一个字符串 abcdef 中删除从第 2 个位置（字符 b）开始的三个字符，
--然后在删除的起始位置插入第二个字符串，从而创建并返回一个字符串
SELECT  STUFF('abcdef', 2, 3, 'ghijk')

--将数字转化为8位字符串
SELECT  CONVERT(CHAR(8), 20091001)

--合并4个整形为一个字符串
SELECT  CONVERT(CHAR(1), IsPermit) + CONVERT(CHAR(1), IsOpened)
        + CONVERT(CHAR(1), IsOnLine) + CONVERT(CHAR(1), IsInstalled)
FROM    Bar_BaseInfo


--将int型字段值转化成(0->00, 1->10)形式字符串并给其赋BarStatus字段别名
SELECT  RIGHT(CONVERT(CHAR(3), ( IsPermit * 10 + 100 )), 2) AS BarStatus
FROM    Bar_BaseInfo

--同上
SELECT  RIGHT(CAST(( IsPermit * 10 + 100 ) AS VARCHAR(3)), 2) AS BarStatus
FROM    Bar_BaseInfo

SELECT IsPermit FROM dbo.Bar_BaseInfo

--将一个表中的4个int型字段分别转化成(0->00, 1->10)格式再合并成一个字符串  然后在尾部加1
SELECT  RIGHT(CAST(( IsPermit * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOpened * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOnLine * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsInstalled * 10 + 100 ) AS VARCHAR(3)), 2) + '1' AS BarStatus2
FROM    Bar_BaseInfo 

--数字日期转换为字符日期并添加间隔符号
select stuff(stuff(convert(char(8),20091001),5,0,'-'),8,0,'-')


--合并两条SQL语句
SELECT  RowType ,
        BarCode ,
        BarName ,
        Boss ,
        Manager ,
        NetAD ,
        BarAddress ,
        ManagerTel ,
        ADTel ,
        ServerQuantity ,
        RIGHT(CAST(( IsPermit * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOpened * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOnLine * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsInstalled * 10 + 100 ) AS VARCHAR(3)), 2) + '1' AS BarStatus2 ,
        LicenseNum ,
        IssueOrg ,
        CONVERT(VARCHAR(32), LicenseExpiredData, 112) AS LicenseExpiredData
FROM    Bar_BaseInfo
WHERE   1 = 1

