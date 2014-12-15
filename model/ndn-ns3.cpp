#include "ndn-ns3.hpp"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

namespace ndn {

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
