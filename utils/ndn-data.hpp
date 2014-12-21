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

#ifndef NDN_DATA_H
#define NDN_DATA_H

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <ndn-cxx/data.hpp>

namespace ns3 {

namespace ndn {

using ns3::Ptr;
using ns3::Packet;

class Data : public ::ndn::Data
{
public:
  Data ();

  Data (::ndn::Block block);

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
