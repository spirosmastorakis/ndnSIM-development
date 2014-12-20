#ifndef NDN_INTEREST_H
#define NDN_INTEREST_H

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <ndn-cxx/interest.hpp>

namespace ns3 {

namespace ndn {

using ns3::Ptr;
using ns3::Packet;

class Interest : public ::ndn::Interest
{
public:
  Ptr<Packet>
  getPacket () const;

  void
  setPacket (Ptr<Packet> packet);

private:
  Ptr<Packet> m_packet = Create<Packet> ();
};

} // namespace ndn

} // namespace ns3

#endif
