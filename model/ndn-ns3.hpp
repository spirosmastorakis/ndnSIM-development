#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

namespace ndn {

class UnknownHeaderException {};

class Convert
{
public:
  Convert();

  void
  ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

  Block&
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  void
  InterestToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

  void
  DataToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

private:
  uint8_t* m_buffer;
  uint8_t* m_headerBuffer;
  uint8_t* m_trailerBuffer;
  size_t m_headerLength;
  size_t m_trailerLength;
};

}

#endif
