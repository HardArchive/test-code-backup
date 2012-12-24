USE [BJCultureSys]
GO
/****** 对象:  StoredProcedure [dbo].[GetBusinessControl]    脚本日期: 12/24/2012 17:21:48 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[GetBusinessControl]
AS 
    SELECT TOP 1
            *
    FROM    dbo.Mgt_BarBusinessControl
    WHERE   IsSuccess IS NULL
            OR IsSuccess = 0;
    UPDATE  dbo.Mgt_BarBusinessControl
    SET     IsSuccess = 1
    WHERE   BarCode = ( SELECT TOP 1
                                BarCode
                        FROM    dbo.Mgt_BarBusinessControl
                        WHERE   IsSuccess IS NULL
                                OR IsSuccess = 0
                      );
    /*由于数据库字段功用未明确暂时先这样写了，具体监控字段，该赋何值只能到时候再改了
    本存储过程的作用是监控经营控制命令表，读取新的监控命令下发*/