#include "ndn-strategy-choice-helper.h"

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
  StackHelper::getKeyChain().sign(*command);
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
