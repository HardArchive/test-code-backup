@echo off
setlocal enabledelayedexpansion
set colsnum=60
set linesnum=22
MODE con: COLS=%colsnum% lines=%linesnum%
::���÷�����ServiceName
set ProgramName=Dr.Watson

::���ƾ��м���
set/a colsnum=%colsnum%-8
::echo�Լ����Ŀո�ռ5�У�ʵ�ʲ�ֹ5��
set myecho=%ProgramName%���Գ������
::����Ҫ������ַ�����ֵ������myecho
set flag=0
::��ʼ��������flag
for /l %%i in (0,1,1000) do (
set tt=!myecho:~%%i,1!
if "!tt!" equ "" goto next
set/a flag=!flag!+1
)
::ͳ����������ַ�������
:next
set/a colsnum=!colsnum!-!flag!
set/a colsnum=!colsnum!/2
for /l %%i in (1,1,!colsnum!) do (
set space= !space!
)
::�����˵����
:MENUSTART
color 1a
set tm1=%time:~0,2%
set tm2=%time:~3,2%
set tm3=%time:~6,2%
title %ProgramName%����
CLS
ECHO.
ECHO !space!%myecho%
ECHO.        
ECHO.        ��ǰϵͳʱ�� %date% %tm1%��%TM2%��%TM3%��
ECHO.
ECHO.       �T�T�T�T�T�بT�T�T�ƨT�T�T���T�T�T��T�T�T�T�T
ECHO        [1] %ProgramName%����
ECHO. 
ECHO        [2] %ProgramName%��װ
ECHO. 
ECHO        [3] %ProgramName%����
ECHO. 
ECHO        [4] %ProgramName%ж��
ECHO.         
ECHO        [5] �˳�...
ECHO.       �T�T�T�T�T��T�T�T��T�T�T���T�T�T�֨T�T�T�T�T
ECHO.
ECHO                 -=����RG���=- (V0.1)
ECHO.

set choice=
set /p choice=       �������Ӧ�����ֺ�[Enter]ִ��:
if not "%choice%"=="" set choice=%choice%
ECHO.
if /I "%choice%"=="1" Goto READMEPANEL
if /I "%choice%"=="2" Goto INSTALLPANEL
if /I "%choice%"=="3" Goto SETPANEL
if /I "%choice%"=="4" Goto UNINSTALLPANEL
if /I "%choice%"=="5" Goto EXITPANEL
if /I "%choice%" NEQ "5" Goto ERRORPANEL
goto MENUSTART

::˵�����
:READMEPANEL
CLS
title %ProgramName%����
echo.
echo ------------------------------------------------------------
echo Dr.Watson����������ҽ������һ�����������Գ���
echo. 
echo Dr.Watson����Ӧ�ó��������ʱ���Զ�������
echo ������Ĭ������£����Ὣ������йص��ڴ��ΪDUMP�ļ��Թ�����Ա������
echo ------------------------------------------------------------ ��
echo. 
pause
CLS
goto MENUSTART

::��װ���
:INSTALLPANEL
CLS
title ��װ%ProgramName%
echo ���ڰ�װ%ProgramName%... 
CALL drwtsn32 -i
CLS
goto MENUSTART

::�������
:SETPANEL
CLS
title ����%ProgramName%
echo ��������%ProgramName%����... 
CALL drwtsn32
CLS
goto MENUSTART

::ж�����
:UNINSTALLPANEL
CLS
title ж��%ProgramName%
echo ����ж��%ProgramName%... 
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug" /v "Auto" /t "REG_SZ" /d "0" /f
::/v��ֵ��  /t���� /dֵ /f����ʾ
CLS
goto MENUSTART

::����������
:ERRORPANEL
CLS
echo ��������ȷ������...
timeout /t 5
goto MENUSTART

::�˳����
:EXITPANEL
cls
echo. 
echo.               %ProgramName%����V0.1��(RG����)
echo. 
echo ------------------------------------------------------------
echo ˵������������һ������%ProgramName%��������������%ProgramName%���á����á�ͣ�ã�һ�����ڳ������ʱ�ڡ�
echo. 
echo �÷������ղ˵�˵��������Ӧ��ѡ�����ּ��ɡ�
echo. 
echo ���ߣ�RG  
echo. 
echo ��ַ��http://www.9cpp.com/
echo ------------------------------------------------------------
echo. 
echo 5����˳�,�����˳�...
timeout /t 5
Exit
