System Requirements

    On Linux, you need libpcap (http://www.tcpdump.org/) installed.
    On Windows, you need WinPcap (http://www.winpcap.org/) installed.


Functions

1. Auto detect ARP spoofing and Bidirectional antispoof
    The ArpAntiSpoofer can automatically detect ARP spoofing. When spoofing is detected, it can send antispoof packet to the hosts you want to protect and the gateway. The antispoof packet rate can be multiple times the spoof packet rate, so the gateway and all the hosts under protected will be able to survive the spoofing.

2. ARP helper
    In some situation (e.g. the gateway device is too old), the gateway doesn't send ARP reply and/or request packet normally. The ArpAntiSpoofer can help it to work well with ARP: It can reply ARP requests for the gateway, and/or proactively broadcast ARP replys to the whole subnet. It's important to note that the second function also helps a lot when IP conflicts.


Configuration

    Before the function of the program works, the configuration file should be filled correctly. On Linux, the default location of the configuration file is /etc/arpas.ini. On Windows, the default location is .\arpas.ini. You may modify the configuration through the GUI or directly modify the ini file. And you can use '-c' command to specify other configuration files.
    The meanings of the keys in the ini file are as follows:

[main]
adapter:
    The network adapter defined by libpcap/WinPcap. On Linux, it is the network interface such as eth0, eth1 etc. On Windows, you had better modify it through the GUI.
netmask:
    The subnet mask of the network. The default value is 255.255.255.0.
bind_gateway:
    A boolean, bind the IP and MAC address of the gateway or not.
auto_detect:
    A boolean, automatically detect spoofing or not.
keep_protect_status:
    A boolean, keep antispoof status when start/stop auto detect or not.
multiple:
    The ratio of antispoofing to spoofing. For example, if the spoofer is sending 1 packet per second, then we will send `multiple' packet per second to antispoof.
spoof_range_second:
    Spoof range, in second. If there is no spoofing packet in `spoof_range_second' seconds, then we think that the spoofing is stopped.
reply_for_gateway:
    A boolean, reply ARP requests for the gateway or not. If the gateway doesn't reply ARP requests normally, set this to 1.
reply_for_hosts_to_gw:
    A boolean, send ARP reply for the hosts we protected to the gateway or not. If the gateway doesn't send ARP requests normally, set this to 1.
reply_for_hosts_intv:
    Interval of sending ARP reply for the hosts, in millisecond.
remote_ctrl:
    A boolean, enable remote control (telnet) or not.
remote_ctrl_port:
    Port of remote control.
remote_ctrl_pswd:
    Password of remote control. In telnet interface, you should type the password first, then the greeting words will appear.
local_addr:
    IP and MAC address of local host, in this format: IP_address|MAC_address.
gateway_addr:
    IP and MAC address of the gateway, in this format: IP_address|MAC_address.
log_file:
    Full path of the log file. Default path is /var/log/arpas.log in Linux and .\arpas.log in Windows.
notify_spoof:
    Notify spoofing status if detected.

[protect_hosts]
    All the hosts you want to protect should be added to this section, including the local host. The key name is the IP of the host. The key value is in this format: MAC_address|protect_flag. `protect_flag' specifies the action of antispoofing, in which 'h' means sending antispoof packet to this Host, 'g' means sending antispoof packet to Gateway.
    Example:
192.168.0.100=00-11-22-33-44-55|hg
192.168.0.101=00:11:22:33:44:66|g
