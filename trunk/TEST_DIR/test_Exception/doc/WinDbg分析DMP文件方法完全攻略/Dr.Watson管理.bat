@echo off
setlocal enabledelayedexpansion
set colsnum=60
set linesnum=22
MODE con: COLS=%colsnum% lines=%linesnum%
::设置服务名ServiceName
set ProgramName=Dr.Watson

::名称居中计算
set/a colsnum=%colsnum%-8
::echo以及其后的空格共占5列，实际不止5列
set myecho=%ProgramName%调试程序管理
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
title %ProgramName%管理
CLS
ECHO.
ECHO !space!%myecho%
ECHO.        
ECHO.        当前系统时间 %date% %tm1%点%TM2%分%TM3%秒
ECHO.
ECHO.       ═════控═══制═══命═══令═════
ECHO        [1] %ProgramName%介绍
ECHO. 
ECHO        [2] %ProgramName%安装
ECHO. 
ECHO        [3] %ProgramName%设置
ECHO. 
ECHO        [4] %ProgramName%卸载
ECHO.         
ECHO        [5] 退出...
ECHO.       ═════输═══入═══数═══字═════
ECHO.
ECHO                 -=◆◇RG◇◆=- (V0.1)
ECHO.

set choice=
set /p choice=       请输入对应的数字后按[Enter]执行:
if not "%choice%"=="" set choice=%choice%
ECHO.
if /I "%choice%"=="1" Goto READMEPANEL
if /I "%choice%"=="2" Goto INSTALLPANEL
if /I "%choice%"=="3" Goto SETPANEL
if /I "%choice%"=="4" Goto UNINSTALLPANEL
if /I "%choice%"=="5" Goto EXITPANEL
if /I "%choice%" NEQ "5" Goto ERRORPANEL
goto MENUSTART

::说明面板
:READMEPANEL
CLS
title %ProgramName%介绍
echo.
echo ------------------------------------------------------------
echo Dr.Watson中文名华生医生．是一个程序错误调试程序。
echo. 
echo Dr.Watson会在应用程序崩溃的时候自动弹出，
echo 并且在默认情况下，它会将与出错有关的内存存为DUMP文件以供程序员分析。
echo ------------------------------------------------------------ 　
echo. 
pause
CLS
goto MENUSTART

::安装面板
:INSTALLPANEL
CLS
title 安装%ProgramName%
echo 正在安装%ProgramName%... 
CALL drwtsn32 -i
CLS
goto MENUSTART

::设置面板
:SETPANEL
CLS
title 设置%ProgramName%
echo 正在启动%ProgramName%设置... 
CALL drwtsn32
CLS
goto MENUSTART

::卸载面板
:UNINSTALLPANEL
CLS
title 卸载%ProgramName%
echo 正在卸载%ProgramName%... 
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug" /v "Auto" /t "REG_SZ" /d "0" /f
::/v键值名  /t类型 /d值 /f不提示
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
echo.               %ProgramName%管理V0.1版(RG制作)
echo. 
echo ------------------------------------------------------------
echo 说明：这是这是一个管理%ProgramName%的批处理，适用于%ProgramName%启用、设置、停用；一般用于程序调适时期。
echo. 
echo 用法：按照菜单说明输入相应的选择数字即可。
echo. 
echo 作者：RG  
echo. 
echo 网址：http://www.9cpp.com/
echo ------------------------------------------------------------
echo. 
echo 5秒后退出,正在退出...
timeout /t 5
Exit
