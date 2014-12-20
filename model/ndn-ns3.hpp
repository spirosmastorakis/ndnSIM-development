#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.hpp"

#include <ndn-cxx/encoding/block.hpp>

namespace ndn {

class Block;

class UnknownHeaderException {};

class Convert
{
public:
  static Block&
  FromPacket(ns3::Ptr<ns3::Packet> packet);

  static void
  ToPacket(shared_ptr<Block> block, ns3::Ptr<ns3::Packet> packet);
};

}

#endif
