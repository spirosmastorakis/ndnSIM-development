#ifndef NDN_HEADER_HPP
#define NDN_HEADER_HPP

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include "ns3/packet.h"

namespace ndn {

/**
 * @ingroup ndn-ns3
 * @brief This class describes the basic functionality
 * of the NDN header for ndnSIM
 */
class NdnHeader: public ns3::Header
{
public:

  /**
   * \brief Default constructor
   */
  NdnHeader();

  /**
   * \brief Constructor
   *
   * @param headerBuffer The buffer that includes the header
   * @param headerLength The length of the header
   */
  NdnHeader(uint8_t* headerBuffer, size_t headerLength);

  /**
   * \brief Destructor
   */
  ~NdnHeader();

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ns3::Header
  virtual ns3::TypeId GetInstanceTypeId(void) const;

  virtual uint32_t GetSerializedSize(void) const;

  virtual void Serialize(ns3::Buffer::Iterator start) const;

  virtual uint32_t Deserialize(ns3::Buffer::Iterator start);

  virtual void Print(std::ostream &os) const;

  /**
   * \brief Add the NDN header to the NS-3 packet
   *
   * @param packet The NS-3 packet to which the header would be added
   * @param ndnHeader The NDN header to be added
   */
  void AddNdnHeader(ns3::Ptr<ns3::Packet> packet , NdnHeader ndnHeader);

  /**
   * \brief Remove the NDN header from a NS-3 packet
   *
   * @param packet The NS-3 packet including the header
   * @param ndnHeader The NDN header to be removed
   */
  void RemoveNdnHeader(ns3::Ptr<ns3::Packet> packet , NdnHeader ndnHeader);

  /**
   * \brief Get the buffer including the header
   *
   * \returns pointer to the header buffer
   */
  const uint8_t* GetBuffer();

  /**
   * \brief Set the buffer including the header
   *
   * @param headerBuffer The buffer including the header
   */
  void SetBuffer(uint8_t* headerBuffer);

  /**
   * \brief Get the header length
   *
   * \returns the length of the header
   */
  size_t GetLength();

  /**
   * \brief Set the length of the header
   *
   * @param headerLength The length of the header
   */
  void SetLength(size_t headerLength);

private:
  ns3::Buffer m_buffer;
  size_t m_headerLength;
  uint8_t* m_headerBuffer;
};

}

#endif
