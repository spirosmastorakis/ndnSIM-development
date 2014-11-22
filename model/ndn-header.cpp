#include "ndn-header.hpp"

namespace ndn {

NdnHeader::NdnHeader() :
  m_headerLength (0),
  m_headerBuffer (nullptr)
{
}

NdnHeader::NdnHeader(uint8_t* headerBuffer, size_t headerLength)
{
  m_headerLength = headerLength;
  m_headerBuffer = headerBuffer;
}

NdnHeader::~NdnHeader()
{
}

ns3::TypeId
NdnHeader::GetInstanceTypeId(void) const
{
  static ns3::TypeId tid = ns3::TypeId ("ndn::NdnHeader")
  .SetParent<ns3::Chunk> ()
  ;
  return tid;
}

uint32_t
NdnHeader::GetSerializedSize(void) const
{
  return uint32_t(m_headerLength);
}

void
NdnHeader::Serialize(ns3::Buffer::Iterator start) const
{
  ns3::Buffer::Iterator i = start;
  i.Write(m_headerBuffer,  uint32_t(m_headerLength));
}

uint32_t
NdnHeader::Deserialize(ns3::Buffer::Iterator start)
{
  ns3::Buffer::Iterator i = start;
  i.Read(m_headerBuffer, m_headerLength);
  return uint32_t(m_headerLength);
}

void
NdnHeader::Print(std::ostream &os) const
{
  os << m_headerBuffer;
}

void
NdnHeader::AddNdnHeader(ns3::Ptr<ns3::Packet> packet, NdnHeader ndnHeader)
{
  packet->AddHeader(ndnHeader);
}

void
NdnHeader::RemoveNdnHeader(ns3::Ptr<ns3::Packet> packet, NdnHeader ndnHeader)
{
  packet->RemoveHeader(ndnHeader);
}

void
NdnHeader::SetBuffer(uint8_t* headerBuffer)
{
  m_headerBuffer = headerBuffer;
}

const uint8_t*
NdnHeader::GetBuffer()
{
  return m_headerBuffer;
}

void
NdnHeader::SetLength(size_t headerLength)
{
  m_headerLength = headerLength;
}

}
