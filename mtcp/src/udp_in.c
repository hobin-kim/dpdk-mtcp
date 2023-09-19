#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "tcp_util.h"
#include "tcp_in.h"
#include "tcp_out.h"
#include "tcp_ring_buffer.h"
#include "eventpoll.h"
#include "debug.h"
#include "timer.h"
#include "ip_in.h"
#include "clock.h"
#include "mtcp_api.h" // hobin added
#if USE_CCP
#include "ccp.h"
#endif

/*----------------------------------------------------------------------------*/
#include <rte_mbuf.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <mtcp_api.h>
/*----------------------------------------------------------------------------*/
void intToIPAddress(unsigned int num, char *ipAddress) {
    // Calculate the four octets
    unsigned int A = num / (256 * 256 * 256);
    num = num - (A * 256 * 256 * 256);

    unsigned int B = num / (256 * 256);
    num = num - (B * 256 * 256);

    unsigned int C = num / 256;
    unsigned int D = num % 256;

    // Print the IP address
    sprintf(ipAddress, "%u.%u.%u.%u", D, C, B, A);
}

int
ProcessUDPPacket(mtcp_manager_t mtcp, 
		 uint32_t cur_ts, const int ifidx, const struct iphdr *iph, int ip_len)
{
	fprintf(stderr, "In udp_in.c ProcessUDPPacet function");
	// UDP payload parsing하기
	struct udp_hdr* udph = (struct udp_hdr *)(iph + 1);
	uint8_t *payload = (uint8_t*)(udph + 1);  // UDP header는 2B
	int payloadlen = ntohs(udph->dgram_len) - sizeof(struct udp_hdr);

    char src_ip[16];
	char dst_ip[16];
	intToIPAddress(iph->saddr, src_ip);
	intToIPAddress(iph->daddr, dst_ip);
	fprintf(stderr, "In udp_in.c source ip address = %s \n dest ip address = %s\n", src_ip, dst_ip);
	fprintf(stderr, "In udp_in.c payload? = %s \n payload len? = %d\n", payload, payloadlen-1);
	fprintf(stderr, "In udp_in.c mtcp address = %p\n", mtcp);

	// variable declaration
	int ret;

	// UDP socket 찾기
	// mtcp->ucp_socket

	// UDP socket의 recv buffer에 데이터 저장하고, RBInit?
	if(!mtcp->udp_rcvbuf) {
		mtcp->udp_rcvbuf = udp_RBInit(mtcp->rbm_rcv);
		if(!mtcp->udp_rcvbuf) {
			udp_RaiseErrorEvent(mtcp);
			return ERROR;
		}
	}

	// RBPut to the recv buffer
	ret = udp_RBPut(mtcp->rbm_rcv, mtcp->udp_rcvbuf, payload, (uint32_t)payloadlen);
	if (ret < 0) {
		TRACE_ERROR("Cannot merge payload. reason: %d\n", ret);
	}
	
	// raise read event
	udp_RaiseReadEvent(mtcp);
	return TRUE;
};
