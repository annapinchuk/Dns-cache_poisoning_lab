from scapy.all import DNSQR
Qdsec = DNSQR(qname='www.example.com')
dns = DNS(id=0xAAAA, qr=0, qdcount=1, ancount=0, nscount=0,
arcount=0, qd=Qdsec)
ip = IP(dst='10.9.0.53', src='1.2.3.4')
udp = UDP(dport=33333, sport=1234, chksum=0)
request = ip/udp/dns
# Save the packet to a file
with open('req.bin', 'wb') as f:
    f.write(bytes(request))