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
