#include "ndn-data.h"

namespace ns3 {

namespace ndn {

using ns3::Ptr;
using ns3::Packet;

Data::Data () :
  ::ndn::Data ()
{
}

Data::Data (::ndn::Block block) :
  ::ndn::Data (block)
{
}

Ptr<Packet>
Data::getPacket () const
{
  return m_packet;
}

void
Data::setPacket (Ptr<Packet> packet)
{
  m_packet = packet;
}

} // namespace ndn

} // namespace ns3
