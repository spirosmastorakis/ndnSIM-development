/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_L3_PROTOCOL_H
#define NDN_L3_PROTOCOL_H

#include <list>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"

#include "ns3/ndnSIM/NFD/core/privilege-helper.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/null-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/internal-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/face-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/status-server.hpp"
#include "ns3/ndnSIM/NFD/core/config-file.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/general-config-section.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/tables-config-section.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/pit.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/pit-entry.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/name-tree.hpp"

using nfd::Forwarder;
using nfd::InternalFace;
using nfd::FaceManager;
using nfd::FibManager;
using nfd::StrategyChoiceManager;
using nfd::StatusServer;

namespace ns3 {

class Packet;
class NetDevice;
class Node;
class Header;

namespace ndn {

class Face;
//class ForwardingStrategy;

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
class L3Protocol :
    public Object
{
public:
  typedef std::vector<Ptr<Face> > FaceList;

  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  static const uint16_t ETHERNET_FRAME_TYPE; ///< @brief Ethernet Frame Type of Ndn
  static const uint16_t IP_STACK_PORT;       ///< @brief TCP/UDP port for NDN stack
  // static const uint16_t IP_PROTOCOL_TYPE;    ///< \brief IP protocol type of NDN

  /**
   * \brief Default constructor. Creates an empty stack without forwarding strategy set
   */
  L3Protocol();
  virtual ~L3Protocol ();

  /**
   * \brief Initialize NFD instance
   *
   * @param node The node in which NFD would be initialized
   */
  void
  initialize(Ptr<Node> node);

  /**
   * \brief Initialized NFD management
   *
   */
  void
  initializeManagement();

  /**
   * \brief Get the Fib Manager instance
   *
   * \returns shared pointer to the Fib Manager instance
   */
  shared_ptr<FibManager>
  GetFibManager ();

  /**
   * \brief Set the Fib Manager instance
   *
   * @param fibManager The Fib Manager instance
   */
  void
  SetFibManager (shared_ptr<FibManager> fibManager);

  /**
   * \brief Get the strategy choice manager instance
   *
   * \returns shared pointer to the strategy choice manager instance
   */
  shared_ptr<StrategyChoiceManager>
  GetStrategyChoiceManager ();

  /**
   * \brief Set the strategy choice manager
   *
   * @param strategyChoiceManager The strategy choice manager instance
   */
  void
  SetStrategyChoiceManager (shared_ptr<StrategyChoiceManager> strategyChoiceManager);

  /**
   * \brief Choose whether NFD's CS or the original CS of ndnSIM will be used
   *
   * @param nfdCS True if the CS of NFD has been chosen, false if the original
   * CS of ndnSIM has been chosen
   */
  void
  SetContentStore (const bool nfdCS);

  /**
   * \brief Get the choice of the CS structure that has been used
   *
   * \returns true for the CS of NFD, false for the original CS of ndnSIM
   */
  bool
  GetContentStore ();

  /**
   * \brief Get the forwarder (NFD) instance
   *
   * \returns pointer to the NFD instance
   */
  shared_ptr<Forwarder>
  GetForwarder ();

  /**
   * \brief Set the forwarder (NFD) instance
   *
   * @param forwarder The forwarder instance
   */
  void
  SetForwarder (shared_ptr<Forwarder> forwarder);

  /**
   * \brief Add face to Ndn stack
   *
   * \param face smart pointer to NdnFace-derived object
   * (NdnLocalFace, NdnNetDeviceFace, NdnUdpFace) \returns the
   * index of the Ndn interface added.
   *
   * \see NdnLocalFace, NdnNetDeviceFace, NdnUdpFace
   */
  virtual uint32_t
  AddFace (const Ptr<Face> &face);

  /**
   * \brief Get current number of faces added to Ndn stack
   *
   * \returns the number of faces
   */
  virtual uint32_t
  GetNFaces () const;

  /**
   * \brief Get face by face index
   * \param face The face number (number in face list)
   * \returns The NdnFace associated with the Ndn face number.
   */
  virtual Ptr<Face>
  GetFace (uint32_t face) const;

  /**
   * \brief Get face by face ID
   * \param face The face ID number
   * \returns The NdnFace associated with the Ndn face number.
   */
  virtual Ptr<Face>
  GetFaceById (uint32_t face) const;

  /**
   * \brief Remove face from ndn stack (remove callbacks)
   */
  virtual void
  RemoveFace (Ptr<Face> face);

  /**
   * \brief Get face for NetDevice
   */
  virtual Ptr<Face>
  GetFaceByNetDevice (Ptr<NetDevice> netDevice) const;

protected:
  virtual void DoDispose (void); ///< @brief Do cleanup

  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();

private:
  L3Protocol(const L3Protocol &); ///< copy constructor is disabled
  L3Protocol &operator = (const L3Protocol &); ///< copy operator is disabled

private:
  uint32_t                          m_faceCounter; ///< \brief counter of faces. Increased every time a new face is added to the stack
  FaceList                          m_faces; ///< \brief list of faces that belongs to ndn stack on this node
  shared_ptr<Forwarder>             m_forwarder;

  shared_ptr<InternalFace>          m_internalFace;
  shared_ptr<FibManager>            m_fibManager;
  shared_ptr<FaceManager>           m_faceManager;
  shared_ptr<StrategyChoiceManager> m_strategyChoiceManager;
  shared_ptr<StatusServer>          m_statusServer;

  bool                              m_nfdCS = true;

  // These objects are aggregated, but for optimization, get them here
  Ptr<Node> m_node; ///< \brief node on which ndn stack is installed

  TracedCallback<const Interest&, const Face&> m_inInterests; ///< @brief trace of incoming Interests
  TracedCallback<const Interest&, const Face&> m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<const Data&, const Face&> m_outData; ///< @brief trace of outgoing Data
  TracedCallback<const Data&, const Face&> m_inData; ///< @brief trace of incoming Data
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_L3_PROTOCOL_H */
