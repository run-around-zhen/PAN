#include "ns3/gemma-routing.h"

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

/**
 * @brief tag for DATA header
 */
GemmaDataTag::GemmaDataTag() : Tag() {}
TypeId GemmaDataTag::GetTypeId(void) {
    static TypeId tid =
        TypeId("ns3::GemmaDataTag").SetParent<Tag>().AddConstructor<GemmaDataTag>();
    return tid;
}
void GemmaDataTag::SetPathId(uint32_t pathId) { m_pathId = pathId; }
uint32_t GemmaDataTag::GetPathId(void) const { return m_pathId; }
void GemmaDataTag::SetHopCount(uint32_t hopCount) { m_hopCount = hopCount; }
uint32_t GemmaDataTag::GetHopCount(void) const { return m_hopCount; }
void GemmaDataTag::SetPhase(uint32_t phase) { m_phase = phase; }
uint32_t GemmaDataTag::GetPhase(void) const { return m_phase; }
void GemmaDataTag::SetTimestampTx(uint64_t timestamp) { m_timestampTx = timestamp; }
uint64_t GemmaDataTag::GetTimestampTx(void) const { return m_timestampTx; }
void GemmaDataTag::SetFlagData(uint32_t flag) { m_flagData = flag; }
uint32_t GemmaDataTag::GetFlagData(void) const { return m_flagData; }
void GemmaDataTag::SetPrePathCongestionRank(uint32_t congestionRank) { m_prePathCongestionRank = congestionRank; }
uint32_t GemmaDataTag::GetPrePathCongestionRank(void) const { return m_prePathCongestionRank; }
void GemmaDataTag::SetPathCongestionRank(uint32_t congestionRank) { m_pathCongestionRank = congestionRank; }
uint32_t GemmaDataTag::GetPathCongestionRank(void) const { return m_pathCongestionRank; }
void GemmaDataTag::SetWaitedLoop(uint32_t waitedLoop) { m_waitedLoop = waitedLoop; }
uint32_t GemmaDataTag::GetWaitedLoop(void) const { return m_waitedLoop; }
void GemmaDataTag::SetIsFirst(uint32_t isFirst) { m_isFirst = isFirst; }
uint32_t GemmaDataTag::GetIsFirst(void) const { return m_isFirst; }
void GemmaDataTag::SetSwitchPortKey(uint64_t switchPortKey) { m_switchPortKey = switchPortKey; }
uint64_t GemmaDataTag::GetSwitchPortKey(void) const { return m_switchPortKey; }
void GemmaDataTag::SetStage(uint32_t stage) { m_stage = stage; }
uint32_t GemmaDataTag::GetStage(void) const { return m_stage; }
void GemmaDataTag::SetFlagNotify(uint32_t flag) { m_flagNotify = flag; }
uint32_t GemmaDataTag::GetFlagNotify(void) const { return m_flagNotify; }

TypeId GemmaDataTag::GetInstanceTypeId(void) const { return GetTypeId(); }
uint32_t GemmaDataTag::GetSerializedSize(void) const {
    return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) +
           sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) + 
           sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + 
           sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t);
}
void GemmaDataTag::Serialize(TagBuffer i) const {
    i.WriteU32(m_pathId);
    i.WriteU32(m_hopCount);
    i.WriteU32(m_phase);
    i.WriteU64(m_timestampTx);
    i.WriteU32(m_flagData);
    i.WriteU32(m_prePathCongestionRank);
    i.WriteU32(m_pathCongestionRank);
    i.WriteU32(m_waitedLoop);
    i.WriteU32(m_isFirst);
    i.WriteU64(m_switchPortKey);
    i.WriteU32(m_stage);
    i.WriteU32(m_flagNotify);
}
void GemmaDataTag::Deserialize(TagBuffer i) {
    m_pathId = i.ReadU32();
    m_hopCount = i.ReadU32();
    m_phase = i.ReadU32();
    m_timestampTx = i.ReadU64();
    m_flagData = i.ReadU32();
    m_prePathCongestionRank = i.ReadU32();
    m_pathCongestionRank = i.ReadU32();
    m_waitedLoop = i.ReadU32();
    m_isFirst = i.ReadU32();
    m_switchPortKey = i.ReadU64();
    m_stage = i.ReadU32();
    m_flagNotify = i.ReadU32();
}
void GemmaDataTag::Print(std::ostream &os) const {
    os << "m_pathId=" << m_pathId;
    os << ", m_hopCount=" << m_hopCount;
    os << ", m_phase=" << m_phase;
    os << ". m_timestampTx=" << m_timestampTx;
    os << ", m_flagData=" << m_flagData;
}

GemmaNotifyTag::GemmaNotifyTag() : Tag() {}
TypeId GemmaNotifyTag::GetTypeId(void) {
    static TypeId tid =
        TypeId("ns3::GemmaNotifyTag").SetParent<Tag>().AddConstructor<GemmaNotifyTag>();
    return tid;
}
void GemmaNotifyTag::SetPathId(uint32_t pathId) { m_pathId = pathId; }
uint32_t GemmaNotifyTag::GetPathId(void) const { return m_pathId; }
void GemmaNotifyTag::SetSwitchPortKey(uint64_t switchPortKey) { m_switchPortKey = switchPortKey; }
void GemmaNotifyTag::SetSwitchPortKeyB(uint64_t switchPortKeyB) { m_switchPortKeyB = switchPortKeyB; }
void GemmaNotifyTag::SetSwitchPortKeyC(uint64_t switchPortKeyC) { m_switchPortKeyC = switchPortKeyC; }
uint64_t GemmaNotifyTag::GetSwitchPortKey(void) const { return m_switchPortKey; }
uint64_t GemmaNotifyTag::GetSwitchPortKeyB(void) const { return m_switchPortKeyB; }
uint64_t GemmaNotifyTag::GetSwitchPortKeyC(void) const { return m_switchPortKeyC; }
void GemmaNotifyTag::SetReceivePacketNum(uint32_t receivePacketNum) { m_receivePacketNum = receivePacketNum; }
uint32_t GemmaNotifyTag::GetReceivePacketNum(void) const { return m_receivePacketNum; }
void GemmaNotifyTag::SetPathScore(uint32_t pathScore) { m_pathScore = pathScore; }
void GemmaNotifyTag::SetPathScoreB(uint32_t pathScoreB) { m_pathScoreB = pathScoreB; }
void GemmaNotifyTag::SetPathScoreC(uint32_t pathScoreC) { m_pathScoreC = m_pathScoreC; }
uint32_t GemmaNotifyTag::GetPathScore(void) const { return m_pathScore; }
uint32_t GemmaNotifyTag::GetPathScoreB(void) const { return m_pathScoreB; }
uint32_t GemmaNotifyTag::GetPathScoreC(void) const { return m_pathScoreC; }
TypeId GemmaNotifyTag::GetInstanceTypeId(void) const { return GetTypeId(); }
uint32_t GemmaNotifyTag::GetSerializedSize(void) const { 
    return sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint64_t)+sizeof(uint64_t)+sizeof(uint32_t)+sizeof(uint32_t)+
    sizeof(uint32_t)+sizeof(uint32_t); 
}
void GemmaNotifyTag::Serialize(TagBuffer i) const { 
    i.WriteU32(m_pathId);
    i.WriteU64(m_switchPortKey); 
    i.WriteU64(m_switchPortKeyB); 
    i.WriteU64(m_switchPortKeyC); 
    i.WriteU32(m_receivePacketNum);
    i.WriteU32(m_pathScore);
    i.WriteU32(m_pathScoreB);
    i.WriteU32(m_pathScoreC);
}
void GemmaNotifyTag::Deserialize(TagBuffer i) { 
    m_pathId = i.ReadU32(); 
    m_switchPortKey = i.ReadU64(); 
    m_switchPortKeyB = i.ReadU64(); 
    m_switchPortKeyC = i.ReadU64(); 
    m_receivePacketNum= i.ReadU32();
    m_pathScore = i.ReadU32();
    m_pathScoreB = i.ReadU32();
    m_pathScoreC = i.ReadU32();
}
void GemmaNotifyTag::Print(std::ostream &os) const { os << "m_pathId=" << m_pathId; }

/**
 * @brief tag for reply/notify packet
 */
GemmaReplyTag::GemmaReplyTag() : Tag() {}
TypeId GemmaReplyTag::GetTypeId(void) {
    static TypeId tid =
        TypeId("ns3::GemmaReplyTag").SetParent<Tag>().AddConstructor<GemmaReplyTag>();
    return tid;
}
void GemmaReplyTag::SetFlagReply(uint32_t flagReply) { m_flagReply = flagReply; }
uint32_t GemmaReplyTag::GetFlagReply(void) const { return m_flagReply; }
void GemmaReplyTag::SetPathId(uint32_t pathId) { m_pathId = pathId; }
uint32_t GemmaReplyTag::GetPathId(void) const { return m_pathId; }
void GemmaReplyTag::SetPhase(uint32_t phase) { m_phase = phase; }
uint32_t GemmaReplyTag::GetPhase(void) const { return m_phase; }
void GemmaReplyTag::SetRank(uint32_t rank) { m_rank = rank; }
uint32_t GemmaReplyTag::GetRank(void) const { return m_rank; }
void GemmaReplyTag::SetVersion(uint32_t version) { m_version = version; }
uint32_t GemmaReplyTag::GetVersion(void) const { return m_version; }
TypeId GemmaReplyTag::GetInstanceTypeId(void) const { return GetTypeId(); }
uint32_t GemmaReplyTag::GetSerializedSize(void) const {
    return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
}
void GemmaReplyTag::Serialize(TagBuffer i) const {
    i.WriteU32(m_flagReply);
    i.WriteU32(m_phase);
    i.WriteU32(m_pathId);
    i.WriteU32(m_rank);
    i.WriteU32(m_version);
}
void GemmaReplyTag::Deserialize(TagBuffer i) {
    m_flagReply = i.ReadU32();
    m_phase = i.ReadU32();
    m_pathId = i.ReadU32();
    m_rank = i.ReadU32();
    m_version = i.ReadU32();
}
void GemmaReplyTag::Print(std::ostream &os) const {
    os << "m_flagReply=" << m_flagReply;
    os << "m_phase=" << m_phase;
}

/*---------------- GemmaRouting ---------------*/
// debugging to check timing
uint64_t GemmaRouting::debug_time = 0;

// static members for topology information and statistics
// uint64_t GemmaRouting::m_nReplyInitSent = 0;
// uint64_t GemmaRouting::m_nReplyTailSent = 0;
// uint64_t GemmaRouting::m_nTimelyInitReplied = 0;
// uint64_t GemmaRouting::m_nTimelyTailReplied = 0;
// uint64_t GemmaRouting::m_nNotifySent = 0;
// uint64_t GemmaRouting::m_nReRoute = 0;
// uint64_t GemmaRouting::m_nOutOfOrderPkts = 0;
// uint64_t GemmaRouting::m_nFlushVOQTotal = 0;
// uint64_t GemmaRouting::m_nFlushVOQByTail = 0;
// std::vector<uint32_t> GemmaRouting::m_historyVOQSize;

uint32_t GemmaRouting::m_RankChangeNum = 0;
uint32_t GemmaRouting::m_ReviceReplyNum = 0;
uint32_t GemmaRouting::m_PortPausedNum = 0;
uint32_t GemmaRouting::m_RerouteNum = 0;
uint32_t GemmaRouting::m_packetNum = 0;
uint32_t GemmaRouting::m_maxRank = 0;
std::set<uint32_t> GemmaRouting::queueIdSet;
uint32_t GemmaRouting::m_outOfOrderPkts = 0;
uint32_t GemmaRouting::m_nNotifySent = 0;
uint32_t GemmaRouting::m_unusedRerouted = 0;
std::map<uint64_t, uint32_t> GemmaRouting::m_RankChangeMap;

// functions
GemmaRouting::GemmaRouting() {
    m_isToR = false;
    m_switch_id = (uint32_t)-1;

    
    m_agingTime = MilliSeconds(2);                // 2ms
    m_recirculatingTime = NanoSeconds(500);       // 500ns
    m_entryPauseTime = MicroSeconds(6);            // 队列的暂停时间
    m_extraVOQFlushTime = MicroSeconds(8);        // for uncertainty
    m_defaultVOQWaitingTime = MicroSeconds(200);  // 200us

    m_reroutedRank = 100;
    // set constants
    m_congestionRankGap = 500;   // 500ns
}

GemmaRouting::~GemmaRouting() {}
void GemmaRouting::DoDispose() { 
    m_agingEvent.Cancel(); 
    m_recirculatingEvent.Cancel();
}
TypeId GemmaRouting::GetTypeId(void) {
    static TypeId tid =
        TypeId("ns3::GemmaRouting").SetParent<Object>().AddConstructor<GemmaRouting>();
    return tid;
}

uint32_t GemmaRouting::GetOutPortFromPath(const uint32_t &path, const uint32_t &hopCount) {
    return ((uint8_t *)&path)[hopCount];
}

void GemmaRouting::SetOutPortToPath(uint32_t &path, const uint32_t &hopCount,
                                       const uint32_t &outPort) {
    ((uint8_t *)&path)[hopCount] = outPort;
}

uint64_t GemmaRouting::GetFlowKey(uint32_t ip1, uint32_t ip2, uint16_t port1, uint16_t port2) {
    /** IP_ADDRESS: 11.X.X.1 */
    assert(((ip1 & 0xff000000) >> 24) == 11);
    assert(((ip2 & 0xff000000) >> 24) == 11);
    assert((ip1 & 0x000000ff) == 1);
    assert((ip2 & 0x000000ff) == 1);
    uint32_t dstToRId = Settings::hostIp2SwitchId[ip2];
    uint64_t ret = 0;
    ret += uint64_t((ip1 & 0x00ffff00) >> 8) + uint64_t((uint32_t)port1 << 16);
    // ret += uint64_t((ip1 & 0x00ffff00) >> 8) + uint64_t((uint32_t)0 << 16);
    ret = ret << 8;
    // ret += uint64_t((ip2 & 0x00ffff00) >> 8) + uint64_t((uint32_t)port2 << 16);
    ret += (dstToRId%256);
    //ret += port1 % 10;
    // if (Settings::flowKeyNum[ret] == 0) {
    //     std::cout << ip1 << " " << dstToRId << " " << port1 << " " << ip2 << " " << port2 << std::endl;
    // }
    //Settings::flowKeyNum[ret] ++;
    //Settings::maxRepeatFlowNum = std::max(Settings::maxRepeatFlowNum, Settings::flowKeyNum[ret]);
    return ret;
}

uint64_t GemmaRouting::GetFlowPhaseKey(uint64_t flowKey, uint32_t phase) {
    return (flowKey<<16) + phase;
}

uint64_t GemmaRouting::GetSwitchPortKey(uint32_t switchNum, uint16_t port) {
    return (switchNum<<16)+port;
}
uint32_t GemmaRouting::GetPathFlightNum(uint32_t srcId, uint16_t portId) {   // 获取交换机和指定端口上的路径的数据包
    uint64_t switchPortKey = GetSwitchPortKey(srcId, portId);
    // std::cout << "目前是在交换机和端口为:" << srcId << " " << portId << " " << switchPortKey << std::endl;
    // std::cout << "发送和接收的数据包分别为:" << Settings::m_sendPakcetNum[switchPortKey] << " " << Settings::m_receivePacketNum[switchPortKey] << std::endl;
    return Settings::m_sendPakcetNum[switchPortKey] - Settings::m_receivePacketNum[switchPortKey];
}
uint32_t GemmaRouting::GetPathScore(uint32_t srcId, uint32_t pathId) {   // 获取交换机和指定端口上的路径的数据包
    
    uint32_t outDev1 = GetOutPortFromPath(pathId, 0); 
    uint32_t outDev2 = GetOutPortFromPath(pathId, 1);
    uint32_t coreId = Settings::IdPortToId[GetSwitchPortKey(srcId, outDev1)];
    // std::cout << "中间交换机的id和端口为:" << coreId << " " << outDev2 << std::endl;
    uint32_t flightNum1 = GetPathFlightNum(m_switch_id, outDev1); 
    uint32_t flightNum2 = GetPathFlightNum(coreId, outDev2);
    // std::cout << "此时路径上的数据包数量为:" << flightNum1 << " " << flightNum2 << std::endl;
    return flightNum1*3 + flightNum2;
}

uint32_t GemmaRouting::NewGetPathScore(uint32_t srcId, uint32_t pathId) {   // 获取交换机和指定端口上的路径的数据包
    // uint8_t path[4];
    // path = (uint8_t *)&pathId;
    uint64_t score = 0;
    uint32_t outDev1 = GetOutPortFromPath(pathId, 0); 
    uint32_t flightNum1 = GetPathFlightNum(srcId, outDev1); 
    uint32_t nowId = srcId;
    if (((uint8_t *)&pathId)[2] != 0) {
        score += flightNum1 * 4;
        for (int i = 0; i < 4; i++) {
            uint32_t outDev = GetOutPortFromPath(pathId, i);
            uint32_t nextId = Settings::IdPortToId[GetSwitchPortKey(nowId, outDev)];
            if (i != 0) {
                if (i == 1 || i == 2) score += m_downPathScore[GetSwitchPortKey(nowId, outDev)];
                else {
                    score += m_downPathScore[GetSwitchPortKey(nowId, outDev)];
                }
            }
            // std::cout << nextId << " " << m_downPathScore[GetSwitchPortKey(nowId, outDev)] << std::endl;
            nowId = nextId;
        }
    } else {
        score += flightNum1 * 3;
        uint32_t outDev2 = GetOutPortFromPath(pathId, 1);
        uint32_t coreId = Settings::IdPortToId[GetSwitchPortKey(srcId, outDev1)];
        score += m_downPathScore[GetSwitchPortKey(coreId, outDev2)];
    }
    return score;
}

uint32_t GemmaRouting::GetLocalPathScore(uint32_t srcId, uint32_t pathId) {
    uint64_t score = 0;
    uint32_t outDev1 = GetOutPortFromPath(pathId, 1); 
    uint32_t flightNum1 = GetPathFlightNum(srcId, outDev1); 
    score += flightNum1 * 4;
    uint32_t outDev2 = GetOutPortFromPath(pathId, 2);
    uint32_t coreId = Settings::IdPortToId[GetSwitchPortKey(srcId, outDev1)];
    score += m_downPathScore[GetSwitchPortKey(coreId, outDev2)];
    // std::cout << "计算本地分数: " << flightNum1 << " " << m_downPathScore[GetSwitchPortKey(coreId, outDev2)] << std::endl;
    return score;
}


uint32_t GemmaRouting::DoHash(const uint8_t *key, size_t len, uint32_t seed) {
    uint32_t h = seed;
    if (len > 3) {
        const uint32_t *key_x4 = (const uint32_t *)key;
        size_t i = len >> 2;
        do {
            uint32_t k = *key_x4++;
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
            h = (h << 13) | (h >> 19);
            h += (h << 2) + 0xe6546b64;
        } while (--i);
        key = (const uint8_t *)key_x4;
    }
    if (len & 3) {
        size_t i = len & 3;
        uint32_t k = 0;
        key = &key[i - 1];
        do {
            k <<= 8;
            k |= *key--;
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

void GemmaRouting::SendReply(Ptr<Packet> p, CustomHeader &ch, uint32_t flagReply,
                                uint32_t pkt_pathId, uint32_t pathRank, uint32_t pathVersion) {
    qbbHeader seqh;
    seqh.SetSeq(0);
    seqh.SetPG(ch.udp.pg);
    seqh.SetSport(ch.udp.dport);
    seqh.SetDport(ch.udp.sport);
    seqh.SetIntHeader(ch.udp.ih);

    Ptr<Packet> replyP = Create<Packet>(
        std::max(64 - 14 - 20 - (int)seqh.GetSerializedSize(), 0));  // at least 64 Bytes
    replyP->AddHeader(seqh);                                         // qbbHeader

    // ACK-like packet, no L4 header
    Ipv4Header ipv4h;
    ipv4h.SetSource(Ipv4Address(ch.dip));
    ipv4h.SetDestination(Ipv4Address(ch.sip));
    ipv4h.SetProtocol(0xFD);  // (N)ACK - (IRN)
    ipv4h.SetTtl(64);
    ipv4h.SetPayloadSize(replyP->GetSize());
    ipv4h.SetIdentification(UniformVariable(0, 65536).GetValue());
    replyP->AddHeader(ipv4h);  // ipv4Header

    PppHeader ppp;
    ppp.SetProtocol(0x0021);  // EtherToPpp(0x800), see point-to-point-net-device.cc
    replyP->AddHeader(ppp);   // pppHeader

    // attach slbControlTag
    GemmaReplyTag gemmaReplyTag;
    gemmaReplyTag.SetFlagReply(flagReply);
    gemmaReplyTag.SetPathId(pkt_pathId);
    gemmaReplyTag.SetRank(pathRank);
    gemmaReplyTag.SetVersion(pathVersion);
    if (flagReply == GemmaReplyTag::DATA) {
        gemmaReplyTag.SetPhase(0);
    } else if (flagReply == GemmaReplyTag::TAIL) {
        gemmaReplyTag.SetPhase(1);
    } else {
        SLB_LOG("ERROR - Unknown GemmaReplyTag flag:" << flagReply);
        exit(1);
    }

    replyP->AddPacketTag(gemmaReplyTag);

    // dummy reply's inDev interface
    replyP->AddPacketTag(FlowIdTag(Settings::Gemma_CTRL_DUMMY_INDEV));

    // extract customheader
    CustomHeader replyCh(CustomHeader::L2_Header | CustomHeader::L3_Header |
                         CustomHeader::L4_Header);
    replyP->PeekHeader(replyCh);

    // send reply packets
    SLB_LOG(PARSE_FIVE_TUPLE(ch) << "================================### Send REPLY"
                                 << ",ReplyFlag:" << flagReply);
    DoSwitchSendToDev(replyP, replyCh);  // will have ACK's priority
    return;
}
void GemmaRouting::SendNotify(Ptr<Packet> p, CustomHeader &ch, uint32_t pathId, uint64_t switchPortKey, uint32_t receivePacketNum,
     uint32_t pathScore) {
    qbbHeader seqh;
    seqh.SetSeq(0);
    seqh.SetPG(ch.udp.pg);
    seqh.SetSport(ch.udp.dport);
    seqh.SetDport(ch.udp.sport);
    seqh.SetIntHeader(ch.udp.ih);

    Ptr<Packet> fbP = Create<Packet>(
        std::max(64 - 14 - 20 - (int)seqh.GetSerializedSize(), 0));  // at least 64 Bytes
    fbP->AddHeader(seqh);                                            // qbbHeader

    // ACK-like packet, no L4 header
    Ipv4Header ipv4h;
    ipv4h.SetSource(Ipv4Address(ch.dip));
    ipv4h.SetDestination(Ipv4Address(ch.sip));
    ipv4h.SetProtocol(0xFD);  // (N)ACK - (IRN)
    ipv4h.SetTtl(64);
    ipv4h.SetPayloadSize(fbP->GetSize());
    ipv4h.SetIdentification(UniformVariable(0, 65536).GetValue());
    fbP->AddHeader(ipv4h);  // ipv4Header

    PppHeader ppp;
    ppp.SetProtocol(0x0021);  // EtherToPpp(0x800), see point-to-point-net-device.cc
    fbP->AddHeader(ppp);      // pppHeader

    // attach GemmaNotifyTag
    GemmaNotifyTag gemmaNotifyTag;
    gemmaNotifyTag.SetPathId(pathId);
    gemmaNotifyTag.SetSwitchPortKey(switchPortKey);
    gemmaNotifyTag.SetSwitchPortKeyB(0);
    gemmaNotifyTag.SetSwitchPortKeyC(0);
    gemmaNotifyTag.SetReceivePacketNum(receivePacketNum);
    gemmaNotifyTag.SetPathScore(pathScore);
    fbP->AddPacketTag(gemmaNotifyTag);

    // dummy notify's inDev interface
    fbP->AddPacketTag(FlowIdTag(Settings::Gemma_CTRL_DUMMY_INDEV));

    // extract customheader
    CustomHeader fbCh(CustomHeader::L2_Header | CustomHeader::L3_Header | CustomHeader::L4_Header);
    fbP->PeekHeader(fbCh);

    /** OVERHEAD: reply overhead statistics **/
    GemmaRouting::m_nNotifySent += 1;
    // send notify packets
    SLB_LOG(PARSE_FIVE_TUPLE(ch) << "================================### Send NOTIFY");
    DoSwitchSendToDev(fbP, fbCh);  // will have ACK's priority
    return;
}

/** MAIN: Every SLB packet is hijacked to this function at switches */
void GemmaRouting::RouteInput(Ptr<Packet> p, CustomHeader &ch) {
    // Packet arrival time
    
    Time now = Simulator::Now();
    // Turn on aging event scheduler if it is not running
    if (!m_agingEvent.IsRunning()) {
        SLB_LOG("Gemma routing restarts aging event scheduling:" << m_switch_id << now);
        m_agingEvent = Simulator::Schedule(m_agingTime, &GemmaRouting::AgingEvent, this);
    }
    if (!m_recirculatingEvent.IsRunning()) {
        m_recirculatingEvent = Simulator::Schedule(m_recirculatingTime, &GemmaRouting::RecirculatingEvent, this);
    }

    // get srcToRId, dstToRId
    assert(Settings::hostIp2SwitchId.find(ch.sip) !=
           Settings::hostIp2SwitchId.end());  // Misconfig of Settings::hostIp2SwitchId - sip
    assert(Settings::hostIp2SwitchId.find(ch.dip) !=
           Settings::hostIp2SwitchId.end());  // Misconfig of Settings::hostIp2SwitchId - dip
    uint32_t srcToRId = Settings::hostIp2SwitchId[ch.sip];
    uint32_t dstToRId = Settings::hostIp2SwitchId[ch.dip];

    /** FILTER: Quickly filter intra-pod traffic */
    if (srcToRId == dstToRId) {  // do normal routing (only one path)
        DoSwitchSendToDev(p, ch);
        return;
    }
    assert(srcToRId != dstToRId);  // Should not be in the same pod

    if (ch.l3Prot != 0x11 && ch.l3Prot != 0xFD) {
        SLB_LOG(PARSE_FIVE_TUPLE(ch) << "ACK/PFC or other control pkts -> do flow-ECMP. Sw("
                                     << m_switch_id << "),l3Prot:" << ch.l3Prot);
        DoSwitchSendToDev(p, ch);
        return;
    }
    assert(ch.l3Prot == 0x11 || ch.l3Prot == 0xFD);  // Only supports UDP (data) or (N)ACK packets

    // get gemmaDataTag from packet
    GemmaDataTag gemmaDataTag;
    bool foundGemmaDataTag = p->PeekPacketTag(gemmaDataTag);

    // get SlbControlTag from packet
    GemmaReplyTag gemmaReplyTag;
    bool foundGemmaReplyTag = p->PeekPacketTag(gemmaReplyTag);

    // get conweaveNotifyTag from packet
    GemmaNotifyTag gemmaNotifyTag;
    bool foundGemmaNotifyTag = p->PeekPacketTag(gemmaNotifyTag);
    // if (foundGemmaNotifyTag) {
    //     std::cout << m_switch_id << " 收到了通知数据包" << std::endl;
    // }
    if (ch.l3Prot == 0xFD) { /** ACK or Gemma's Control Packets */
        /** NOTE: Gemma uses 0xFD protocol id for its control packets.
         * Quick filter purely (N)ACK packets (not Gemma control packets)
         **/
        if (!foundGemmaReplyTag && !foundGemmaNotifyTag) {  // pure-(N)ACK
            if (m_switch_id == srcToRId) {
                SLB_LOG(PARSE_FIVE_TUPLE(ch)
                        << "[TxToR/*PureACK] Sw(" << m_switch_id << "),ACK detected");
            }
            if (m_switch_id == dstToRId) {
                SLB_LOG(PARSE_FIVE_TUPLE(ch)
                        << "[RxToR/*PureACK] Sw(" << m_switch_id << "),ACK detected");
            }
            DoSwitchSendToDev(p, ch);
            return;
        }

        /** NOTE: Gemma's control packets are forwarded with default flow-ECMP */
        // if (!m_isToR) {
        //     SLB_LOG(PARSE_FIVE_TUPLE(ch) << "Gemma Ctrl Pkts use flow-ECMP at non-ToR switches");
        //     DoSwitchSendToDev(p, ch);
        //     return;
        // }
    }

    // print every 1ms for logging
    if (Simulator::Now().GetMilliSeconds() > debug_time) {
        std::cout << "[Logging] Current time: " << Simulator::Now() << std::endl;
        debug_time = Simulator::Now().GetMilliSeconds();
    }

    /**------------------------  Start processing SLB packets  ------------------------*/
    if (m_isToR) {  // ToR switch
        /***************************
         *    Source ToR Switch    *
         ***************************/
        if (m_switch_id == srcToRId) {  // SrcToR -
            assert(ch.l3Prot == 0x11);  // Only UDP (Data) - TxToR can see only UDP (DATA) packets
            assert(!foundGemmaDataTag && !foundGemmaReplyTag && !foundGemmaNotifyTag);  // dataTag and replyTag should not be found at TxToR
            GemmaRouting::m_packetNum ++;
            /** PIPELINE: emulating the p4 pipelining */
            gemmaTxMeta tx_md;  // SrcToR packet metadata
            tx_md.pkt_flowkey = GetFlowKey(ch.sip, ch.dip, ch.udp.sport, ch.udp.dport);
            auto &txEntry = m_gemmaTxTable[tx_md.pkt_flowkey];

            /** INIT: initialize flowkey */
            if (txEntry._flowkey == 0) { /* if new */
                m_RankChangeMap[tx_md.pkt_flowkey] = 2;
                txEntry._flowkey = tx_md.pkt_flowkey;
                tx_md.newConnection = true;
            }

            txEntry._activeTime = now;  // record the entry's last-accessed time
            uint32_t qIndex = ch.udp.pg;    // 获取当前转发的队列号
            std::set<uint32_t> pathSet = m_GemmaRoutingTable[dstToRId]; 
                        /** PATH: update and get current path */
            /** NOTE: if new connection, set initial random path */
            if (txEntry._pathId == CW_DEFAULT_32BIT) {
                assert(tx_md.newConnection == true);
                tx_md.foundGoodPath = true;
                
                auto it = pathSet.begin();
                tx_md.goodPath = *it;
                tx_md.goodPath = *(std::next(pathSet.begin(), rand() % pathSet.size()));
                // int nowScore = NewGetPathScore(m_switch_id, tx_md.foundGoodPath);
                // while (it != pathSet.end()) {
                //     uint32_t path = *it;
                //     uint32_t tmpOutDev = GetOutPortFromPath(path, 0);
                //     it ++;
                //     //std::cout << "pathId为:" << path << " 拥塞程度为:" << m_txPathCongestion[path] << " 端口为:" << tmpOutDev
                //     //<< "是否被暂停" << paused[tmpOutDev][qIndex] << std::endl;
                //     if (paused[tmpOutDev][qIndex]) {
                //         continue;
                //     }
                //     uint32_t tmpPathScore = NewGetPathScore(m_switch_id, path);
                //     // if (tmpPathScore < m_reroutedRank || tmpPathScore < pathScore - 100) {
                //     //     if (nowScore == -1) {
                //     //         tx_md.goodPath = path;
                //     //         nowScore = tmpPathScore;
                //     //     }else if (tmpPathScore > nowScore) {
                //     //         tx_md.goodPath = path;
                //     //         nowScore = tmpPathScore;
                //     //     }
                //     // }
                //     if (tmpPathScore < nowScore) {
                //         tx_md.goodPath = path;
                //         nowScore = tmpPathScore;
                //     }
                // }
                txEntry._pathId = tx_md.goodPath;
                txEntry._phase = 0;
                // std::cout << "流第一次抵达:" << tx_md.pkt_flowkey << " 挑选的路径的分数为:" <<  NewGetPathScore(m_switch_id, txEntry._pathId)
                //     << " " << GetPathScore(m_switch_id, txEntry._pathId) << std::endl;
            }
            tx_md.currPath = txEntry._pathId;
            tx_md.prePath = txEntry._prePathId;
            tx_md.phase = txEntry._phase;
            
            bool isFoundNewPath = false;
            uint32_t outDev = GetOutPortFromPath(tx_md.currPath, 0);  // 获取即将要转发的端口号
            // uint32_t pathScore = GetPathScore(m_switch_id, tx_md.currPath);
            uint32_t pathScore = NewGetPathScore(m_switch_id, tx_md.currPath);
            
            if (pathScore > m_reroutedRank && !txEntry.is_reRoute) {
            //if (pathScore > m_reroutedRank) {
                // std::cout << tx_md.pkt_flowkey << " 太拥堵了,当前这条路径的分数为:" << GetPathScore(m_switch_id, tx_md.currPath) << " " << NewGetPathScore(m_switch_id, tx_md.currPath) << std::endl;
                // if (pathScore < m_reroutedRank && paused[outDev][qIndex]) {
                //     std::cout << "当前分数小于阈值，但是端口还是被暂停了" << std::endl;
                // }
                GemmaRouting::m_PortPausedNum ++; 
                tx_md.goodPath = txEntry._pathId;
                int nowScore = pathScore;
                auto it = pathSet.begin();
                while (it != pathSet.end()) {
                    uint32_t path = *it;
                    uint32_t tmpOutDev = GetOutPortFromPath(path, 0);
                    it ++;
                    //std::cout << "pathId为:" << path << " 拥塞程度为:" << m_txPathCongestion[path] << " 端口为:" << tmpOutDev
                    //<< "是否被暂停" << paused[tmpOutDev][qIndex] << std::endl;
                    if (paused[tmpOutDev][qIndex]) {
                        continue;
                    }
                    uint32_t tmpPathScore = NewGetPathScore(m_switch_id, path);
                    if (tmpPathScore < nowScore) {
                        tx_md.goodPath = path;
                        nowScore = tmpPathScore;
                    }
                }
                
                // if (nowScore < m_reroutedRank || nowScore < pathScore - m_reroutedRank || (paused[outDev][qIndex] && tx_md.goodPath != txEntry._pathId)) {   // 找到分数最低的那条路径
                    // if (pathScore < m_reroutedRank && paused[outDev][qIndex]) {
                    //     std::cout << "找到新路了" << " " << pathScore << " " << nowScore << std::endl;
                    // }
                if (nowScore < m_reroutedRank || nowScore < pathScore - m_reroutedRank) {
                    isFoundNewPath = true;
                }
                if (isFoundNewPath) {
                    uint32_t score1 = NewGetPathScore(srcToRId, tx_md.goodPath);
                    // if ((score1 < score2 &&  score1 + 50 < score2) || (score1 > score2 && score1 - score2 > 50)) {
                    if (nowScore > 100) {
                        // std::cout << tx_md.pkt_flowkey << " 找寻新路径成功,新路径的分数为:" << nowScore << std::endl;
                    }
                    //     std::cout << "差值过大" << std::endl;
                    // }
                    txEntry._prePathId = txEntry._pathId;   //记录原来的路径
                    txEntry._pathId = tx_md.goodPath;   // 最新的路径
                    txEntry._phase = txEntry._phase + 1;  // 阶段+1
                    m_pathInvaildTime[tx_md.pkt_flowkey] = now + m_entryPauseTime;
                    txEntry.is_reRoute = true;
                    GemmaRouting::m_RerouteNum ++;
                } else {
                   // std::cout << "找寻新路径失败" << std::endl;
                }
            }
            gemmaDataTag.SetIsFirst(txEntry.isFirst);
            if (txEntry.isFirst == 1) {
                txEntry.isFirst = 0;
            }
            gemmaDataTag.SetPathId(tx_md.currPath);
            gemmaDataTag.SetHopCount(0);
            gemmaDataTag.SetPhase(tx_md.phase);
            gemmaDataTag.SetTimestampTx(now.GetNanoSeconds());

            if (isFoundNewPath) {
                gemmaDataTag.SetFlagData(GemmaDataTag::TAIL);
                //std::cout << "FlowId为" << tx_md.pkt_flowkey << "发送了Tail数据包 阶段为:" << tx_md.phase << std::endl;
                txEntry.isFirst = 1;    // 发完tail下一个数据包就是新路径的第一个数据包
            } else {
                gemmaDataTag.SetFlagData(GemmaDataTag::DATA);
            }
            gemmaDataTag.SetPathCongestionRank(m_txPathCongestion[tx_md.currPath]);
            gemmaDataTag.SetPrePathCongestionRank(m_txPathCongestion[tx_md.prePath]);
            gemmaDataTag.SetWaitedLoop(0);
            
            //GemmaRouting::queueIdSet.insert(qIndex);
            outDev = GetOutPortFromPath(tx_md.currPath, 0);  // 获取当前的端口号
            uint64_t switchPortKey = GetSwitchPortKey(m_switch_id, outDev);
            // std::cout << "发送方交换机执行发送数据包操作,端口号为" << m_switch_id << " " << outDev  << " " << switchPortKey << std::endl;
            Settings::m_sendPakcetNum[switchPortKey] ++;
            gemmaDataTag.SetSwitchPortKey(switchPortKey);
            p->AddPacketTag(gemmaDataTag);
            DoSwitchSend(p, ch, outDev, qIndex);
            return;
        }
        /***************************
         *  Destination ToR Switch *
         ***************************/
        else if (m_switch_id == dstToRId) {
            if (foundGemmaDataTag) {  // DATA
                /** PIPELINE: emulating the p4 pipelining */
                uint64_t switchPortKey = gemmaDataTag.GetSwitchPortKey();
                // if (Settings::m_receivePacketNum[switchPortKey] == 0) {
                //     std::cout << "接受方交换机收到了来自交换机和端口分别为:" << switchPortKey << " " << (switchPortKey>>16) << " " << (switchPortKey%(1<<16)) << "的数据包" << std::endl;
                // }
                Settings::m_receivePacketNum[switchPortKey] ++;   // 收到的信息数量加1
                // m_receivePacketNum[switchPortKey] ++;
                // if (m_receivePacketNum[switchPortKey] % 10 == 1) {
                //     uint32_t switchNum = switchPortKey>>16;
                //     std::cout << m_switch_id << " 给交换机:" << switchNum << "发送响应数据包" << std::endl;
                //     SendNotify(p, ch, m_receivePacketNum[switchPortKey], switchPortKey);
                // }
                gemmaRxMeta rx_md;
                rx_md.pkt_flowkey = GetFlowKey(ch.sip, ch.dip, ch.udp.sport, ch.udp.dport);
                // if (rx_md.pkt_flowkey == 171597367334) {
                //     std::cout << "接收方交换机收到数据包:" << rx_md.pkt_flowkey << " " << gemmaDataTag.GetStage() << " " 
                //         << gemmaDataTag.GetPhase() << std::endl;
                // }
                auto &rxEntry = m_gemmaRxTable[rx_md.pkt_flowkey];

                /** INIT: setup flowkey */
                if (rxEntry._flowkey == 0) {
                    //std::cout << "流第一次抵达:" << rx_md.pkt_flowkey  << std::endl;
                    rxEntry._flowkey = rx_md.pkt_flowkey;
                    rxEntry._phase = 0;
                    rx_md.newConnection = true;
                    // auto vec = m_ipTable[ch.dip];
                    // std::cout << "目前在交换机上: " << m_switch_id << " 要去的ip为:" << ch.dip << std::endl;
                    // for (auto i : vec) {
                    //     std::cout << i << " ";
                    // }
                    // std::cout << std::endl;
                }

                /**
                 * ACTIVE: update active time (for aging)
                 */
                rxEntry._activeTime = now;

                /**
                 * PARSING: parse packet's GemmaDataTag
                 */
                rx_md.pkt_pathId = gemmaDataTag.GetPathId();
                rx_md.pkt_timestamp_Tx = gemmaDataTag.GetTimestampTx();
                rx_md.pkt_flagData = gemmaDataTag.GetFlagData();
                rx_md.pkt_phase = gemmaDataTag.GetPhase();
                rx_md.pkt_ecnbits = ch.GetIpv4EcnBits(); /* ECN bits */

                uint64_t nowTime = Simulator::Now().GetNanoSeconds();
                uint64_t gapTime = nowTime - rx_md.pkt_timestamp_Tx;
                uint32_t rank = gapTime / m_congestionRankGap + 1;
                if (1) {  // 尚未经过内循环的
                    GemmaRouting::m_maxRank = std::max(GemmaRouting::m_maxRank, rank);
                    // if (m_rxPathCongestion[rx_md.pkt_pathId] != rank) {
                    //     GemmaRouting::m_RankChangeNum ++;
                    //    // std::cout << "发送pathId为:" << rx_md.pkt_pathId << " 拥塞程度为：" << rank << " 当前交换机的id为:" 
                    //     //  << m_switch_id << " 原来的拥塞程度为:" << m_rxPathCongestion[rx_md.pkt_pathId] <<
                    //     //  " 要去的交换机为:" << Settings::hostIp2SwitchId[ch.sip] << std::endl;
                    //     uint32_t changeRank = rank > m_rxPathCongestion[rx_md.pkt_pathId] ? 
                    //         rank - m_rxPathCongestion[rx_md.pkt_pathId] : m_rxPathCongestion[rx_md.pkt_pathId] - rank;
                    //     // std::cout << "路径拥塞程度发生改变为:" << changeRank << std::endl;
                    //     m_RankChangeMap[changeRank] ++;
                    //     m_rxPathCongestion[rx_md.pkt_pathId] = rank;
                    //     m_rxPathCongestionVersion[rx_md.pkt_pathId] ++;
                    //     SendReply(p, ch, GemmaReplyTag::DATA, rx_md.pkt_pathId, rank, m_rxPathCongestionVersion[rx_md.pkt_pathId]);
                    // }
                }
                if (gemmaDataTag.GetFlagData() == GemmaDataTag::TAIL) { 
                    // std::cout << "flowKey为:" << rx_md.pkt_flowkey << " Tail包花费的时间为:" << gapTime << " 发送时间为:" << 
                    //     gemmaDataTag.GetTimestampTx()  << std::endl;
                    // std::cout << "路径的拥塞程度为:" << gemmaDataTag.GetPathCongestionRank() << std::endl;
                    // std::cout << "目前这条路上的拥塞程度为:" << m_rxPathCongestion[rx_md.pkt_pathId] << std::endl;
                    // std::cout << "接收到了tail数据包,发送Tail的响应数据包" << std::endl;
                    SendReply(p, ch, GemmaReplyTag::TAIL, rx_md.pkt_pathId, rank, m_rxPathCongestionVersion[rx_md.pkt_pathId]);
                }
                // if (gemmaDataTag.GetIsFirst() == 1) {
                //     std::cout << "flowKey为:" << rx_md.pkt_flowkey << " 阶段为:" << rx_md.pkt_phase << " 的第一个数据包到达" << std::endl;
                //     std::cout << "花费的时间为:" << gapTime  << " 发送时间为:" << gemmaDataTag.GetTimestampTx() << std::endl;
                //     std::cout << "路径的拥塞程度为:" << gemmaDataTag.GetPathCongestionRank() << std::endl;
                //     std::cout << "目前这条路上的拥塞程度为:" << m_rxPathCongestion[rx_md.pkt_pathId] << std::endl;
                // }
                uint64_t flowPhaseKey = GetFlowPhaseKey(rx_md.pkt_flowkey, rx_md.pkt_phase);   // 流ID+阶段
                //std::cout << "flowPhaseKey为:" << flowPhaseKey << " flowKey为:" << rx_md.pkt_flowkey << std::endl;
                if (rx_md.pkt_phase > rxEntry._phase) {   // 乱序
                    // if (rx_md.pkt_flowkey == 171597367334) {
                    //     std::cout << "接收方交换机数据包发生乱序了,flowKey为:" << rx_md.pkt_flowkey << " phase为:" << rx_md.pkt_phase << 
                    //         " 交换机的phase为:" << rxEntry._phase << std::endl;
                    // }
                     
                    if (m_voqMap.find(flowPhaseKey) == m_voqMap.end()) {    // 第一次进行初始化
                        //std::cout << "flowKey为:" << rx_md.pkt_flowkey << " 队列开始进行初始化" << std::endl;
                        ConWeaveVOQ &voq = m_voqMap[flowPhaseKey];
                        rx_md.timeExpectedToFlush = now.GetNanoSeconds() + m_defaultVOQWaitingTime.GetNanoSeconds() + m_extraVOQFlushTime.GetNanoSeconds();
                        voq.Set(flowPhaseKey, ch.dip,
                                    NanoSeconds(rx_md.timeExpectedToFlush),
                                    m_extraVOQFlushTime, m_ipTable[ch.dip]); // 设置一个默认的刷新时间
                        voq.m_deleteCallback = MakeCallback(&GemmaRouting::DeleteVOQ, this);
                        voq.m_CallbackByVOQFlush =
                            MakeCallback(&GemmaRouting::CallbackByVOQFlush, this);
                        voq.m_switchSendToDevCallback =
                            MakeCallback(&GemmaRouting::DoSwitchSendToDev, this);
                    }
                    
                    m_voqMap[flowPhaseKey].Enqueue(p);
                    if (gemmaDataTag.GetFlagData() == GemmaDataTag::TAIL) {   // 其他阶段的Tail数据包
                        //std::cout << "其他阶段的Tail到达了 " << rx_md.pkt_phase << " FlowKey为:" << rx_md.pkt_flowkey << std::endl;
                        m_isFlush[GetFlowPhaseKey(rx_md.pkt_flowkey, rx_md.pkt_phase+1)] = true;   // 下一阶段标记可以刷新了
                    }
                    m_outOfOrderPkts++;
                    return;
                } else if (rx_md.pkt_phase == rxEntry._phase && gemmaDataTag.GetFlagData() == GemmaDataTag::TAIL) {
                    auto voq = m_voqMap.find(GetFlowPhaseKey(rx_md.pkt_flowkey, rx_md.pkt_phase+1));   // 刷新下一阶段
                    // if (rx_md.pkt_flowkey == 171597367334) {
                    //     std::cout << "接收方交换机接收到tail,flowKey为:" << rx_md.pkt_flowkey << " phase为:" << rx_md.pkt_phase << 
                    //         " 交换机的phase为:" << rxEntry._phase << std::endl;
                    // }
                    if (voq == m_voqMap.end()) {

                        // std::cout << "flowkey: " << rx_md.pkt_flowkey << " Tail优先到达,不乱顺序" << std::endl;
                        rxEntry._phase = rxEntry._phase + 1;
                        GemmaRouting::m_unusedRerouted ++;
                        // std::cout << "flowkey:" << rx_md.pkt_flowkey << " 更新后交换机上的状态为:" << rxEntry._phase << std::endl;
                    }else {
                        //std::cout << "flowkey: " << rx_md.pkt_flowkey << " Tail到达,开始刷新队列,phase为:" << rx_md.pkt_phase << std::endl;
                        assert(voq != m_voqMap.end());
                        rx_md.timeExpectedToFlush = now.GetNanoSeconds() + 1;
                        rx_md.timeExpectedToFlush = (rx_md.timeExpectedToFlush > now.GetNanoSeconds())
                                ? (rx_md.timeExpectedToFlush - now.GetNanoSeconds()) : 0;
                        voq->second.RescheduleFlush(NanoSeconds(rx_md.timeExpectedToFlush));
                    }
                }
                /**
                 * SEND: send to end-host
                 */
                DoSwitchSendToDev(p, ch);
                return;
            }

            if (foundGemmaReplyTag) {  // Received REPLY
                gemmaTxMeta tx_md;
                tx_md.pkt_flowkey = GetFlowKey(ch.dip, ch.sip, ch.udp.dport, ch.udp.sport);
                tx_md.reply_flag = gemmaReplyTag.GetFlagReply();
                uint32_t pathId = gemmaReplyTag.GetPathId();
                uint32_t pathRank = gemmaReplyTag.GetRank();
                uint32_t pathVersion = gemmaReplyTag.GetVersion();
                auto &txEntry = m_gemmaTxTable[tx_md.pkt_flowkey];
                if (tx_md.reply_flag == GemmaReplyTag::TAIL) {
                   // std::cout << tx_md.pkt_flowkey << " 收到了tail的响应,可以重路由了" << std::endl;
                    txEntry.is_reRoute = false;
                }else {
                    if (pathVersion > m_txPathCongestionVersion[pathId]) {
                        // std::cout << "收到的信息为:" << "pathId = " << pathId << " " << " pathRank = " << pathRank << " 交换机为:" << m_switch_id 
                        // << " 原来的拥塞程度为:" << m_txPathCongestion[pathId] << " 来自的交换机是:" << Settings::hostIp2SwitchId[ch.sip] << std::endl;
                        assert(m_txPathCongestionVersion[pathId] != pathVersion);
                        m_txPathCongestion[pathId] = pathRank;
                        m_txPathCongestionVersion[pathId] = pathVersion;
                        GemmaRouting::m_ReviceReplyNum ++;
                    }
                }
                
                return;  // drop this reply
            }
            if (foundGemmaNotifyTag) {
                m_downPathScore[gemmaNotifyTag.GetSwitchPortKey()] = gemmaNotifyTag.GetPathScore();
                // if (gemmaNotifyTag.GetSwitchPortKeyC() != 0) {
                //     m_downPathScore[gemmaNotifyTag.GetSwitchPortKeyC()] = gemmaNotifyTag.GetPathScoreC();
                // } else {
                //     if (gemmaNotifyTag.GetSwitchPortKeyB() != 0) {
                //         m_downPathScore[gemmaNotifyTag.GetSwitchPortKeyB()] = gemmaNotifyTag.GetPathScoreB();
                //     }
                // }
                return;
            }
        }
    }

    /******************************
     *  Non-ToR Switch (Core/Agg) *
     ******************************/
    
    if (foundGemmaDataTag) {  // UDP with PathId
        uint64_t nowTime = Simulator::Now().GetNanoSeconds();
        receiveNum ++;
        if (receiveNum % 10000 == 0) {
            if (lastTime == 0) lastTime = 2000000000;
            responseNum = CalResponseNum(nowTime-lastTime, nowTime);
            if (Settings::is_fatTree) responseNum *= 3;
            // if (responseNum != 9) std::cout << "此时响应的次数为: " << responseNum << std::endl;
            lastTime = nowTime;
        }
        uint32_t flowKey = GetFlowKey(ch.sip, ch.dip, ch.udp.sport, ch.udp.dport);
        auto &rxEntry = m_gemmaCoreTable[flowKey];
        rxEntry.m_prePathCongestionRank = gemmaDataTag.GetPrePathCongestionRank();
        rxEntry.m_pathCongestionRank = gemmaDataTag.GetPathCongestionRank();
        uint64_t switchPortKey = gemmaDataTag.GetSwitchPortKey();
        Settings::m_receivePacketNum[switchPortKey] ++;   // 收到的信息数量加1
        m_receivePacketNum[switchPortKey] ++; 

        // update hopCount
        uint32_t hopCount = gemmaDataTag.GetHopCount() + 1;
        gemmaDataTag.SetHopCount(hopCount);
        uint32_t qIndex = ch.udp.pg;
        
        bool flag_enqueue = false;
        uint64_t flowPhaseKey;
        if (hopCount == 1 && m_GemmaRoutingTable[dstToRId].size() != 0) { // 发送方的agg且不作为core
            std::set<uint32_t> pathSet = m_GemmaRoutingTable[dstToRId];  // pathSet to RxToR
            // std::cout << "发送agg交换机收到了消息: " << m_switch_id << " --> " << dstToRId << std::endl;
            // std::cout << "当前的路径大小为: " << pathSet.size() << std::endl;
            gemmaTxMeta tx_md;
            tx_md.pkt_flowkey = GetFlowKey(ch.sip, ch.dip, ch.udp.sport, ch.udp.dport);
            auto &txEntry = m_gemmaTxTable[tx_md.pkt_flowkey];

            if (txEntry._flowkey == 0) { /* if new */
                txEntry._flowkey = tx_md.pkt_flowkey;
                txEntry._pathId = gemmaDataTag.GetPathId();
                tx_md.newConnection = true;
            }

            txEntry._activeTime = now;  // record the entry's last-accessed time
            
            tx_md.currPath = txEntry._pathId;
            tx_md.stage = txEntry._phase;
            bool isFoundNewPath = false;
            uint32_t outDev = GetOutPortFromPath(tx_md.currPath, 1);  // 获取即将要转发的端口号
            // // uint32_t pathScore = GetPathScore(m_switch_id, tx_md.currPath);
            uint32_t pathScore = GetLocalPathScore(m_switch_id, tx_md.currPath);
            
            if (pathScore > m_reroutedRank) {
                // std::cout << tx_md.pkt_flowkey << " 太拥堵了,当前这条路径的分数为:" << pathScore << std::endl;
                tx_md.goodPath = tx_md.currPath;
                uint32_t nowScore = pathScore;
                //std::cout << "当前路的分数为:" << nowScore << " " << tx_md.goodPath << std::endl;
                auto it = pathSet.begin();
                while (it != pathSet.end()) {
                    uint32_t path = *it;
                    uint32_t tmpOutDev = GetOutPortFromPath(path, 1);
                    it ++;
                    if (paused[tmpOutDev][qIndex] || path == tx_md.currPath) {
                        continue;
                    }
                    uint32_t tmpPathScore = GetLocalPathScore(m_switch_id, path);
                    //std::cout << "另外一条路的分数为:" << tmpPathScore << " " << path << std::endl;
                    if (tmpPathScore < nowScore) {
                        tx_md.goodPath = path;
                        nowScore = tmpPathScore;
                    }
                }
                if (nowScore < m_reroutedRank || nowScore < pathScore - 100) {   // 找到分数最低的那条路径
                    isFoundNewPath = true;
                }
                if (isFoundNewPath) {
                    // if (nowScore > m_reroutedRank) {
                    //     std::cout << "二次重路由找寻新路径成功,新路径的分数为:" << nowScore << std::endl;
                    // }
                    txEntry._pathId = tx_md.goodPath;   // 最新的路径
                    txEntry._phase = txEntry._phase + 1;  // 阶段+1
                } else {
                   // std::cout << "找寻新路径失败" << std::endl;
                }
            }
            gemmaDataTag.SetPathId(tx_md.currPath);

            if (isFoundNewPath) {
                gemmaDataTag.SetFlagNotify(GemmaDataTag::TAIL);
            } else {
                gemmaDataTag.SetFlagNotify(GemmaDataTag::DATA);
            }
            // gemmaDataTag.SetPathCongestionRank(m_txPathCongestion[tx_md.currPath]);
            // gemmaDataTag.SetPrePathCongestionRank(m_txPathCongestion[tx_md.prePath]);
            // gemmaDataTag.SetWaitedLoop(0);
            gemmaDataTag.SetStage(tx_md.stage);
        }else if (hopCount == 3) { // 接收方的agg
            
            uint64_t switchPortKey = gemmaDataTag.GetSwitchPortKey();
            gemmaRxMeta rx_md;
            rx_md.pkt_flowkey = GetFlowKey(ch.sip, ch.dip, ch.udp.sport, ch.udp.dport);
            auto &rxEntry = m_gemmaRxTable[rx_md.pkt_flowkey];
            /** INIT: setup flowkey */
            if (rxEntry._flowkey == 0) {
                //std::cout << "流第一次抵达:" << rx_md.pkt_flowkey  << std::endl;
                rxEntry._flowkey = rx_md.pkt_flowkey;
                rxEntry._phase = 0;
                rx_md.newConnection = true;
            }
            
            rxEntry._activeTime = now;
            /**
             * PARSING: parse packet's GemmaDataTag
             */
            rx_md.pkt_pathId = gemmaDataTag.GetPathId();
            rx_md.pkt_flagData = gemmaDataTag.GetFlagNotify();
            rx_md.pkt_stage = gemmaDataTag.GetStage();
            // if (rx_md.pkt_flowkey == 171597367334) {
            //     std::cout << "接收agg交换机收到了消息: " << rx_md.pkt_stage << " " << rx_md.pkt_flagData << " " << m_switch_id << std::endl;
            // }
            
            flowPhaseKey = GetFlowPhaseKey(rx_md.pkt_flowkey, rx_md.pkt_stage);   // 流ID+阶段
            //std::cout << "flowPhaseKey为:" << flowPhaseKey << " flowKey为:" << rx_md.pkt_flowkey << std::endl;
            if (rx_md.pkt_stage > rxEntry._phase) {   // 乱序
                // std::cout << "数据包发生乱序了,flowKey为:" << rx_md.pkt_flowkey << " stage为:" << rx_md.pkt_stage << 
                //     " 交换机的phase为:" << rxEntry._phase << std::endl; 
                if (m_voqMap.find(flowPhaseKey) == m_voqMap.end()) {    // 第一次进行初始化
                    // if (rx_md.pkt_flowkey == 171597367334) {
                    //     std::cout << "flowKey为:" << rx_md.pkt_flowkey << " 阶段为:" << rx_md.pkt_stage << " 队列开始进行初始化" << std::endl;
                    // }
                    ConWeaveVOQ &voq = m_voqMap[flowPhaseKey];
                    rx_md.timeExpectedToFlush = now.GetNanoSeconds() + m_defaultVOQWaitingTime.GetNanoSeconds() + m_extraVOQFlushTime.GetNanoSeconds();
                    voq.Set(flowPhaseKey, ch.dip,
                                NanoSeconds(rx_md.timeExpectedToFlush),
                                m_extraVOQFlushTime, m_ipTable[ch.dip]); // 设置一个默认的刷新时间
                    voq.m_deleteCallback = MakeCallback(&GemmaRouting::DeleteVOQ, this);
                    voq.m_CallbackByVOQFlush =
                        MakeCallback(&GemmaRouting::CallbackByVOQFlush, this);
                    voq.m_switchSendToDevCallback =
                        MakeCallback(&GemmaRouting::DoSwitchSendToDev, this);
                }
                
                
                if (rx_md.pkt_flagData == GemmaDataTag::TAIL) {   // 其他阶段的Tail数据包
                    // if (rx_md.pkt_flowkey == 171597367334) {
                    //     std::cout << "其他阶段的Tail到达了 " << rx_md.pkt_stage << " FlowKey为:" << rx_md.pkt_flowkey << std::endl;
                    // }
                    m_isFlush[GetFlowPhaseKey(rx_md.pkt_flowkey, rx_md.pkt_stage+1)] = true;   // 下一阶段标记可以刷新了
                }
                m_outOfOrderPkts++;
                flag_enqueue = true;
            } else if (rx_md.pkt_stage == rxEntry._phase && rx_md.pkt_flagData == GemmaDataTag::TAIL) {
                // if (rx_md.pkt_flowkey == 171597367334) {
                //     std::cout << rx_md.pkt_flowkey << " 接收到了tail数据包" << std::endl;
                // }
                auto voq = m_voqMap.find(GetFlowPhaseKey(rx_md.pkt_flowkey, rx_md.pkt_stage+1));   // 刷新下一阶段
                if (voq == m_voqMap.end()) {
                    rxEntry._phase = rxEntry._phase + 1;
                    GemmaRouting::m_unusedRerouted ++;
                    // if (rx_md.pkt_flowkey == 171597367334) {
                    //     std::cout << "flowkey:" << rx_md.pkt_flowkey << " 更新后交换机上的状态为:" << rxEntry._phase << std::endl;
                    // }
                }else {
                    // if (rx_md.pkt_flowkey == 171597367334) {
                    //     std::cout << "flowkey: " << rx_md.pkt_flowkey << " Tail到达,开始刷新队列,phase为:" << rx_md.pkt_stage << std::endl;
                    // }
                    assert(voq != m_voqMap.end());
                    rx_md.timeExpectedToFlush = now.GetNanoSeconds() + 1;
                    rx_md.timeExpectedToFlush = (rx_md.timeExpectedToFlush > now.GetNanoSeconds())
                            ? (rx_md.timeExpectedToFlush - now.GetNanoSeconds()) : 0;
                    voq->second.RescheduleFlush(NanoSeconds(rx_md.timeExpectedToFlush));
                }
            }
        }
        // get outPort
        uint32_t outDev =
            GetOutPortFromPath(gemmaDataTag.GetPathId(), gemmaDataTag.GetHopCount());
        uint64_t newSwitchPortKey = GetSwitchPortKey(m_switch_id, outDev);
        gemmaDataTag.SetSwitchPortKey(newSwitchPortKey);
        // if (m_receivePacketNum[switchPortKey] % responseNum != m_receivePacketNum[switchPortKey] % 5) {
        //     std::cout << "此时的负载不太对: " << responseNum << std::endl;
        // }
        // if (m_receivePacketNum[switchPortKey] % responseNum == 0) {
        if (m_receivePacketNum[switchPortKey] % responseNum == 0) {
            //std::cout << responseNum << std::endl;
            uint64_t downScore = GetPathFlightNum(m_switch_id, outDev); 
            SendNotify(p, ch, gemmaDataTag.GetPathId(), newSwitchPortKey, m_receivePacketNum[switchPortKey], downScore);
        }
        Settings::m_sendPakcetNum[newSwitchPortKey] ++;
        m_sendPakcetNum[newSwitchPortKey] ++;
        // re-serialize tag
        GemmaDataTag temp_tag;
        p->RemovePacketTag(temp_tag);
        p->AddPacketTag(gemmaDataTag);
        if (flag_enqueue) m_voqMap[flowPhaseKey].Enqueue(p);
        else DoSwitchSend(p, ch, outDev, qIndex);
        return;
    }
    if (foundGemmaNotifyTag) {
        m_downPathScore[gemmaNotifyTag.GetSwitchPortKey()] = gemmaNotifyTag.GetPathScore();
        uint32_t outDev = 0;
        if (m_isAgg) {
            outDev = GetOutPortFromPath(gemmaNotifyTag.GetPathId(), 1); 
        } else {
            outDev = GetOutPortFromPath(gemmaNotifyTag.GetPathId(), 2);
        }
        if (gemmaNotifyTag.GetSwitchPortKeyB() != 0) {
            // std::cout << "收到portB的信息:" << (gemmaNotifyTag.GetSwitchPortKeyB()>>16) << " " << gemmaNotifyTag.GetPathScoreB() << std::endl;
            // m_downPathScore[gemmaNotifyTag.GetSwitchPortKeyB()] = gemmaNotifyTag.GetPathScoreB();
            gemmaNotifyTag.SetSwitchPortKeyC(GetSwitchPortKey(m_switch_id, outDev));
            gemmaNotifyTag.SetPathScoreC(GetPathFlightNum(m_switch_id, outDev));
        }else {
            gemmaNotifyTag.SetSwitchPortKeyB(GetSwitchPortKey(m_switch_id, outDev));
            gemmaNotifyTag.SetPathScoreB(GetPathFlightNum(m_switch_id, outDev));
        }
        GemmaNotifyTag temp_tag;
        p->RemovePacketTag(temp_tag);
        p->AddPacketTag(gemmaNotifyTag);
    }
    // UDP with ECMP
    SLB_LOG("[NonToR/ECMP] Sw(" << m_switch_id << ")," << PARSE_FIVE_TUPLE(ch));
    DoSwitchSendToDev(p, ch);
    return;
}

uint32_t GemmaRouting::CalResponseNum(uint64_t time, uint64_t nowTime) {
    //if (nowTime < 2002000000) return 5;
    // std::cout << "计算的时间为: " << time << std::endl;
    if (time < 220000) return 5;
    else if (time < 400000) return 3;
    else if (time < 500000) return 2;
    return 1;
}

void GemmaRouting::SetConstants(Time extraReplyDeadline, Time extraVOQFlushTime,
                                   Time txExpiryTime, Time defaultVOQWaitingTime,
                                   Time pathPauseTime, bool pathAwareRerouting) {
    // NS_LOG_FUNCTION("Setup new parameters at sw" << m_switch_id);
    // m_extraReplyDeadline = extraReplyDeadline;
    // m_extraVOQFlushTime = extraVOQFlushTime;
    // m_txExpiryTime = txExpiryTime;
    // m_defaultVOQWaitingTime = defaultVOQWaitingTime;
    // m_pathPauseTime = pathPauseTime;
    // m_pathAwareRerouting = pathAwareRerouting;
    // assert(m_pathAwareRerouting);  // by default, path-aware routing
}

void GemmaRouting::SetSwitchInfo(bool isToR, bool isAgg, uint32_t switch_id) {
    m_isToR = isToR;
    m_isAgg = isAgg;
    m_switch_id = switch_id;
}

/** CALLBACK: callback functions  */
void GemmaRouting::DoSwitchSend(Ptr<Packet> p, CustomHeader &ch, uint32_t outDev,
                                   uint32_t qIndex) {
    m_switchSendCallback(p, ch, outDev, qIndex);
}
void GemmaRouting::DoSwitchSendToDev(Ptr<Packet> p, CustomHeader &ch) {
    m_switchSendToDevCallback(p, ch);
}

// used for callback in VOQ
void GemmaRouting::DeleteVOQ(uint64_t flowPhaseKey) { 
    m_voqMap.erase(flowPhaseKey); 
    m_isFlush.erase(flowPhaseKey);
}

void GemmaRouting::CallbackByVOQFlush(uint64_t flowPhaseKey, uint32_t voqSize) {
    //m_historyVOQSize.push_back(voqSize);  // statistics - track VOQ size
    // update RxEntry
    uint64_t flowkey = flowPhaseKey >> 16;    // 右移8得到flowkey
    auto &rxEntry = m_gemmaRxTable[flowkey];  // flowcut entry
    //std::cout << "队列刷新,flowKey为:" << flowkey << " flowPhaseKey为:" << flowPhaseKey
    //    << " 交换机上的flowid为:" << rxEntry._flowkey << " phase为:" << rxEntry._phase << std::endl;
    assert(rxEntry._flowkey == flowkey);         // sanity check
    //assert(rxEntry._reordering == true);         // sanity check
    // rxEntry._reordering = false;
    rxEntry._phase ++;
    
    uint64_t newFlowPhaseKey = GetFlowPhaseKey(flowkey, rxEntry._phase+1);
    if (m_isFlush[newFlowPhaseKey])  {   // 下一阶段的tail已经到达
        // if (flowkey == 171597367334) {
        //     std::cout << "下一阶段的tail已经到达 直接刷新队列 flowKey为:" << flowkey << std::endl;
        // }
        auto voq = m_voqMap.find(newFlowPhaseKey);
        if (voq == m_voqMap.end()) {
            rxEntry._phase ++;
            return;
        }
        voq->second.RescheduleFlush(NanoSeconds(1));
    }
}

void GemmaRouting::SetSwitchSendCallback(SwitchSendCallback switchSendCallback) {
    m_switchSendCallback = switchSendCallback;
}

void GemmaRouting::SetSwitchSendToDevCallback(SwitchSendToDevCallback switchSendToDevCallback) {
    m_switchSendToDevCallback = switchSendToDevCallback;
}

uint32_t GemmaRouting::GetVolumeVOQ() {
    uint32_t nTotalPkts = 0;
    for (auto voq : m_voqMap) {
        nTotalPkts += voq.second.getQueueSize();
    }
    return nTotalPkts;
}

void GemmaRouting::AgingEvent() {
    auto now = Simulator::Now();

    // auto itr1 = m_gemmaTxTable.begin();
    // while (itr1 != m_gemmaTxTable.end()) {
    //     if (now - ((itr1->second)._activeTime) > m_agingTime) {
    //         itr1 = m_gemmaTxTable.erase(itr1);
    //     } else {
    //         ++itr1;
    //     }
    // }

    // auto itr2 = m_gemmaRxTable.begin();
    // while (itr2 != m_gemmaRxTable.end()) {
    //     if (now - ((itr2->second)._activeTime) > m_agingTime) {
    //         itr2 = m_gemmaRxTable.erase(itr2);
    //     } else {
    //         ++itr2;
    //     }
    // }

    // auto itr3 = m_gemmaCoreTable.begin();
    // while (itr3 != m_gemmaCoreTable.end()) {
    //     if (now - ((itr3->second)._activeTime) > m_agingTime) {
    //         itr3 = m_gemmaCoreTable.erase(itr3);
    //     } else {
    //         ++itr3;
    //     }
    // }

    m_agingEvent = Simulator::Schedule(m_agingTime, &GemmaRouting::AgingEvent, this);
}



void GemmaRouting::RecirculatingEvent() {
    std::queue<Ptr<Packet> > tmpRecirculationQueue; 
    if (!recirculationQueue.empty()) {
       //std::cout << "有数据包进行交换机内循环" << std::endl;
    }
    while (!recirculationQueue.empty()) {
        Ptr<Packet> p = recirculationQueue.front();
        recirculationQueue.pop();
        GemmaDataTag gemmaDataTag;
        bool foundGemmaDataTag = p->PeekPacketTag(gemmaDataTag);
        assert(foundGemmaDataTag);
        CustomHeader ch(CustomHeader::L2_Header | CustomHeader::L3_Header |
                        CustomHeader::L4_Header);
        p->PeekHeader(ch);
        uint32_t flowKey = GetFlowKey(ch.sip, ch.dip, ch.udp.sport, ch.udp.dport);
        auto &rxEntry = m_gemmaCoreTable[flowKey];
        uint32_t CongestionGap = rxEntry.m_prePathCongestionRank > rxEntry.m_pathCongestionRank ?
                                    rxEntry.m_prePathCongestionRank - rxEntry.m_pathCongestionRank : 0;
        gemmaDataTag.SetWaitedLoop(gemmaDataTag.GetWaitedLoop() + 1);
        // re-serialize tag
        GemmaDataTag temp_tag;
        p->RemovePacketTag(temp_tag);
        p->AddPacketTag(gemmaDataTag);
        // std::cout << "拥塞程度差值为:" << CongestionGap << " " << " 等待时间为:" << gemmaDataTag.GetWaitedLoop() << std::endl;
        // if (CongestionGap <= gemmaDataTag.GetWaitedLoop()) {
        if (1) {
            uint32_t hopCount = gemmaDataTag.GetHopCount() + 1;
            gemmaDataTag.SetHopCount(hopCount);
            uint32_t outDev = GetOutPortFromPath(gemmaDataTag.GetPathId(), gemmaDataTag.GetHopCount());
            uint32_t qIndex = ch.udp.pg;
            //std::cout << "数据包即将转发,等待时间为:" << gemmaDataTag.GetWaitedLoop() * 500 << " 拥塞的差值为:" << CongestionGap << std::endl;
            //std::cout << "内循环转发的信息为:outDev=" << outDev << " qIndex=" << qIndex << std::endl; 
            DoSwitchSend(p, ch, outDev, qIndex);
            //std::cout << "转发成功!" << std::endl;  
        } else {
            tmpRecirculationQueue.push(p);
        }
    }
    while (!tmpRecirculationQueue.empty()) {
        Ptr<Packet> p = tmpRecirculationQueue.front();
        tmpRecirculationQueue.pop();
        recirculationQueue.push(p);
    }
    m_recirculatingEvent = Simulator::Schedule(m_recirculatingTime, &GemmaRouting::RecirculatingEvent, this);
}

}  // namespace ns3