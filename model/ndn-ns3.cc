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

#include "ndn-ns3.h"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

namespace ndn {

Convert::Convert()
{
}

shared_ptr<Block>
Convert::FromPacket(ns3::Ptr<ns3::Packet> packet)
{
  Buffer buffer(packet->GetSize());
  packet->CopyData(buffer.buf(), packet->GetSize());
  shared_ptr<Block> block = make_shared<Block>(Block(buffer.buf(), packet->GetSize()));
  return block;
}

void
Convert::ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet)
{
  size_t   headerLength;
  uint8_t *headerBuffer;

  block->parse();
  headerLength = block->size();
  headerBuffer = const_cast<uint8_t*>(block->wire());
  NdnHeader ndnHeader(headerBuffer, headerLength);
  ndnHeader.AddNdnHeader(packet, ndnHeader);
}

}
