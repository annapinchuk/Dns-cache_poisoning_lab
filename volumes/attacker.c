#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    FILE *req = fopen("req.bin", "wb");
    FILE *res = fopen("res.bin", "wb");
    unsigned short id = 0;

    while (1)
    {
        // Modify the name in the question field (offset=41)
        memcpy(req + 41, "bbbbb", 5);
        // Modify the name in the answer field (offset=64)
        memcpy(res + 64, "bbbbb", 5);
        // Modify the transaction ID field (offset=28)
        unsigned short id_net_order = htons(id);
        memcpy(res + 28, &id_net_order, 2);
        id++;
    }
}
