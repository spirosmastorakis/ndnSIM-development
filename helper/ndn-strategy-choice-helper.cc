#include "ndn-strategy-choice-helper.h"

#include "ns3/log.h"

namespace ns3 {

namespace ndn {

NS_LOG_COMPONENT_DEFINE ("ndn.StrategyChoiceHelper");

using ::nfd::ControlParameters;
using ::nfd::StrategyChoiceManager;

StrategyChoiceHelper::StrategyChoiceHelper ()
{
}

StrategyChoiceHelper::~StrategyChoiceHelper ()
{
}

void
StrategyChoiceHelper::StrategyChoice (ControlParameters parameters, Ptr<Node> node)
{
  NS_LOG_DEBUG ("Strategy choice command was initialized");
  Block encodedParameters(parameters.wireEncode());

  Name commandName("/localhost/nfd/strategy-choice");
  commandName.append("set");
  commandName.append(encodedParameters);

  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  KeyChain keyChain;
  Name key;
  key=keyChain.generateRsaKeyPairAsDefault("set");
  keyChain.sign(*command);
  // CommandValidator validator;
  // validator.addSupportedPrivilege("fib");
  // validator.addInterestRule(commandName.toUri(), key, *keyChain.getPublicKey (key));
  // GenerateCommand(*command);
  // NS_LOG_DEBUG ("Command was generated");
  // m_face->onReceiveData += [this, command, encodedParameters] (const Data& response) {
  //   NS_LOG_DEBUG ("NFD responded with " << response.getName());
  // };
  Ptr<L3Protocol> L3protocol = node->GetObject<L3Protocol> ();
  shared_ptr<StrategyChoiceManager> strategyChoiceManager = L3protocol->GetStrategyChoiceManager ();
  // strategyChoiceManager->addInterestRule(commandName.toUri(), key, *keyChain.getPublicKey (key));
  strategyChoiceManager->onStrategyChoiceRequest(*command);
  NS_LOG_DEBUG ("Forwarding strategy installed in node " << node->GetId ());
}

void
StrategyChoiceHelper::Install (const NodeContainer& c, const Name& namePrefix, const Name& strategy)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i, namePrefix, strategy);
    }
}

void
StrategyChoiceHelper::Install (Ptr<Node> node, const Name& namePrefix, const Name& strategy)
{
  ControlParameters parameters;
  parameters.setName (namePrefix);
  NS_LOG_DEBUG ("Node ID: " << node->GetId () << " with forwarding strategy " << strategy);
  parameters.setStrategy (strategy);
  StrategyChoice (parameters, node);
}

void
StrategyChoiceHelper::InstallAll (const Name& namePrefix, const Name& strategy)
{
  Install (NodeContainer::GetGlobal (), namePrefix, strategy);
}

} // namespace ndn

} // namespace ns
