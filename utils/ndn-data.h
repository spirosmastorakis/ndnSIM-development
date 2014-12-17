/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2014 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * See AUTHORS file for the list of authors.
 */

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
