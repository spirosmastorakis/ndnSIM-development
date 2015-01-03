/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2014  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef NDN_L3_PROTOCOL_H
#define NDN_L3_PROTOCOL_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"

#include <list>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/traced-callback.h"

#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/internal-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/face-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/status-server.hpp"

namespace ns3 {

class Packet;
class Node;
class Header;

namespace ndn {

using nfd::Forwarder;
using nfd::InternalFace;
using nfd::FaceManager;
using nfd::FibManager;
using nfd::StrategyChoiceManager;
using nfd::StatusServer;

/**
 * \defgroup ndn ndnSIM: NDN simulation module
 *
 * This is a modular implementation of NDN protocol for NS-3
 */
/**
 * \ingroup ndn
 * \brief Implementation network-layer of NDN stack
 *
 * This class defines the API to manipulate the following aspects of
 * the NDN stack implementation:
 * -# register a face (Face-derived object) for use by the NDN
 *    layer
 *
 * Each Face-derived object has conceptually a single NDN
 * interface associated with it.
 *
 * In addition, this class defines NDN packet coding constants
 *
 * \see Face, ForwardingStrategy
 */
class L3Protocol : public Object {
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId
  GetTypeId();

  static const uint16_t ETHERNET_FRAME_TYPE; ///< @brief Ethernet Frame Type of Ndn
  static const uint16_t IP_STACK_PORT;       ///< @brief TCP/UDP port for NDN stack
  // static const uint16_t IP_PROTOCOL_TYPE;    ///< \brief IP protocol type of NDN

  /**
   * \brief Default constructor. Creates an empty stack without forwarding strategy set
   */
  L3Protocol();
  virtual ~L3Protocol();

  /**
   * \brief Initialize NFD instance
   *
   */
  void
  initialize(bool shouldUseNfdCs);

  /**
   * \brief Initialized NFD management
   *
   */
  void
  initializeManagement(bool shouldUseNfdCs);

  shared_ptr<FibManager>
  GetFibManager();

  void
  SetFibManager(shared_ptr<FibManager> fibManager);

  shared_ptr<StrategyChoiceManager>
  GetStrategyChoiceManager();

  void
  SetStrategyChoiceManager(shared_ptr<StrategyChoiceManager> strategyChoiceManager);

  /**
   * \brief Get the forwarder (NFD) instance
   *
   * \returns pointer to the NFD instance
   */
  shared_ptr<Forwarder>
  GetForwarder();

  void
  SetForwarder(shared_ptr<Forwarder> forwarder);

  /**
   * \brief Add face to Ndn stack
   *
   * \param face smart pointer to NdnFace-derived object
   * (NdnLocalFace, NdnNetDeviceFace, NdnUdpFace) \returns the
   * index of the Ndn interface added.
   *
   * \see NdnLocalFace, NdnNetDeviceFace, NdnUdpFace
   */
  virtual nfd::FaceId
  AddFace(shared_ptr<Face> face);

  // /**
  //  * \brief Get current number of faces added to Ndn stack
  //  *
  //  * \returns the number of faces
  //  */
  // virtual uint32_t
  // GetNFaces() const;

  // /**
  //  * \brief Get face by face index
  //  * \param face The face number (number in face list)
  //  * \returns The NdnFace associated with the Ndn face number.
  //  */
  // virtual shared_ptr<Face>
  // GetFace(uint32_t face) const;

  /**
   * \brief Get face by face ID
   * \param face The face ID number
   * \returns The NdnFace associated with the Ndn face number.
   */
  virtual shared_ptr<Face>
  GetFaceById(nfd::FaceId face) const;

  // /**
  //  * \brief Remove face from ndn stack (remove callbacks)
  //  */
  // virtual void
  // RemoveFace(shared_ptr<Face> face);

  /**
   * \brief Get face for NetDevice
   */
  virtual shared_ptr<Face>
  GetFaceByNetDevice(Ptr<NetDevice> netDevice) const;

  shared_ptr<Forwarder>
  getForwarder()
  {
    return m_forwarder;
  }

public: // Workaround for python bindings
  static Ptr<L3Protocol>
  getL3Protocol(Ptr<Object> node);

protected:
  virtual void
  DoDispose(void); ///< @brief Do cleanup

  /**
   * This function will notify other components connected to the node that a new stack member is now
   * connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them
   * together.
   */
  virtual void
  NotifyNewAggregate();

private:
  L3Protocol(const L3Protocol&); ///< copy constructor is disabled

  L3Protocol&
  operator=(const L3Protocol&); ///< copy operator is disabled

private:
  shared_ptr<Forwarder> m_forwarder;

  shared_ptr<InternalFace> m_internalFace;
  shared_ptr<FibManager> m_fibManager;
  shared_ptr<FaceManager> m_faceManager;
  shared_ptr<StrategyChoiceManager> m_strategyChoiceManager;
  shared_ptr<StatusServer> m_statusServer;

  Ptr<ContentStore> m_csFromNdnSim;

  // These objects are aggregated, but for optimization, get them here
  Ptr<Node> m_node; ///< \brief node on which ndn stack is installed

  TracedCallback<const Interest&, const Face&>
    m_inInterests; ///< @brief trace of incoming Interests
  TracedCallback<const Interest&, const Face&>
    m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<const Data&, const Face&> m_outData; ///< @brief trace of outgoing Data
  TracedCallback<const Data&, const Face&> m_inData;  ///< @brief trace of incoming Data
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_L3_PROTOCOL_H */
