from scapy.all import DNSQR, DNS, IP, UDP

# Construct the DNS header and payload
Qdsec = DNSQR(qname='start.example.com')
dns = DNS(id=0xAAAA, qr=0, qdcount=1, qd=Qdsec)
# Construct the IP, UDP headers, and the entire packet
ip = IP(dst='10.9.0.53', src='1.1.2.2')
udp = UDP(dport=53, sport=1234, chksum=0)
request = ip/udp/dns
# Save the packet to a file
with open('req.bin', 'wb') as f:
    f.write(bytes(request))
