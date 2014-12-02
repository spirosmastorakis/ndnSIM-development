#ifndef NDN_DATA_H
#define NDN_DATA_H

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/name.hpp>

namespace ns3 {

namespace ndn {

using ns3::Ptr;
using ns3::Packet;
using ::ndn::Name;

class Data : public ::ndn::Data
{
public:
  Data (const Name& name);

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
