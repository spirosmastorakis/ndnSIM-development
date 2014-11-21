#ifndef NDN_HEADER_HPP
#define NDN_HEADER_HPP

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include "ns3/packet.h"

namespace ndn {

class NdnHeader: public ns3::Header
{
public:
  NdnHeader();

  NdnHeader(uint8_t* headerBuffer, size_t headerLength);

  ~NdnHeader();

  virtual ns3::TypeId GetInstanceTypeId(void) const;

  virtual uint32_t GetSerializedSize(void) const;

  virtual void Serialize(ns3::Buffer::Iterator start) const;

  virtual uint32_t Deserialize(ns3::Buffer::Iterator start);

  virtual void Print(std::ostream &os) const;

  void AddNdnHeader(ns3::Ptr<ns3::Packet> packet , NdnHeader ndnHeader);

  void RemoveNdnHeader(ns3::Ptr<ns3::Packet> packet , NdnHeader ndnHeader);

  const uint8_t* GetBuffer();

  void SetBuffer(uint8_t* headerBuffer);

  void SetLength(size_t headerLength);

private:
  ns3::Buffer m_buffer;
  size_t m_headerLength;
  uint8_t* m_headerBuffer;
};

}

#endif
