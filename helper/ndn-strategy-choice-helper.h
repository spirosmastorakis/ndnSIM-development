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

#ifndef NDN_STRATEGY_CHOICE_HELPER_H
#define NDN_STRATEGY_CHOICE_HELPER_H

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
}; // namespace nfd
}; // namespace ndn

namespace ns3 {
namespace ndn {

using ::ndn::nfd::ControlParameters;

/**
 * @ingroup ndn-helpers
 * @brief This class describes the basic functionality
 * of the strategy choice helper that interacts with
 * the strategy choice manager of NFD
 */
class StrategyChoiceHelper
{
public:

  /**
   * \brief Default constructor
   */
  StrategyChoiceHelper ();

  /**
   * \brief Destructor
   */
  ~StrategyChoiceHelper ();

  /**
   * \brief Install the strategy choice helper to a number of nodes included
   * in a node container
   *
   * @param c The node container including the nodes
   * @param namePrefix The requested forwarding strategy would be
   * applied to that name prefix
   * @param strategy The name of the desired forwarding strategy
   */
  static void
  Install (const NodeContainer& c, const ::ndn::Name& namePrefix, const ::ndn::Name& strategy);

  /**
   * \brief Install the strategy choice helper to a node
   *
   * @param node Install the strategy choice helper to that node
   * @param namePrefix The requested forwarding strategy would be
   * applied to that name prefix
   * @param strategy The name of the desired forwarding strategy
   */
  static void
  Install (Ptr<Node> node, const ::ndn::Name& namePrefix, const ::ndn::Name& strategy);

  /**
   * \brief Install the strategy choice helper to all the nodes
   *
   * @param namePrefix The requested forwarding strategy would be
   * applied to that name prefix
   * @param strategy The name of the desired forwarding strategy
   */
  static void
  InstallAll (const ::ndn::Name& namePrefix, const ::ndn::Name& strategy);

  // static void
  // InstallCustomStrategy (Ptr<Node> node, const Name& customStrategy);

  // static void
  // InstallCustomStrategy (const NodeContainer& c, const Name& customStrategy);

  // static void
  // InstallCustomStrategyAll (const Name& strategy);

private:
  static void
  StrategyChoice (const ControlParameters& parameters, Ptr<Node> node);
};

} // namespace ndn
} // namespace ns3

#endif // NDN_STRATEGY_CHOICE_HELPER_H
