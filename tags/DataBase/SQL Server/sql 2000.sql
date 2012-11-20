--4.2.1  �������ݿ�
--��������SQL Server 2000��ѯ��������
--�ڵ����ġ�SQL ��ѯ�����������ڵġ���ѯ���༭�Ի��������봴�����ݿ��T-SQL��䣬��������£�
CREATE DATABASE book
On primary
(	name='book_data1',
filename='E:\DataBase\book_data1.mdf',
size=5MB,
maxsize=UNLIMITED,
filegrowth=10%), 
(	name='book_data2',
filename='E:\DataBase\book_data2.ndf',
size=3MB,
maxsize=50MB,
filegrowth=1MB),
filegroup bookgroup1
(	name='book_data3',
filename='E:\DataBase\book_data3.ndf',
size=5MB,
maxsize=UNLIMITED,
filegrowth=2MB),
(	name='book_data4',
filename='E:\DataBase\book_data4.ndf',
size=5MB,
maxsize=UNLIMITED,
filegrowth=2MB),
filegroup bookgroup2
(	name='book_data5',
filename='E:\DataBase\book_data5.ndf',
size=2MB,
maxsize=100MB,
filegrowth=15%)
Log on
(	name='book_log',
filename='E:\DataBase\book_log.ldf',
size=2MB,
maxsize=35MB,
filegrowth=1MB)
Go

--4.2.2  �޸����ݿ�
--��������ͨ��ALTER DATABASE����ڡ�ͼ�����ϵͳ�����ݿ������һ�������ļ���book_data12������ָ�����ʼ��СΪ3MB������С�������ƣ�����������Ϊ15%��

ALTER  DATABASE  book
ADD  FILE
(	NAME='book_data12',
FILENAME='E:\DataBase\book_data12.mdf',
SIZE=3MB,
MAXSIZE=UNLIMITED,
FILEGROWTH=15%)
GO

--������Ϊ��book�����ݿ�����һ����ΪTHIRD���ļ��飬��Ϊ���ļ���������������ļ��������ļ����ֱ��ǡ�book_data2���͡�book_data3�������С�book_data2�������ļ��ĳ�ʼ��С��10MB��������2MB����������С�������ƣ�����book_data3�������ļ���ʼ��С��12MB����12%����������СΪ120MB��
ALTER  DATABASE  book
ADD  FILEGROUP  THIRD
GO
ALTER  DATABASE  book
ADD  FILE
(	NAME='book_data2',
FILENAME='E:\DataBase\book_data2.ndf',
SIZE=10MB,
MAXSIZE=UNLIMITED,
FILEGROWTH=2MB),
(	NAME='book_data3',
FILENAME='E:\DataBase\book_data3.ndf',
SIZE=12MB,
MAXSIZE=120MB,
FILEGROWTH=12%)
TO  FILEGROUP  THIRD
GO

--�������ݿ��ļ�
ALTER  DATABASE  book
ADD  FILE
(	NAME='book_data2',
FILENAME='E:\DataBase\book_data2.ndf',
SIZE=10MB,
MAXSIZE=UNLIMITED,
FILEGROWTH=2MB)
GO

--�����ݿ⡰book���е��ļ���THIRDɾ����ע�ⱻɾ���ļ������Ϊ�գ����������κ������ļ���������������ļ����Ƚ������ļ�ɾ����
--�����ݿ���ɾ�������ļ�book_data2
ALTER  DATABASE  book
REMOVE  FILE  book_data2
GO
--�����ݿ���ɾ�������ļ�book_data3��
ALTER  DATABASE  book
REMOVE  FILE  book_data3
GO
--�����ݿ⡰book���е��ļ���THIRDɾ��
ALTER  DATABASE  book
REMOVE FILEGROUP THIRD
GO

--Ϊ���ݿ⡰book�����һ����־�ļ����������־�ļ���Ϊbook_log1������������־�ļ���ʼ��СΪ5MB������СΪ78MB����6MB������
ALTER  DATABASE  book
ADD  LOG  FILE 
(	NAME='book_log1',
FILENAME='E:\DataBase\book_log1.ldf',
SIZE=5MB,
MAXSIZE=78MB,
FILEGROWTH=6MB)
GO
--ɾ������������������־�ļ���
ALTER  DATABASE  book
REMOVE  FILE  book_log1
GO


--�޸����ݿ⡰book�������ԡ��������ļ�book_data2�ĳ�ʼ��С��Ϊ8MB������СΪ120MB����6MB������ע��ÿ��ֻ���޸������ļ���һ�����ԡ�
ALTER  DATABASE  book
MODIFY  FILE
(	NAME='book_data2',
SIZE=18MB)
GO
ALTER  DATABASE  book
MODIFY  FILE
(	NAME='book_data2',
MAXSIZE=120MB)
GO
ALTER  DATABASE  book
MODIFY  FILE
(	NAME='book_data2',
FILEGROWTH=6MB)
GO

--4.2.3  ɾ�����ݿ�
--���磬Ҫɾ�����ݿ⡰book����ʹ����������:
DROP  DATABASE  book

--4.3.2  �û��Զ�����������
--1.�û��Զ�����������
--�����ݿ⡰ͼ�����ϵͳ���У����������û��Զ�����������xlx1��xlx2��Ҫ�����Ƿֱ������������varchar��char��
--�ڲ�ѯ�������������������������䣺
USE book
EXEC sp_addtype MyType, 'char(32)', 'not null'
EXEC sp_addtype  xlx1,'varchar(16)','not null'
EXEC sp_addtype  xlx2,'char(24)','null'

--4.5.1  ����Լ��
--1.ͨ��һ��ʵ��ʵ������Լ��
--����һ����Ʒ��Ϣ���Բ�Ʒ��ź�����Ϊ������
USE book
CREATE TABLE products (
p_id char(8) NOT NULL,
p_name char(10) NOT NULL,
price money DEFAULT 0.01 ,
QUANTITY smallint NULL,
CONSTRAINT pk_p_id PRIMARY KEY (p_id, p_name)

) ON [PRIMARY]