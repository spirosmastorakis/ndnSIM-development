#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include <memory>

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

namespace ndn {

class Block;

class UnknownHeaderException {};

class Convert
{
public:

  static void
  ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

  static Block&
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  static void
  InterestToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

  static void
  DataToPacket(std::shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

};

}

#endif
