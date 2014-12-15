#ifndef NDN_TRAILER_HPP
#define NDN_TRAILER_HPP

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include "ns3/packet.h"

namespace ndn {

/**
 * @ingroup ndn-ns3
 * @brief This class describes the basic functionality
 * of the NDN trailer for ndnSIM
 */
class NdnTrailer: public ns3::Trailer
{
public:

  /**
   * \brief Default constructor
   */
  NdnTrailer();

  /**
   * \brief Constructor
   *
   * @param trailerBuffer The buffer that includes the trailer
   * @param trailerLength The length of the trailer
   */
  NdnTrailer(uint8_t* trailerBuffer, size_t trailerLength);

  /**
   * \brief Destructor
   */
  ~NdnTrailer();

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ns3::Trailer
  virtual ns3::TypeId GetInstanceTypeId(void) const;

  virtual uint32_t GetSerializedSize(void) const;

  virtual void Serialize(ns3::Buffer::Iterator start) const;

  virtual uint32_t Deserialize(ns3::Buffer::Iterator start);

  virtual void Print(std::ostream &os) const;

  /**
   * \brief Add the NDN trailer to the NS-3 packet
   *
   * @param packet The NS-3 packet to which the trailer would be added
   * @param ndnTrailer The NDN trailer to be added
   */
  void AddNdnTrailer(ns3::Ptr<ns3::Packet> packet, NdnTrailer ndnTrailer);

  /**
   * \brief Remove the NDN trailer from a NS-3 packet
   *
   * @param packet The NS-3 packet including the trailer
   * @param ndnTrailer The NDN trailer to be removed
   */
  void RemoveNdnTrailer(ns3::Ptr<ns3::Packet> packet, NdnTrailer ndnTrailer);

  /**
   * \brief Set the buffer including the trailer
   *
   * @param trailerBuffer The buffer including the trailer
   */
  void SetBuffer(uint8_t* trailerBuffer);

  /**
   * \brief Set the length of the trailer
   *
   * @param trailerLength The length of the trailer
   */
  void SetLength(size_t trailerLength);

private:
  ns3::Buffer m_buffer;
  size_t m_trailerLength;
  uint8_t* m_trailerBuffer;
};

}

#endif
