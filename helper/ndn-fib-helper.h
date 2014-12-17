/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2014 University of California, Los Angeles
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
 * See AUTHORS file for the list of authors.
 */

#ifndef NDN_FIB_HELPER_H
#define NDN_FIB_HELPER_H

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-l3-protocol.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-stack-helper.h"

namespace ndn {
namespace nfd {
class ControlParameters;
};
};

namespace ns3 {
namespace ndn {

using ::ndn::nfd::ControlParameters;

/**
 * @ingroup ndn-helpers
 * @brief This class describes the basic functionality
 * of the fib helper that interacts with the fib manager
 * of NFD
 */
class FibHelper
{
public:

  /**
   * \brief Default constructor
   */
  FibHelper ();

  /**
   * \brief Destructor
   */
  ~FibHelper ();

  /**
   * \brief Send command to the fib manager of NFD in order to add a next hop
   *
   * @param parameters The required control parameters
   * @param node The node that sends the command
   */
  static void
  AddNextHop(const ControlParameters& parameters, Ptr<Node> node);

  /**
   * \brief Send command to the fib manager of NFD in order to remove a next hop
   *
   * @param parameters The required control parameters
   * @param node The node that sends the command
   */
  static void
  RemoveNextHop(const ControlParameters& parameters, Ptr<Node> node);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param nodeName Node name
   * \param prefix Routing prefix
   * \param faceId Face index
   * \param metric Routing metric
   */
  static void
  AddRoute(const std::string& nodeName, const std::string &prefix, uint32_t faceId, int32_t metric);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param nodeName Node
   * \param prefix Routing prefix
   * \param faceId Face index
   * \param metric Routing metric
   */
  static void
  AddRoute(Ptr<Node> node, const std::string &prefix, uint32_t faceId, int32_t metric);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param node   Node
   * \param prefix Routing prefix
   * \param face   Face
   * \param metric Routing metric
   */
  static void
  AddRoute (Ptr<Node> node, const std::string &prefix, Ptr<Face> face, int32_t metric);

  /**
   * @brief Add forwarding entry to FIB (work only with point-to-point links)
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param otherNode The other node, to which interests (will be used to infer face id
   * \param metric Routing metric
   */
  static void
  AddRoute (Ptr<Node> node, const std::string &prefix, Ptr<Node> otherNode, int32_t metric);

  /**
   * @brief Add forwarding entry to FIB (work only with point-to-point links)
   *
   * \param nodeName Node name (refer to ns3::Names)
   * \param prefix Routing prefix
   * \param otherNode The other node name, to which interests (will be
   *                  used to infer face id (refer to ns3::Names)
   * \param metric Routing metric
   */
  static void
  AddRoute (const std::string &nodeName, const std::string &prefix,
            const std::string &otherNodeName, int32_t metric);

private:
  static void
  GenerateCommand(Interest& interest);
};

} // namespace ndn

} // namespace ns3

#endif // NDN_FIB_HELPER_H
