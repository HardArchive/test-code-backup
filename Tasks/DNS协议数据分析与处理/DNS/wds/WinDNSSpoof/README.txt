WinDNSSpoof
-----------

This tool is a simple DNS ID Spoofer for Windows 9x/2K.
Firstly you must be able to sniff traffic of the target.
If you are in a switched network you can use ARP Cache
Poisoning tools like winarp_sk or winarp_mim (see 
http://www.arp-sk.org)

If you want to be sure that the DNS Server doesn't answer
before WinDNSSpoof configure a personnal firewall to block
outgoing DNS traffic (UDP 53 destination port). After this
WinDNSSpoof takes care to split DNS packet to spoof and the
others. For this you must specify the MAC address of the
DNS server (or the default gateway if the DNS server is in
another network).

Usage : wds -h

Example : wds -n www.microsoft.com -i 216.239.39.101 -g 00-00-39-5c-45-3b

N.B. : WinDNSSpoof requires WinPcap (see http://winpcap.polito.it)


>>> Valgasu <<<

Mail : valgasu@securiteinfo.com
Web  : www.securiteinfo.com