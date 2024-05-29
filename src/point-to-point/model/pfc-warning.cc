#include "ns3/pfc-warning.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <random>
#include <set>

#include "ns3/assert.h"
#include "ns3/event-id.h"
#include "ns3/flow-id-tag.h"
#include "ns3/ipv4-header.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/ppp-header.h"
#include "ns3/qbb-header.h"
#include "ns3/random-variable.h"
#include "ns3/settings.h"
#include "ns3/simulator.h"
#include "ns3/udp-header.h"


namespace ns3 {

PfcNotify::PfcNotify(): Tag() {}
TypeId PfcNotify::GetTypeId(void) {
    static TypeId tid =
        TypeId("ns3::PfcNotify").SetParent<Tag>().AddConstructor<PfcNotify>();
    return tid;
}
TypeId PfcNotify::GetInstanceTypeId(void) const { return GetTypeId(); }
uint32_t PfcNotify::GetSerializedSize(void) const {
    return sizeof(uint32_t) + sizeof(uint32_t);
}
void PfcNotify::Serialize(TagBuffer i) const {
    i.WriteU32(m_sourceId);
    i.WriteU32(m_type);
}
void PfcNotify::Deserialize(TagBuffer i) {
    m_sourceId = i.ReadU32();
    m_type = i.ReadU32();
}

void PfcNotify::Print(std::ostream &os) const {
    os << "m_sourceId=" << m_sourceId;
}

PfcWarning::PfcWarning() {
}
uint32_t PfcWarning::m_pasuedNum = 0;


PfcWarning::~PfcWarning() {}

TypeId PfcWarning::GetTypeId(void) {
    static TypeId tid =
        TypeId("ns3::PfcWarning").SetParent<Object>().AddConstructor<PfcWarning>();
    return tid;
}

void PfcWarning::UpdateWarning(Ptr<Packet> p) {
    PfcNotify pfcNotify;
    bool foundPfcNotify = p->PeekPacketTag(pfcNotify);
    // std::cout << "开始处理通知包:" << foundPfcNotify << std::endl;
    uint32_t pausePort = 0;
    for (int i = 17; i < 17+8; i++) {
        if (Settings::IdPortToId[GetSwitchPortKey(m_switch_id,i)] == pfcNotify.m_sourceId) {
            //std::cout << m_switch_id << ":端口 " << i << " 接收到暂停预警了" << std::endl;
            pausePort = i;
            break;
        }
        
    }
    if (pfcNotify.m_type) {
        m_isWarning[pausePort] = 1;
        m_warningTime[pausePort] = Simulator::Now().GetNanoSeconds();
        // std::cout << m_switch_id << " " << pausePort << " 接收到预警,时间为:" << m_warningTime[pausePort] << std::endl;
    }else {
       // std::cout << "持续的时间为:" << Simulator::Now().GetNanoSeconds() - m_warningTime[pausePort] << std::endl;
        m_isWarning[pausePort] = 0;
        m_warningTime[pausePort] = 0;
    }
}

void PfcWarning::SendPfcWarning(Ptr<Packet> p, uint32_t inDev, uint32_t type) {
    // std::cout << "发送通知数据包" << std::endl;
    // qbbHeader seqh;
    // seqh.SetSeq(0);
    // seqh.SetPG(ch.udp.pg);
    // seqh.SetSport(ch.udp.dport);
    // seqh.SetDport(ch.udp.sport);
    // seqh.SetIntHeader(ch.udp.ih);

    Ptr<Packet> fbP = Create<Packet>(0);  // at least 64 Bytes
    // fbP->AddHeader(seqh);                                            // qbbHeader

    // ACK-like packet, no L4 header
    Ipv4Header ipv4h;
    ipv4h.SetSource(Settings::node_id_to_ip(m_switch_id));
    ipv4h.SetDestination(Ipv4Address("255.255.255.255"));
    ipv4h.SetProtocol(0xFD);  // (N)ACK - (IRN)
    ipv4h.SetTtl(64);
    ipv4h.SetPayloadSize(fbP->GetSize());
    ipv4h.SetIdentification(UniformVariable(0, 65536).GetValue());
    fbP->AddHeader(ipv4h);  // ipv4Header

    PppHeader ppp;
    ppp.SetProtocol(0x0021);  // EtherToPpp(0x800), see point-to-point-net-device.cc
    fbP->AddHeader(ppp);      // pppHeader

    PfcNotify pfcNotifyTag;
    pfcNotifyTag.m_sourceId = m_switch_id;
    pfcNotifyTag.m_type = type;
    fbP->AddPacketTag(pfcNotifyTag);

    // extract customheader
    CustomHeader fbCh(CustomHeader::L2_Header | CustomHeader::L3_Header | CustomHeader::L4_Header);
    fbP->PeekHeader(fbCh);

   
    DoSwitchSend(fbP, fbCh, inDev, 0);  // will have ACK's priority
    return;
}

bool PfcWarning::SwitchPaused(uint32_t switch_id, uint32_t pathId) {
    uint32_t port1 = GetOutPortFromPath(pathId, 0);
    uint32_t port2 = GetOutPortFromPath(pathId, 1);
    uint32_t aggSwitchId = Settings::IdPortToId[GetSwitchPortKey(switch_id, port1)];
    // for (int i = 136; i <= 143; i++) {
    //     for (int j = 1; j <= 8; j++) {
    //         if (Settings::m_pausedInfo[i][j]) {
    //             std::cout << "此时" << i << " " << j << "被暂停了" << std::endl;
    //         }
    //     }
    // }
    // std::cout << "检测" << switch_id << " " << port1 << " 对应的中间交换机" << aggSwitchId << " " 
    //     << port2 << " " << Settings::m_pausedInfo[aggSwitchId][port2] << std::endl;
    return Settings::m_pausedInfo[aggSwitchId][port2];
}

uint64_t PfcWarning::GetSwitchPortKey(uint32_t switchNum, uint16_t port) {
    return (switchNum<<16)+port;
}

uint32_t PfcWarning::GetOutPortFromPath(const uint32_t& path, const uint32_t& hopCount) {
    return ((uint8_t*)&path)[hopCount];
}

void PfcWarning::SetSwitchSendCallback(SwitchSendCallback switchSendCallback) {
    m_switchSendCallback = switchSendCallback;
}

void PfcWarning::SetSwitchSendToDevCallback(SwitchSendToDevCallback switchSendToDevCallback) {
    m_switchSendToDevCallback = switchSendToDevCallback;
}

void PfcWarning::DoSwitchSendToDev(Ptr<Packet> p, CustomHeader &ch) {
    m_switchSendToDevCallback(p, ch);
}

void PfcWarning::DoSwitchSend(Ptr<Packet> p, CustomHeader &ch, uint32_t outDev,
                                   uint32_t qIndex) {
    m_switchSendCallback(p, ch, outDev, qIndex);
}
}