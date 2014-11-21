#include "ndn-trailer.hpp"

namespace ndn {

NdnTrailer::NdnTrailer() :
  m_trailerLength (0),
  m_trailerBuffer (NULL)
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
