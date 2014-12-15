#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"
#include "ns3/ndn-fw-hop-count-tag.h"

#include <ndn-cxx/common.hpp>

namespace ndn {

class Block;

class UnknownHeaderException {};

class Convert
{
public:
  static shared_ptr<Block>
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  static void
  ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);
};

}

#endif
