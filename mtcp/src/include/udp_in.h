#ifndef UDP_IN_H
#define UDP_IN_H


#include "mtcp.h"

void intToIPAddress(unsigned int num, char *ipAddress);
int
ProcessUDPPacket(mtcp_manager_t mtcp, 
		 uint32_t cur_ts, const int ifidx, const struct iphdr *iph, int ip_len);

#endif /* UDP_IN_H */
