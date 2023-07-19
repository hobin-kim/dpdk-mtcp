#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include <arpa/inet.h>

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

int
ProcessUDPPacket(mtcp_manager_t mtcp, 
		 uint32_t cur_ts, const int ifidx, const struct iphdr *iph, int ip_len)
{
	// UDP payload parsing하기
	struct udphdr* udph = (struct udphdr *) ((u_char *)iph + (iph->ihl << 2));
	uint8_t *payload    = (uint8_t *)udph + 2 ;  // UDP header는 2B
	int payloadlen = ip_len - (payload - (u_char *)iph);


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
	

	// raise read event

	// 

};
