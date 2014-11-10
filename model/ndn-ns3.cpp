#include "ndn-ns3.hpp"

namespace ndn {

Convert::Convert()
{
}

void
Convert::ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet)
{
  uint32_t m_type = block->type();
  if (m_type == 0x05)
    InterestToPacket(block, packet);
  else
    if (m_type == 0x06)
      DataToPacket(block, packet);
    else
      throw UnknownHeaderException();
}

Block&
Convert::FromPacket(ns3::Ptr<ns3::Packet> packet)
{
  Buffer buffer(packet->GetSize());
  packet->CopyData(buffer.buf(), packet->GetSize());
  Block *block = new Block(buffer.buf(), packet->GetSize());
  return *block;
}

void
Convert::InterestToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet)
{
  block->parse();
  m_headerLength = block->size();
  m_headerBuffer = const_cast<uint8_t*>(block->wire());
  NdnHeader::NdnHeader ndnHeader(m_headerBuffer, m_headerLength);
  ndnHeader.AddNdnHeader(packet, ndnHeader);
}

void
Convert::DataToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet)
{
  block->parse();

  m_headerLength = block->size();
  m_headerLength -= block->get(tlv::SignatureInfo).size();
  m_headerLength -= block->get(tlv::SignatureValue).size();
  m_headerLength -= block->get(tlv::Content).size();

  m_trailerLength = block->get(tlv::SignatureInfo).size() + block->get(tlv::SignatureValue).size();

  m_buffer = const_cast<uint8_t*>(block->wire());
  memcpy((void*)&m_headerBuffer, (void*)&m_buffer, m_headerLength);
  memcpy((void*)&m_trailerBuffer, (void*)&m_headerBuffer[int((block->get(tlv::Content).size())+m_headerLength)], m_trailerLength);

  NdnHeader::NdnHeader ndnHeader(m_headerBuffer, m_headerLength);
  NdnTrailer ndnTrailer(m_trailerBuffer, m_trailerLength);
  ndnHeader.AddNdnHeader(packet , ndnHeader);
  ndnTrailer.AddNdnTrailer(packet, ndnTrailer);
}

}
