#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
<<<<<<< HEAD

#include "ndn-header.hpp"
#include "ndn-trailer.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>
=======
#include "ns3/ptr.h"

#include <ndn-cxx/common.hpp>
>>>>>>> 46366be3d265891e64d92f874c23b11f572a8db7

namespace ndn {

class Block;

class UnknownHeaderException {};

<<<<<<< HEAD
class Convert
{

public:

=======
class Convert {

public:
>>>>>>> 46366be3d265891e64d92f874c23b11f572a8db7
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
