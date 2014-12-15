#ifndef NDN_INTEREST_H
#define NDN_INTEREST_H

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <ndn-cxx/interest.hpp>

namespace ns3 {

namespace ndn {

using ns3::Ptr;
using ns3::Packet;

/**
 * @ingroup ndn
 * @defgroup ndn-interest Interest
 * @brief This class inherits the ndn::Interest
 * class and was created to only compute the
 * hop count
 */
class Interest : public ::ndn::Interest
{
public:

  /**
   * \brief Get the packet associated with this
   * interest structure
   *
   * \returns shared pointer to the packet
   */
  Ptr<Packet>
  getPacket () const;

  /**
   * \brief Set the packet associated with this
   * interest structure
   *
   * @param packet The packet that will be associated with the
   * interest structure
   */
  void
  setPacket (Ptr<Packet> packet);

private:
  Ptr<Packet> m_packet = Create<Packet> ();
};

} // namespace ndn

} // namespace ns3

#endif
