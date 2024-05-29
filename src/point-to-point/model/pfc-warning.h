#ifndef __PFC_WARNING_H__
#define __PFC_WARNING_H__

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

// tag for congestion NOTIFY packet
class PfcNotify : public Tag {
   public:
    PfcNotify();
    

    static TypeId GetTypeId(void);
    virtual TypeId GetInstanceTypeId(void) const;
    uint32_t GetSerializedSize(void) const;
    virtual void Serialize(TagBuffer i) const;
    virtual void Deserialize(TagBuffer i);
    virtual void Print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, PfcNotify const& tag) {
        return os << "m_sourceId:" << tag.m_sourceId << std::endl;
    }

    uint32_t m_sourceId;
    uint32_t m_type;
};

class PfcWarning : public Object {
    friend class SwitchMmu;
    friend class SwitchNode;

   public:
    PfcWarning();
    ~PfcWarning();
    void SendPfcWarning(Ptr<Packet> p, uint32_t inDev, uint32_t type);
    void UpdateWarning(Ptr<Packet> p);
        // callback of SwitchSend
    void DoSwitchSend(Ptr<Packet> p, CustomHeader& ch, uint32_t outDev,
                      uint32_t qIndex);  
    void DoSwitchSendToDev(Ptr<Packet> p, CustomHeader& ch);
    /* static */
    static TypeId GetTypeId(void);
    typedef Callback<void, Ptr<Packet>, CustomHeader&, uint32_t, uint32_t> SwitchSendCallback;
    typedef Callback<void, Ptr<Packet>, CustomHeader&> SwitchSendToDevCallback;
    void SetSwitchSendCallback(SwitchSendCallback switchSendCallback);  // set callback
    void SetSwitchSendToDevCallback(
        SwitchSendToDevCallback switchSendToDevCallback);
    uint64_t GetSwitchPortKey(uint32_t switchNum, uint16_t port);
    uint32_t GetOutPortFromPath(const uint32_t& path, const uint32_t& hopCount);             
    bool SwitchPaused(uint32_t switch_id, uint32_t pathId);

    static uint32_t m_pasuedNum;

    bool m_isToR;
    uint32_t m_switch_id; 
    std::map<uint32_t, bool> m_isWarning;                     
    std::map<uint32_t, uint64_t> m_warningTime;
    // callback
    SwitchSendCallback m_switchSendCallback;  // bound to SwitchNode::SwitchSend (for Request/UDP)
    SwitchSendToDevCallback m_switchSendToDevCallback;  // bound to SwitchNode::SendToDevContinue (for Probe, Reply)

};
}

#endif