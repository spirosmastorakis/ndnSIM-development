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

#ifndef NDN_STRATEGY_CHOICE_HELPER_H
#define NDN_STRATEGY_CHOICE_HELPER_H

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"
#include "ndn-stack-helper.hpp"

namespace ndn {
namespace nfd {
class ControlParameters;
};
};

namespace ns3 {
namespace ndn {

using ::ndn::nfd::ControlParameters;

class StrategyChoiceHelper
{
public:
  StrategyChoiceHelper ();

  ~StrategyChoiceHelper ();

  static void
  Install (const NodeContainer& c, const ::ndn::Name& namePrefix, const ::ndn::Name& strategy);

  static void
  Install (Ptr<Node> node, const ::ndn::Name& namePrefix, const ::ndn::Name& strategy);

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
