@echo off
setlocal enabledelayedexpansion
set colsnum=60
set linesnum=22
MODE con: COLS=%colsnum% lines=%linesnum%
::���÷�����ServiceName
set ServiceName=IpLookupService.exe

::���ƾ��м���
set/a colsnum=%colsnum%-8
::echo�Լ����Ŀո�ռ5�У�ʵ�ʲ�ֹ5��
set myecho=������Ҫ����%ServiceName%����
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
title %ServiceName%����
CLS
ECHO.
ECHO !space!%myecho%
ECHO.        
ECHO.        ��ǰϵͳʱ�� %date% %tm1%��%TM2%��%TM3%��
ECHO.
ECHO.       �T�T�T�T�T�بT�T�T�ƨT�T�T���T�T�T��T�T�T�T�T
ECHO        [1] ����%ServiceName%����
ECHO. 
ECHO        [2] ��װ%ServiceName%����
ECHO. 
ECHO        [3] ����%ServiceName%����
ECHO.         
ECHO        [4] ɾ��%ServiceName%����
ECHO.                 
ECHO        [5] �˳�...
ECHO.       �T�T�T�T�T��T�T�T��T�T�T���T�T�T�֨T�T�T�T�T
ECHO.
ECHO                -=����QNA���=- (V0.5)
ECHO.

set choice=
set /p choice=       �������Ӧ�����ֺ�[Enter]ִ��:
if not "%choice%"=="" set choice=%choice%
ECHO.
if /I "%choice%"=="1" Goto STARTPANEL
if /I "%choice%"=="2" Goto INSTALLPANEL
if /I "%choice%"=="3" Goto RESTARTPANEL
if /I "%choice%"=="4" Goto DELPANEL
if /I "%choice%"=="5" Goto EXITPANEL
if /I "%choice%" NEQ "5" Goto ERRORPANEL
goto MENUSTART

::�����������
:STARTPANEL
CLS
title ����%ServiceName%����
echo ��������%ServiceName%����... 
CALL %ServiceName% -d
CLS
goto MENUSTART

::��װ�������
:INSTALLPANEL
CLS
title ��װ%ServiceName%����
echo ���ڰ�װ%ServiceName%����... 
CALL %ServiceName% -i
CLS
goto MENUSTART

::ֹͣ�������
:STOPPANEL
CLS
title ֹͣ%ServiceName%����
echo ����ֹͣ%ServiceName%�������Ժ�......
net stop %ServiceName% 
CLS
goto MENUSTART

::ɾ���������
:DELPANEL
CLS
title ɾ��%ServiceName%����
echo ����ֹͣ%ServiceName%����... 
net stop %ServiceName% 
echo ����ɾ��%ServiceName%����... 
sc delete %ServiceName% 
CALL %ServiceName% -u
CLS
goto MENUSTART

::�����������
:RESTARTPANEL
CLS
title ����%ServiceName%����
echo ����ֹͣ%ServiceName%�������Ժ�......
CALL %ServiceName% -u
echo ��������%ServiceName%����... 
CALL %ServiceName% -i
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
echo.               �������V0.5��(QNA����)
echo. 
echo ------------------------------------------------------------
echo ˵������������һ��ָ�����������������������Ѿ�װ�ķ���֧�ַ���������ֹͣ��ɾ����һ�����ڷ���������ʱ�ڡ�
echo. 
echo �÷����򿪱��ļ�����7�е�%ServiceName%�滻���Լ���Ҫ����ķ��񼴿ɡ�
echo. 
echo ���ߣ�QNA  
echo. 
echo ��ַ��http://www.9cpp.com/
echo ------------------------------------------------------------
echo. 
echo 5����˳�,�����˳�...
timeout /t 5
Exit
