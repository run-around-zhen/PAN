#ifndef __GEMMA_ROUTING_H__
#define __GEMMA_ROUTING_H__

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>

#include "ns3/address.h"
#include "ns3/callback.h"
#include "ns3/conweave-voq.h"
#include "ns3/event-id.h"
#include "ns3/net-device.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/settings.h"
#include "ns3/simulator.h"
#include "ns3/tag.h"

namespace ns3 {

#define CW_DEFAULT_32BIT (UINT32_MAX)
#define CW_DEFAULT_64BIT (Seconds(100).GetNanoSeconds())
#define CW_MAX_TIME (Seconds(100))
#define CW_MIN_TIME (Seconds(0))


/*------ 发送方交换机需要维护的状态 ----*/
struct gemmaTxState {
    uint64_t _flowkey = 0;  // 流id
    bool is_reRoute = false;  // 是否需要重路由
    uint32_t _prePathId = 0; /* 被重新路由之前的路径id集合 */
    uint32_t _pathId = CW_DEFAULT_32BIT;    /* encoded current path ID (composition of uplinks) */
    Time _activeTime;   // 流活动时间
    uint32_t _phase;    // 发送方的转发阶段
    uint32_t isFirst = 0;
};

struct gemmaRxState {
    uint64_t _flowkey = 0;  // 流id
    uint32_t _phase;   // 接受方交换机上的阶段 
    Time _activeTime;   // 流活动时间
};

struct gemmaCoreState {   // 中间交换机上的状态
    uint64_t _flowkey = 0;  // 流id
    uint32_t m_prePathCongestionRank;          // 原来的路径拥塞程度
    uint32_t m_pathCongestionRank;          // 现在的路径拥塞程度
    Time _activeTime;   // 流活动时间
};

// follow PISA metadata concept
// 数据包需要知道的消息
struct gemmaTxMeta {
    uint64_t pkt_flowkey = 0;
    bool newConnection = false;
    bool flagExpired = false;
    bool flagReplyTimeout = false;
    bool flagStabilized = false;
    uint32_t epoch = 0;
    uint32_t phase = 0;
    uint32_t stage = 0;
    uint32_t goodPath = CW_DEFAULT_32BIT;
    bool foundGoodPath = false;
    uint32_t currPath = CW_DEFAULT_32BIT;
    uint32_t prePath = 0;
    uint64_t tailTime = 0;

    /*-- REPLY Metadata --*/
    uint32_t reply_flag;
    uint32_t reply_epoch;
    uint32_t reply_phase;
};

// follow PISA metadata concept
struct gemmaRxMeta {
    uint64_t pkt_flowkey = 0;
    uint32_t pkt_pathId = 0;
    uint32_t pkt_epoch = 0;
    uint32_t pkt_phase = 0;
    uint32_t pkt_stage = 0;
    uint64_t pkt_timestamp_Tx = 0;
    uint64_t pkt_timestamp_TAIL = 0;
    uint32_t pkt_flagData = 0;
    uint8_t pkt_ecnbits = 0; /* ECN bits */
    bool newConnection = false;
    uint8_t resultEpochMatch = 0; /* 1: new epoch, 2: prev epoch, 0: curr epoch */
    Time phase0TxTime;
    Time phase0RxTime;
    bool flagOutOfOrder = false;
    bool flagPhase0Cache = false; /* to check whether RTT info is available or not */
    uint64_t tailTime = 0;
    uint64_t timegapAtTx = 0;
    uint64_t timeExpectedToFlush = 0;
    bool flagEnqueue = false;
};

/*----------------------------*/
// tag for data and request
class GemmaDataTag : public Tag {
   public:
    GemmaDataTag();
    void SetPathId(uint32_t pathId);
    uint32_t GetPathId(void) const;
    void SetHopCount(uint32_t hopCount);
    uint32_t GetHopCount(void) const;
    void SetPhase(uint32_t phase);
    uint32_t GetPhase(void) const;
    void SetTimestampTx(uint64_t timestamp);
    uint64_t GetTimestampTx(void) const;
    void SetFlagData(uint32_t flag);
    uint32_t GetFlagData(void) const;

    void SetPrePathCongestionRank(uint32_t congestRank);
    uint32_t GetPrePathCongestionRank(void) const;
    void SetPathCongestionRank(uint32_t congestRank);
    uint32_t GetPathCongestionRank(void) const;
    void SetWaitedLoop(uint32_t waitedLoop);
    uint32_t GetWaitedLoop(void) const;
    void SetIsFirst(uint32_t isFirst);
    uint32_t GetIsFirst(void) const;
    void SetSwitchPortKey(uint64_t switchPortKey);
    uint64_t GetSwitchPortKey(void) const;
    void SetStage(uint32_t stage);
    uint32_t GetStage(void) const;
    void SetFlagNotify(uint32_t flag);
    uint32_t GetFlagNotify(void) const;

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    virtual uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, GemmaDataTag const& tag) {
        return os << "m_pathId:" << tag.m_pathId << "\n"
                  << "m_hopCount:" << tag.m_hopCount << "\n"
                  << "m_phase:" << tag.m_phase << "\n"
                  << "m_timestampTx:" << tag.m_timestampTx << "\n"
                  << "m_flagData:" << tag.m_flagData << "\n"
                  << std::endl;
    }

    enum GemmaDataTagFlag {
        NONE = 0, /* Default, unused */
        DATA = 1,
        TAIL = 2,
    };

   private:
    uint32_t m_pathId;
    uint32_t m_hopCount;
    uint32_t m_phase;
    uint64_t m_timestampTx;                       // departure time at TxToR
    uint32_t m_flagData = GemmaDataTagFlag::NONE;  // control flag
    uint32_t m_prePathCongestionRank;          // 原来的路径拥塞程度
    uint32_t m_pathCongestionRank;          // 现在的路径拥塞程度
    uint32_t m_waitedLoop;                  // 已经等待的内循环次数
    uint32_t m_isFirst;                     // 是否重路由的第一个数据包
    uint64_t m_switchPortKey;               // 发送的交换机id和端口号
    uint32_t m_stage;   // 第二层的数据包阶段
    uint32_t m_flagNotify = GemmaDataTagFlag::NONE;  // control flag
};

// tag for congestion NOTIFY packet
class GemmaNotifyTag : public Tag {
   public:
    GemmaNotifyTag();
    void SetPathId(uint32_t pathId);
    uint32_t GetPathId(void) const;
    void SetSwitchPortKey(uint64_t switchPortKey);
    void SetSwitchPortKeyB(uint64_t switchPortKeyB);
    void SetSwitchPortKeyC(uint64_t switchPortKeyC);
    uint64_t GetSwitchPortKey(void) const;
    uint64_t GetSwitchPortKeyB(void) const;
    uint64_t GetSwitchPortKeyC(void) const;
    void SetReceivePacketNum(uint32_t receivePacketNum);
    uint32_t GetReceivePacketNum(void) const;
    void SetPathScore(uint32_t pathScore);
    void SetPathScoreB(uint32_t pathScoreB);
    void SetPathScoreC(uint32_t pathScoreC);
    uint32_t GetPathScore(void) const;
    uint32_t GetPathScoreB(void) const;
    uint32_t GetPathScoreC(void) const;
    

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, GemmaNotifyTag const& tag) {
        return os << "m_pathId:" << tag.m_pathId << std::endl;
    }

   private:
    uint32_t m_pathId;  // path of DATA
    uint64_t m_switchPortKey;
    uint64_t m_switchPortKeyB;
    uint64_t m_switchPortKeyC;
    uint32_t m_receivePacketNum;
    uint32_t m_pathScore;
    uint32_t m_pathScoreB;
    uint32_t m_pathScoreC;
};


// tag for reply
class GemmaReplyTag : public Tag {
   public:
    GemmaReplyTag();
    void SetFlagReply(uint32_t flagReply);
    uint32_t GetFlagReply(void) const;
    void SetPathId(uint32_t m_pathId);
    uint32_t GetPathId(void) const;
    void SetPhase(uint32_t phase);
    uint32_t GetPhase(void) const;
    void SetRank(uint32_t rank);
    uint32_t GetRank(void) const;
    void SetVersion(uint32_t version);
    uint32_t GetVersion(void) const;

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, GemmaReplyTag const& tag) {
        return os << "m_flagReply:" << tag.m_flagReply << "\n"
                  << "m_phase:" << tag.m_phase << "\n"
                  << std::endl;
    }

    enum GemmaReplyTagFlag {
        NONE = 0, /* Default, unused */
        DATA = 1, /* 拥塞程度发生变化时的响应 */
        TAIL = 2, /* CLEAR in paper */
    };

   private:
    uint32_t m_flagReply;
    uint32_t m_phase;
    uint32_t m_pathId;
    uint32_t m_rank;
    uint32_t m_version;
};

/**
 * @brief ConWeave object is created for each ToR Switch
 * -- path <-> outPort transformation --
 * uint8_t pathp[4] = {uint8_t[port0], uint8_t[port1], uint8_t[port2], uint8_t[port3]}
 * uint32_t path = *((uint32_t*) pathp);
 * (uint8_t*)&path[0] -> port0
 */

class GemmaRouting : public Object {
    friend class SwitchMmu;
    friend class SwitchNode;

   public:
    GemmaRouting();
    ~GemmaRouting();
    virtual void DoDispose();

    /* static */
    static TypeId GetTypeId(void);
    static uint32_t GetOutPortFromPath(
        const uint32_t& path,
        const uint32_t& hopCount);  // decode outPort from path, given a hop's order
    static void SetOutPortToPath(uint32_t& path, const uint32_t& hopCount,
                                 const uint32_t& outPort);  // encode outPort to path

    /* key */
    static uint64_t GetFlowKey(uint32_t ip1, uint32_t ip2, uint16_t port1,
                               uint16_t port2);                             // hashkey (4-tuple)
    static uint64_t GetFlowPhaseKey(uint64_t flowKey, uint32_t phase);
    uint32_t GetPathFlightNum(uint32_t srcId, uint16_t portId);
    uint32_t GetPathScore(uint32_t srcId, uint32_t pathId);
    uint32_t NewGetPathScore(uint32_t srcId, uint32_t pathId);
    uint32_t GetLocalPathScore(uint32_t srcId, uint32_t pathId);
    uint32_t CalResponseNum(uint64_t time, uint64_t nowTime);
    static uint64_t GetSwitchPortKey(uint32_t switchNum, uint16_t port);
    static uint32_t DoHash(const uint8_t* key, size_t len, uint32_t seed);  // hash function
    uint32_t GetMgemmaTxTable() {return (uint32_t)m_gemmaTxTable.size();}

    uint32_t GetNumVOQ() { return (uint32_t)m_voqMap.size(); }
    uint32_t GetVolumeVOQ();
    const std::unordered_map<uint64_t, ConWeaveVOQ>& GetVOQMap() { return m_voqMap; }

    /* main function */
    void SendReply(Ptr<Packet> p, CustomHeader& ch, uint32_t flagReply, uint32_t pkt_epoch, uint32_t pathRank, uint32_t pathVersion);
    void SendNotify(Ptr<Packet> p, CustomHeader &ch, uint32_t pathId, uint64_t switchPortKey, uint32_t receivePacketNum,
         uint32_t pathScore);
    void RouteInput(Ptr<Packet> p, CustomHeader& ch);  // core function

    void DeleteVOQ(uint64_t flowPhaseKey);  // used for callback when reorder queue is flushed
    EventId m_agingEvent;
    EventId m_recirculatingEvent;
    void AgingEvent();  // aging Tx/RxTableEntry (for cleaning and improve NS-3 simulation)
    void RecirculatingEvent();  // 交换机内循环
    /* SET functions */
    void SetConstants(Time extraReplyDeadline, Time extraVOQFlushTime, Time txExpiryTime,
                      Time defaultVOQWaitingTime, Time pathPauseTime, bool pathAwareRerouting);
    void SetSwitchInfo(bool isToR, bool isAgg, uint32_t switch_id);

    // callback of SwitchSend
    void DoSwitchSend(Ptr<Packet> p, CustomHeader& ch, uint32_t outDev,
                      uint32_t qIndex);                       // TxToR and Agg/CoreSw
    void DoSwitchSendToDev(Ptr<Packet> p, CustomHeader& ch);  // only at RxToR

    void CallbackByVOQFlush(uint64_t flowPhaseKey, uint32_t voqSize);  // used for callback in VOQ

    typedef Callback<void, Ptr<Packet>, CustomHeader&, uint32_t, uint32_t> SwitchSendCallback;
    typedef Callback<void, Ptr<Packet>, CustomHeader&> SwitchSendToDevCallback;
    void SetSwitchSendCallback(SwitchSendCallback switchSendCallback);  // set callback
    void SetSwitchSendToDevCallback(
        SwitchSendToDevCallback switchSendToDevCallback);  // set callback

    /* topological info (should be initialized in the beginning) */
    std::map<uint32_t, std::set<uint32_t> >
        m_GemmaRoutingTable;                      // <RxToRId -> set<pathId> > just for reference
    std::map<uint32_t, uint32_t> m_ipTable;

    /* statistics (logging) */
    static uint32_t m_RankChangeNum;    // 路径梯度变化的次数
    static uint32_t m_ReviceReplyNum;   // 收到的响应个数
    static uint32_t m_PortPausedNum;    // 端口被暂停的次数
    static uint32_t m_RerouteNum;       // 重新路由的次数
    static uint32_t m_packetNum;        // 数据包个数
    static uint32_t m_maxRank;          // 最大梯度
    static std::set<uint32_t> queueIdSet;       // 队列的id集合
    static uint32_t m_outOfOrderPkts;  // 乱序数据包数量
    static uint32_t m_nNotifySent;     // 通知数据包数量
    static uint32_t m_unusedRerouted;  // 无效重新路由
    static std::map<uint64_t, uint32_t> m_RankChangeMap;   // 拥塞程度改变的情况

   private:
    // callback
    SwitchSendCallback m_switchSendCallback;  // bound to SwitchNode::SwitchSend (for Request/UDP)
    SwitchSendToDevCallback m_switchSendToDevCallback;  // bound to SwitchNode::SendToDevContinue (for Probe, Reply)

    // topology parameters
    bool m_isToR;          // is ToR (leaf)
    bool m_isAgg;
    uint32_t m_switch_id;  // switch's nodeID

    // gemma parameters
    uint32_t m_congestionRankGap;   // 路径的拥塞梯度
    uint32_t m_reroutedRank;        // 重新路由拥塞程度梯度
    bool paused[512][64];

    Time m_agingTime;  // aging time (e.g., 2ms)
    Time m_defaultVOQWaitingTime;
    Time m_extraVOQFlushTime;   // extra for uncertainty
    Time m_entryPauseTime;

    Time m_recirculatingTime;  // recirculate time (e.g., 500ns)
    // local
    std::map<uint64_t, gemmaTxState> m_gemmaTxTable;  // flowkey -> TxToR's stateful table
    std::map<uint64_t, gemmaRxState> m_gemmaRxTable;  // flowkey -> RxToR's stateful table
    std::map<uint64_t, gemmaCoreState> m_gemmaCoreTable;  // flowkey -> RxToR's stateful table
    std::queue<Ptr<Packet> > recirculationQueue;        // 内循环队列
    std::map<uint32_t, uint32_t> m_txPathCongestion;  // 发送方路径的拥塞程度
    std::map<uint32_t, uint32_t> m_txPathCongestionVersion;  // 发送方路径的拥塞程度版本号
    std::map<uint32_t, uint32_t> m_rxPathCongestion;  // 接收方路径的拥塞程度
    std::map<uint32_t, uint32_t> m_rxPathCongestionVersion;  // 接收方路径的拥塞程度版本号
    std::unordered_map<uint64_t, ConWeaveVOQ> m_voqMap;  // 暂存队列
    std::unordered_map<uint64_t, bool> m_isFlush;  // 记录每个阶段的Tail数据包是否已经到达
    std::unordered_map<uint64_t, Time> m_pathInvaildTime;   // 路径的失效时间 
    std::map<uint64_t, uint32_t> m_sendPakcetNum;   // 发送的数据包数量
    std::map<uint64_t, uint32_t> m_receivePacketNum;  // 接受的数量
    std::map<uint64_t, uint32_t> m_downPathScore; // 下行链路的分数
    static uint64_t debug_time;
    uint64_t receiveNum;
    uint64_t lastTime;
    uint32_t responseNum = 5;   // 响应数量
    
};

}  // namespace ns3

#endif
