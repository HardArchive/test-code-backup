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


--STUFFɾ��ָ�����ȵ��ַ�������ָ������㴦������һ���ַ���
--STUFF ( character_expression , start , length ,character_expression )
--����ʾ���ڵ�һ���ַ��� abcdef ��ɾ���ӵ� 2 ��λ�ã��ַ� b����ʼ�������ַ���
--Ȼ����ɾ������ʼλ�ò���ڶ����ַ������Ӷ�����������һ���ַ���
SELECT  STUFF('abcdef', 2, 3, 'ghijk')

--������ת��Ϊ8λ�ַ���
SELECT  CONVERT(CHAR(8), 20091001)

--�ϲ�4������Ϊһ���ַ���
SELECT  CONVERT(CHAR(1), IsPermit) + CONVERT(CHAR(1), IsOpened)
        + CONVERT(CHAR(1), IsOnLine) + CONVERT(CHAR(1), IsInstalled)
FROM    Bar_BaseInfo


--��int���ֶ�ֵת����(0->00, 1->10)��ʽ�ַ��������丳BarStatus�ֶα���
SELECT  RIGHT(CONVERT(CHAR(3), ( IsPermit * 10 + 100 )), 2) AS BarStatus
FROM    Bar_BaseInfo

--ͬ��
SELECT  RIGHT(CAST(( IsPermit * 10 + 100 ) AS VARCHAR(3)), 2) AS BarStatus
FROM    Bar_BaseInfo

SELECT IsPermit FROM dbo.Bar_BaseInfo

--��һ�����е�4��int���ֶηֱ�ת����(0->00, 1->10)��ʽ�ٺϲ���һ���ַ���  Ȼ����β����1
SELECT  RIGHT(CAST(( IsPermit * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOpened * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsOnLine * 10 + 100 ) AS VARCHAR(3)), 2)
        + RIGHT(CAST(( IsInstalled * 10 + 100 ) AS VARCHAR(3)), 2) + '1' AS BarStatus2
FROM    Bar_BaseInfo 

--��������ת��Ϊ�ַ����ڲ���Ӽ������
select stuff(stuff(convert(char(8),20091001),5,0,'-'),8,0,'-')


--�ϲ�����SQL���
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

