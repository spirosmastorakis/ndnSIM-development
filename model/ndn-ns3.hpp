/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2014  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.hpp"

#include <ndn-cxx/encoding/block.hpp>

namespace ndn {

class Block;

class UnknownHeaderException {};

class Convert
{
public:
  static shared_ptr<Block>
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  static void
  ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);
};

}

#endif
