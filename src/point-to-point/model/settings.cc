#include "ns3/settings.h"

namespace ns3 {
/* helper function */
Ipv4Address Settings::node_id_to_ip(uint32_t id) {
    return Ipv4Address(0x0b000001 + ((id / 256) * 0x00010000) + ((id % 256) * 0x00000100));
}
uint32_t Settings::ip_to_node_id(Ipv4Address ip) {
    return (ip.Get() >> 8) & 0xffff;
}

/* others */
uint32_t Settings::lb_mode = 0;

std::map<uint32_t, uint32_t> Settings::hostIp2IdMap;
std::map<uint32_t, uint32_t> Settings::hostId2IpMap;

/* statistics */
uint32_t Settings::node_num = 0;
uint32_t Settings::host_num = 0;
uint32_t Settings::switch_num = 0;
uint64_t Settings::cnt_finished_flows = 0;
uint32_t Settings::packet_payload = 1000;

uint32_t Settings::dropped_pkt_sw_ingress = 0;
uint32_t Settings::dropped_pkt_sw_egress = 0;
uint64_t Settings::maxRepeatFlowNum = 0;
bool Settings::is_fatTree = false;

/* for load balancer */
std::map<uint32_t, uint32_t> Settings::hostIp2SwitchId;
std::map<uint64_t, uint64_t> Settings::flowKeyNum;
std::map<uint64_t, uint64_t> Settings::IdPortToId;
std::map<uint64_t, uint32_t> Settings::m_sendPakcetNum;       // 指定交换机id+端口号发送的数据包
std::map<uint64_t, uint32_t> Settings::m_receivePacketNum;    // 对应的交换机id+端口号接收到的数据包
bool Settings::m_pausedInfo[1024][1024];
}  // namespace ns3
