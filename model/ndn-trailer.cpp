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

#include "ndn-trailer.hpp"

namespace ndn {

NdnTrailer::NdnTrailer() :
  m_trailerLength (0),
  m_trailerBuffer (nullptr)
{
}

NdnTrailer::NdnTrailer(uint8_t* trailerBuffer, size_t trailerLength)
{
  m_trailerLength = trailerLength;
  m_trailerBuffer = trailerBuffer;
}

NdnTrailer::~NdnTrailer()
{
}

ns3::TypeId
NdnTrailer::GetInstanceTypeId(void) const
{
  static ns3::TypeId tid = ns3::TypeId ("ndn::NdnTrailer")
  .SetParent<ns3::Chunk> ()
  ;
  return tid;
}

uint32_t
NdnTrailer::GetSerializedSize(void) const
{
  return uint32_t(m_trailerLength);
}

void
NdnTrailer::Serialize(ns3::Buffer::Iterator start) const
{
  ns3::Buffer::Iterator i = start;
  i.Write(m_trailerBuffer,  uint32_t(m_trailerLength));
}

uint32_t
NdnTrailer::Deserialize(ns3::Buffer::Iterator start)
{
  ns3::Buffer::Iterator i = start;
  i.Read(m_trailerBuffer, m_trailerLength);
  return uint32_t(m_trailerLength);
}

void
NdnTrailer::Print(std::ostream &os) const
{
  os << m_trailerBuffer;
}

void
NdnTrailer::AddNdnTrailer(ns3::Ptr<ns3::Packet> packet, NdnTrailer ndnTrailer)
{
  packet->AddTrailer(ndnTrailer);
}

void
NdnTrailer::RemoveNdnTrailer(ns3::Ptr<ns3::Packet> packet, NdnTrailer ndnTrailer)
{
  packet->RemoveTrailer(ndnTrailer);
}

void
NdnTrailer::SetBuffer(uint8_t* trailerBuffer)
{
  m_trailerBuffer = trailerBuffer;
}

void
NdnTrailer::SetLength(size_t trailerLength)
{
  m_trailerLength = trailerLength;
}

}
