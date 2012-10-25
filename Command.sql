--查找表--
select *from IPSectionDispatch.dbo.tSmallIPSection
--清空表-
delete from IPSectionRcv.dbo.tIPDomain
--删除行--
DELETE FROM IPSectionDispatch.dbo.tSmallIPSection WHERE sIPBegin='61.172.201.194'
--更新行--
update IPSectionDispatch.dbo.tSmallIPSection set iStatus=0, iPercent=0 where sIPBegin='61.129.81.5';
select *from IPSectionDispatch.dbo.tSmallIPSection
DELETE FROM IPSectionDispatch.dbo.tSmallIPSection WHERE sIPBegin!='61.129.81.1'  --删除其他行--
61.129.81.5
select *from IPSectionRcv.dbo.tIPDomain
select *from IPSectionDispatch.dbo.tConfig

update from IPSectionDispatch.dbo.tSmallIPSection set iStatus = 0
select *from IPSectionDispatch.dbo.tConfig
select *from IPSectionDispatch.dbo.tSmallIPSection
select *from IPSectionRcv.dbo.tIPDomain
delete from IPSectionRcv.dbo.tIPDomain where sIPBegin='61.129.81.5'
--6	1	61.129.81.4	61.129.81.6	1031885060	1031885062	NULL	2012-09-12 15:34:27.313	2012-08-27 13:44:35.843	50	2	NULL	NULL	NULL	NULL


delete from NetDetective.dbo.tCompanyLog 
select *from NetDetective.dbo.tCompanyLog

select *from NetDetective.dbo.tCompanyLog


delete from NetDetective.dbo.tWebAccountType
select *from NetDetective.dbo.tWebAccountType



delete from NetDetective.dbo.tViolateAccess
select *from NetDetective.dbo.tViolateAccess


delete from NetDetective.dbo.tUser
select *from NetDetective.dbo.tUser
delete from NetDetective.dbo.tComputer
select *from NetDetective.dbo.tComputer
select *from NetDetective.dbo.tSystem



--创建表
create table test.dbo.a_dist(id int,name varchar(20))
insert into test.dbo.a_dist values(1,'abc')
insert into test.dbo.a_dist values(1,'123')
insert into test.dbo.a_dist values(1,'asd')
insert into test.dbo.a_dist values(1,'qwe')

select *from test.dbo.a_dist

--删除一个表中重复的记录
--row_numbenr() over(order by id, name) rn
-- 这是一个数据库分析函数，含义是：按照id, name 排序（order by id, name），列出记录的序号。

--where rn <> 1 什么意思
-- 如果id，name相同的话，rn会从1一直累加，所以如果要删除重复记录，只要保存rn=1的即可，所以删除 rn <> 1 的记录。
--create view a_dist_view as 
select a.*, row_number() over(order by id, name) rn from a_dist as a
delete from a_dist_view where rn <> 1











