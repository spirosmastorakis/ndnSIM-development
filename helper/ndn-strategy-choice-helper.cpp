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

#include "ndn-strategy-choice-helper.hpp"

#include "ns3/log.h"

#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"

namespace ns3 {
namespace ndn {

NS_LOG_COMPONENT_DEFINE ("ndn.StrategyChoiceHelper");

using ::nfd::StrategyChoiceManager;
using ::nfd::StrategyChoice;
using ::nfd::fw::Strategy;
using ::nfd::Forwarder;

StrategyChoiceHelper::StrategyChoiceHelper ()
{
}

StrategyChoiceHelper::~StrategyChoiceHelper ()
{
}

void
StrategyChoiceHelper::StrategyChoice (const ControlParameters& parameters, Ptr<Node> node)
{
  NS_LOG_DEBUG ("Strategy choice command was initialized");
  Block encodedParameters(parameters.wireEncode());

  Name commandName("/localhost/nfd/strategy-choice");
  commandName.append("set");
  commandName.append(encodedParameters);

  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  StackHelper::getKeyChain().signWithSha256(*command);
  Ptr<L3Protocol> L3protocol = node->GetObject<L3Protocol> ();
  shared_ptr<StrategyChoiceManager> strategyChoiceManager = L3protocol->GetStrategyChoiceManager ();
  strategyChoiceManager->onStrategyChoiceRequest(*command);
  NS_LOG_DEBUG ("Forwarding strategy installed in node " << node->GetId ());
}

void
StrategyChoiceHelper::Install (const NodeContainer& c, const ::ndn::Name& namePrefix, const ::ndn::Name& strategy)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i, namePrefix, strategy);
    }
}

void
StrategyChoiceHelper::Install (Ptr<Node> node, const ::ndn::Name& namePrefix, const ::ndn::Name& strategy)
{
  ControlParameters parameters;
  parameters.setName (namePrefix);
  NS_LOG_DEBUG ("Node ID: " << node->GetId () << " with forwarding strategy " << strategy);
  parameters.setStrategy (strategy);
  StrategyChoice (parameters, node);
}

void
StrategyChoiceHelper::InstallAll (const ::ndn::Name& namePrefix, const ::ndn::Name& strategy)
{
  Install (NodeContainer::GetGlobal (), namePrefix, strategy);
}

// void
// StrategyChoiceHelper::InstallCustomStrategy (Ptr<Node> node, const Name& customStrategy)
// {
//   Ptr<L3Protocol> l3Protocol = node->GetObject<L3Protocol> ();
//   //shared_ptr<Forwarder>
//   //Strategy strategy (*(l3Protocol->GetForwarder ()), customStrategy);
//   //shared_ptr<Strategy> strategy = make_shared<customStrategy>
//     (*(l3Protocol->GetForwarder ()), customStrategy);
//   //StrategyChoice strategyChoice;
//   strategyChoice.install (strategy);
// }

// void
// StrategyChoiceHelper::InstallCustomStrategy (const NodeContainer& c, const Name& customStrategy)
// {
//   for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
//     {
//       InstallCustomStrategy (*i, customStrategy);
//     }
// }

// void
// StrategyChoiceHelper::InstallCustomStrategyAll (const Name& customStrategy)
// {
//   InstallCustomStrategy (NodeContainer::GetGlobal (), customStrategy);
// }

} // namespace ndn

} // namespace ns
