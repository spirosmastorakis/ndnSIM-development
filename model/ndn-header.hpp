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

#ifndef NDN_HEADER_HPP
#define NDN_HEADER_HPP

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include "ns3/packet.h"

namespace ndn {

class NdnHeader : public ns3::Header {
public:
  NdnHeader();

  NdnHeader(uint8_t* headerBuffer, size_t headerLength);

  ~NdnHeader();

  virtual ns3::TypeId
  GetInstanceTypeId(void) const;

  virtual uint32_t
  GetSerializedSize(void) const;

  virtual void
  Serialize(ns3::Buffer::Iterator start) const;

  virtual uint32_t
  Deserialize(ns3::Buffer::Iterator start);

  virtual void
  Print(std::ostream& os) const;

  void
  AddNdnHeader(ns3::Ptr<ns3::Packet> packet, NdnHeader ndnHeader);

  void
  RemoveNdnHeader(ns3::Ptr<ns3::Packet> packet, NdnHeader ndnHeader);

  const uint8_t*
  GetBuffer();

  void
  SetBuffer(uint8_t* headerBuffer);

  void
  SetLength(size_t headerLength);

private:
  ns3::Buffer m_buffer;
  size_t m_headerLength;
  uint8_t* m_headerBuffer;
};
}

#endif
