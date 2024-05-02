#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <features.h>

void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) {
    struct ip *iphdr = (struct ip*)(packet + 14); // Assuming Ethernet II header
    struct udphdr *udphdr = (struct udphdr*)(packet + 14 + (iphdr->ip_hl * 4)); // IP header length varies

    printf("Source IP: %s\n", inet_ntoa(iphdr->ip_src));
    printf("Destination IP: %s\n", inet_ntoa(iphdr->ip_dst));
    printf("Source Port: %d\n", ntohs(udphdr->source));
    printf("Destination Port: %d\n", ntohs(udphdr->dest));

    printf("Packet length: %d\n", pkthdr->len);
    printf("Packet data:\n");
    for (int i = 0; i < pkthdr->len; i++) {
        printf("%02x ", packet[i]);
    }
    printf("\n\n\n");
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device: %s\n", errbuf);
        return 1;
    }

    struct bpf_program fp;
    char filter_exp[] = "udp";
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        fprintf(stderr, "Could not parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 1;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Could not install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 1;
    }

    if (pcap_loop(handle, 0, packet_handler, NULL) < 0) {
        fprintf(stderr, "pcap_loop error: %s\n", pcap_geterr(handle));
        return 1;
    }

    pcap_close(handle);
    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <pcap.h>
// #include <features.h>

// /* Callback function for libpcap */
// void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
//     printf("Packet length: %d\n", header->len);
//     printf("Packet data:\n");
//     for (int i = 0; i < header->len; i++) {
//         printf("%02x ", packet[i]);
//     }
//     printf("\n\n");
// }

// int main() {
//     char *device;
//     char errbuf[PCAP_ERRBUF_SIZE];
//     pcap_t *handle;

//     /* Find a suitable device */
//     device = pcap_lookupdev(errbuf);
//     if (device == NULL) {
//         fprintf(stderr, "Error finding a suitable device: %s\n", errbuf);
//         return EXIT_FAILURE;
//     }

//     /* Open the device */
//     handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
//     if (handle == NULL) {
//         fprintf(stderr, "Error opening the device: %s\n", errbuf);
//         return EXIT_FAILURE;
//     }

//     /* Start sniffing packets */
//     pcap_loop(handle, -1, packet_handler, NULL);

//     /* Close the device */
//     pcap_close(handle);

//     return EXIT_SUCCESS;
// }
