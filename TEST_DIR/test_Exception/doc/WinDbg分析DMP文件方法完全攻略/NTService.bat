@echo off
setlocal enabledelayedexpansion
set colsnum=60
set linesnum=22
MODE con: COLS=%colsnum% lines=%linesnum%
::设置服务名ServiceName
set ServiceName=IpLookupService.exe

::名称居中计算
set/a colsnum=%colsnum%-8
::echo以及其后的空格共占5列，实际不止5列
set myecho=根据需要处理%ServiceName%服务
::将你要输出的字符串赋值给变量myecho
set flag=0
::初始化计数器flag
for /l %%i in (0,1,1000) do (
set tt=!myecho:~%%i,1!
if "!tt!" equ "" goto next
set/a flag=!flag!+1
)
::统计你输入的字符串长度
:next
set/a colsnum=!colsnum!-!flag!
set/a colsnum=!colsnum!/2
for /l %%i in (1,1,!colsnum!) do (
set space= !space!
)
::启动菜单面板
:MENUSTART
color 1a
set tm1=%time:~0,2%
set tm2=%time:~3,2%
set tm3=%time:~6,2%
title %ServiceName%处理
CLS
ECHO.
ECHO !space!%myecho%
ECHO.        
ECHO.        当前系统时间 %date% %tm1%点%TM2%分%TM3%秒
ECHO.
ECHO.       ═════控═══制═══命═══令═════
ECHO        [1] 启动%ServiceName%服务
ECHO. 
ECHO        [2] 安装%ServiceName%服务
ECHO. 
ECHO        [3] 重启%ServiceName%服务
ECHO.         
ECHO        [4] 删除%ServiceName%服务
ECHO.                 
ECHO        [5] 退出...
ECHO.       ═════输═══入═══数═══字═════
ECHO.
ECHO                -=◆◇QNA◇◆=- (V0.5)
ECHO.

set choice=
set /p choice=       请输入对应的数字后按[Enter]执行:
if not "%choice%"=="" set choice=%choice%
ECHO.
if /I "%choice%"=="1" Goto STARTPANEL
if /I "%choice%"=="2" Goto INSTALLPANEL
if /I "%choice%"=="3" Goto RESTARTPANEL
if /I "%choice%"=="4" Goto DELPANEL
if /I "%choice%"=="5" Goto EXITPANEL
if /I "%choice%" NEQ "5" Goto ERRORPANEL
goto MENUSTART

::启动服务面板
:STARTPANEL
CLS
title 启动%ServiceName%服务
echo 正在启动%ServiceName%服务... 
CALL %ServiceName% -d
CLS
goto MENUSTART

::安装服务面板
:INSTALLPANEL
CLS
title 安装%ServiceName%服务
echo 正在安装%ServiceName%服务... 
CALL %ServiceName% -i
CLS
goto MENUSTART

::停止服务面板
:STOPPANEL
CLS
title 停止%ServiceName%服务
echo 正在停止%ServiceName%服务，请稍候......
net stop %ServiceName% 
CLS
goto MENUSTART

::删除服务面板
:DELPANEL
CLS
title 删除%ServiceName%服务
echo 正在停止%ServiceName%服务... 
net stop %ServiceName% 
echo 正在删除%ServiceName%服务... 
sc delete %ServiceName% 
CALL %ServiceName% -u
CLS
goto MENUSTART

::重启服务面板
:RESTARTPANEL
CLS
title 重启%ServiceName%服务
echo 正在停止%ServiceName%服务，请稍候......
CALL %ServiceName% -u
echo 正在启动%ServiceName%服务... 
CALL %ServiceName% -i
CLS
goto MENUSTART

::输入错误面板
:ERRORPANEL
CLS
echo 请输入正确的数字...
timeout /t 5
goto MENUSTART

::退出面板
:EXITPANEL
cls
echo. 
echo.               服务管理V0.5版(QNA制作)
echo. 
echo ------------------------------------------------------------
echo 说明：这是这是一个指定服务管理的批处理，适用于已经装的服务支持服务启动、停止、删除；一般用于服务程序调适时期。
echo. 
echo 用法：打开本文件将第7行的%ServiceName%替换成自己需要管理的服务即可。
echo. 
echo 作者：QNA  
echo. 
echo 网址：http://www.9cpp.com/
echo ------------------------------------------------------------
echo. 
echo 5秒后退出,正在退出...
timeout /t 5
Exit
