--���ұ�--
select *from IPSectionDispatch.dbo.tSmallIPSection
--��ձ�-
delete from IPSectionRcv.dbo.tIPDomain
--ɾ����--
DELETE FROM IPSectionDispatch.dbo.tSmallIPSection WHERE sIPBegin='61.172.201.194'
--������--
update IPSectionDispatch.dbo.tSmallIPSection set iStatus=0, iPercent=0 where sIPBegin='61.129.81.5';
select *from IPSectionDispatch.dbo.tSmallIPSection
DELETE FROM IPSectionDispatch.dbo.tSmallIPSection WHERE sIPBegin!='61.129.81.1'  --ɾ��������--
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



--������
create table test.dbo.a_dist(id int,name varchar(20))
insert into test.dbo.a_dist values(1,'abc')
insert into test.dbo.a_dist values(1,'123')
insert into test.dbo.a_dist values(1,'asd')
insert into test.dbo.a_dist values(1,'qwe')

select *from test.dbo.a_dist

--ɾ��һ�������ظ��ļ�¼
--row_numbenr() over(order by id, name) rn
-- ����һ�����ݿ���������������ǣ�����id, name ����order by id, name�����г���¼����š�

--where rn <> 1 ʲô��˼
-- ���id��name��ͬ�Ļ���rn���1һֱ�ۼӣ��������Ҫɾ���ظ���¼��ֻҪ����rn=1�ļ��ɣ�����ɾ�� rn <> 1 �ļ�¼��
--create view a_dist_view as 
select a.*, row_number() over(order by id, name) rn from a_dist as a
delete from a_dist_view where rn <> 1











