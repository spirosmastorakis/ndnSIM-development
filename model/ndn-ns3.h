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

#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"

#include "ndn-header.h"
#include "ndn-trailer.h"
#include "ns3/ndn-fw-hop-count-tag.h"

#include <ndn-cxx/common.hpp>

namespace ndn {

class Block;

class UnknownHeaderException {};

/**
 * @ingroup ndn
 * @defgroup ndn-ns3 Convert
 * @brief This class describes the basic conversions
 * from a ndn::Block to a ns3::Packet and vice versa
 */
class Convert
{
public:

  /**
   * \brief Default constructor
   */
  Convert();

  /**
   * \brief Converts a ns3::Packet to a ndn::Block
   *
   * @param packet The packet that needs to be converted
   * to a block
   *
   * \returns shared pointer to the created ndn::Block
   */
  static shared_ptr<Block>
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  /**
   * \brief Converts a ndn::Block to a ns3::Packet
   *
   * @param block The block that needs to be converted to
   * a packet
   * @param packet The packet that will be filled with the
   * block
   */
  static void
  ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);
};

}

#endif
