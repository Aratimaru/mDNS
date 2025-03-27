#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>

#include "mdns.h"

void encode_mdns_name(const char *hostname, unsigned char *buffer, int *len) {
    const char *pos = hostname;
    unsigned char *ptr = buffer;
    
    while (*pos) {
        const char *dot = strchr(pos, '.');
        if (!dot) dot = pos + strlen(pos);
        
        int label_length = dot - pos;
        *ptr++ = label_length;
        memcpy(ptr, pos, label_length);
        ptr += label_length;
        
        if (*dot == '.') dot++;
        pos = dot;
    }
    *ptr++ = 0;
    *len = ptr - buffer;
}

int main()
{
    int sock;
    struct sockaddr_in multicast_addr;
    unsigned char packet[512];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

     char ifname[] = "eth0"; // Change to your network interface
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)) < 0) {
        perror("Failed to bind to interface");
        exit(EXIT_FAILURE);
    }

    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(MDNS_PORT);
    multicast_addr.sin_addr.s_addr = inet_addr(MDNS_GROUP);

    mdns_header_t *mdns_header_request = (struct mdns_header *)packet;

    mdns_header_request->transaction_id = htons(0x0000);
    mdns_header_request->flags = htons(0x0000);
    mdns_header_request->question_count = htons(0x0001);
    mdns_header_request->answer_count = htons(0x0000);
    mdns_header_request->authority_count = htons(0x0000);
    mdns_header_request->additional_count = htons(0x0000);

    mdns_question_t mdns_question = {
        .name = "example.local",
        .type = 0x0001,
        ._class = 0x8001
    };

    int name_len = 0;
    encode_mdns_name("example.local", packet + sizeof(struct mdns_header), &name_len);

    uint16_t *qtype = (uint16_t *)(packet + sizeof(struct mdns_header) + name_len);
    *qtype = htons(0x0001); // Type A

    uint16_t *qclass = (uint16_t *)(packet + sizeof(struct mdns_header) + name_len + 2);
    *qclass = htons(0x8001); // Class IN + multicast bit

    int packet_size = sizeof(struct mdns_header) + name_len + 4;

    if (sendto(sock, packet, packet_size, 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0) {
        perror("Failed to send mDNS query");
        exit(EXIT_FAILURE);
    }

    printf("mDNS query for example.local sent!\n");
    close(sock);

    return 0;
}