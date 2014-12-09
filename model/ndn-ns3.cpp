#include "ndn-ns3.hpp"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

namespace ndn {

Block&
Convert::FromPacket(ns3::Ptr<ns3::Packet> packet, uint32_t& hopTag)
{
  Buffer buffer(packet->GetSize());
  packet->CopyData(buffer.buf(), packet->GetSize());
  Block *block = new Block(buffer.buf(), packet->GetSize());

  ns3::ndn::FwHopCountTag hopCount;
  bool tagExists = packet->RemovePacketTag (hopCount);
  if (tagExists)
    hopTag = hopCount.Get ();
  return *block;
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
