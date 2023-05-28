#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

void generate_name(char* name)
{
    char letters[26] = "abcdefghijklmnopqrstuvwxyz";
    // Generate a random name of length 5
    name[5] = 0;
    for (size_t i = 0; i < 5; i++)
        name[i] = letters[rand() % 26];
}

int main(int argc, char const *argv[])
{
    FILE *req = fopen("req.bin", "wb");
    FILE *res = fopen("res.bin", "wb");
    unsigned short id = 0;
    char name[6];

    while (1)
    {
        generate_name(name);
        // Modify the name in the question field (offset=41)
        memcpy(req + 41, name, 5);
        // Modify the name in the answer field (offset=64)
        memcpy(res + 64, name, 5);
        
        unsigned short id_net_order = htons(id);
        // Modify the transaction ID field (offset=28)
        memcpy(res + 28, &id_net_order, 2);
        id++;
    }
}
