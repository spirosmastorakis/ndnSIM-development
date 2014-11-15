#ifndef NDN_TRAILER_HPP
#define NDN_TRAILER_HPP

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include "ns3/packet.h"

namespace ndn {

class NdnTrailer: public ns3::Trailer {

public:
  NdnTrailer();

  NdnTrailer(uint8_t* trailBuffer, size_t trailerLength);

  ~NdnTrailer();

  virtual ns3::TypeId GetInstanceTypeId(void) const;

  virtual uint32_t GetSerializedSize(void) const;

  virtual void Serialize(ns3::Buffer::Iterator start) const;

  virtual uint32_t Deserialize(ns3::Buffer::Iterator start);

  virtual void Print(std::ostream &os) const;

  void AddNdnTrailer(ns3::Ptr<ns3::Packet> packet, NdnTrailer ndnTrailer);

  void RemoveNdnTrailer(ns3::Ptr<ns3::Packet> packet, NdnTrailer ndnTrailer);

  void SetBuffer(uint8_t* trailerBuffer);

  void SetLength(size_t trailerLength);

private:
  ns3::Buffer m_buffer;
  size_t m_trailerLength;
  uint8_t* m_trailerBuffer;
};

}

#endif
