#ifndef NDN_DATA_H
#define NDN_DATA_H

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <ndn-cxx/data.hpp>

namespace ns3 {

namespace ndn {

using ns3::Ptr;
using ns3::Packet;

/**
 * @ingroup ndn
 * @defgroup ndn-data Data
 * @brief This class inherits the ndn::Data
 * class and was created to only compute the
 * hop count
 */
class Data : public ::ndn::Data
{
public:

  /**
   * \brief Default constructor
   */
  Data ();

  /**
   * \brief Constructor
   *
   * @param block The block that will be used for the
   * data creation
   */
  Data (::ndn::Block block);

  /**
   * \brief Get the packet associated with this data structure
   *
   * \returns shared pointer to the packet
   */
  Ptr<Packet>
  getPacket () const;

  /**
   * \brief Set the packet associated with this data structure
   *
   * @param packet The packet that will be associated with the
   * data structure
   */
  void
  setPacket (Ptr<Packet> packet);

private:
  Ptr<Packet> m_packet = Create<Packet> ();
};

} // namespace ndn

} // namespace ns3

#endif
