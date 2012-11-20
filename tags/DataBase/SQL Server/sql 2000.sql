--4.2.1  创建数据库
--首先启动SQL Server 2000查询分析器，
--在弹出的【SQL 查询分析器】窗口的【查询】编辑对话框中输入创建数据库的T-SQL语句，其代码如下：
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

--4.2.2  修改数据库
--下面我们通过ALTER DATABASE命令，在“图书管理系统”数据库中添加一个数据文件“book_data12”，并指定其初始大小为3MB；最大大小不受限制，最设置增长为15%。

ALTER  DATABASE  book
ADD  FILE
(	NAME='book_data12',
FILENAME='E:\DataBase\book_data12.mdf',
SIZE=3MB,
MAXSIZE=UNLIMITED,
FILEGROWTH=15%)
GO

--接下来为“book”数据库增加一个名为THIRD的文件组，并为该文件组添加两个数据文件。数据文件名分别是“book_data2”和“book_data3”，其中“book_data2”数据文件的初始大小是10MB，并且以2MB增长，最大大小不受限制；而“book_data3”数据文件初始大小是12MB，以12%增长，最大大小为120MB。
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

--增加数据库文件
ALTER  DATABASE  book
ADD  FILE
(	NAME='book_data2',
FILENAME='E:\DataBase\book_data2.ndf',
SIZE=10MB,
MAXSIZE=UNLIMITED,
FILEGROWTH=2MB)
GO

--将数据库“book”中的文件组THIRD删除。注意被删除文件组必须为空，即不包括任何数据文件，如包含有数据文件则先将数据文件删除。
--从数据库中删除数据文件book_data2
ALTER  DATABASE  book
REMOVE  FILE  book_data2
GO
--从数据库中删除数据文件book_data3。
ALTER  DATABASE  book
REMOVE  FILE  book_data3
GO
--将数据库“book”中的文件组THIRD删除
ALTER  DATABASE  book
REMOVE FILEGROUP THIRD
GO

--为数据库“book”添加一个日志文件，所添加日志文件名为book_log1，并且所添日志文件初始大小为5MB，最大大小为78MB，按6MB增长。
ALTER  DATABASE  book
ADD  LOG  FILE 
(	NAME='book_log1',
FILENAME='E:\DataBase\book_log1.ldf',
SIZE=5MB,
MAXSIZE=78MB,
FILEGROWTH=6MB)
GO
--删除上例中所创建的日志文件。
ALTER  DATABASE  book
REMOVE  FILE  book_log1
GO


--修改数据库“book”的属性。将数据文件book_data2的初始大小改为8MB，最大大小为120MB，按6MB增长。注意每次只能修改数据文件的一个属性。
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

--4.2.3  删除数据库
--例如，要删除数据库“book”则使用以下命令:
DROP  DATABASE  book

--4.3.2  用户自定义数据类型
--1.用户自定义数据类型
--在数据库“图书管理系统”中，创建两个用户自定义数据类型xlx1和xlx2，要求他们分别基于数据类型varchar和char。
--在查询分析器的命令窗口中输入以下语句：
USE book
EXEC sp_addtype MyType, 'char(32)', 'not null'
EXEC sp_addtype  xlx1,'varchar(16)','not null'
EXEC sp_addtype  xlx2,'char(24)','null'

--4.5.1  主键约束
--1.通过一个实例实现主键约束
--创建一个产品信息表，以产品编号和名称为主键。
USE book
CREATE TABLE products (
p_id char(8) NOT NULL,
p_name char(10) NOT NULL,
price money DEFAULT 0.01 ,
QUANTITY smallint NULL,
CONSTRAINT pk_p_id PRIMARY KEY (p_id, p_name)

) ON [PRIMARY]