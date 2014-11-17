#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include <memory>

namespace ndn {


class Block;

class UnknownHeaderException {};

class Convert
{

public:

  static void
  ToPacket(std::shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

  static Block&
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  static void
  InterestToPacket(std::shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

  static void
  DataToPacket(std::shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);

};

}

#endif
