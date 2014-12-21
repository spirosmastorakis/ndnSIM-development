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

#include "ndn-ns3.hpp"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

namespace ns3 {
namespace ndn {

::ndn::Block
Convert::FromPacket(Ptr<const Packet> packet)
{
  auto buffer = std::make_shared<::ndn::Buffer>(packet->GetSize());
  packet->CopyData(buffer->buf(), packet->GetSize());
  return ::ndn::Block(buffer);
}

void
Convert::ToPacket(const ::ndn::Block& block, Ptr<Packet> packet)
{
  size_t headerLength;
  uint8_t* headerBuffer;

  block.parse();
  headerLength = block.size();
  headerBuffer = const_cast<uint8_t*>(block.wire());
  ::ndn::NdnHeader ndnHeader(headerBuffer, headerLength);
  ndnHeader.AddNdnHeader(packet, ndnHeader);
}

} // namespace ndn
} // namespace ns3
