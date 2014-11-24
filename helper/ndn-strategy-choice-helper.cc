#include "ndn-strategy-choice-helper.h"

#include "ns3/log.h"

namespace ns3 {

namespace ndn {

NS_LOG_COMPONENT_DEFINE ("ndn.StrategyChoiceHelper");

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;
using ::nfd::StrategyChoiceManager;
using ::nfd::CommandValidator;

// const Name FibHelper::s_identityName("/add/FibEntry");
// shared_ptr<::ndn::IdentityCertificate> FibHelper::s_certificate;
//template void KeyChain::sign<Interest>(Interest);

StrategyChoiceHelper::StrategyChoiceHelper () :
  m_strategy ("/localhost/nfd/strategy/best-route")
{
}

StrategyChoiceHelper::StrategyChoiceHelper (Name strategy)
{
  m_strategy = strategy;
}

StrategyChoiceHelper::~StrategyChoiceHelper ()
{
}

void
StrategyChoiceHelper::StrategyChoice (ControlParameters parameters, Ptr<Node> node) const
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
StrategyChoiceHelper::Install (const NodeContainer &c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
StrategyChoiceHelper::Install (Ptr<Node> node) const
{
  ControlParameters parameters;
  parameters.setName ("/test");
  NS_LOG_DEBUG ("Node ID: " << node->GetId () << " with forwarding strategy " << m_strategy);
  parameters.setStrategy (m_strategy);
  StrategyChoice (parameters, node);
}

void
StrategyChoiceHelper::InstallAll ()
{
  Install (NodeContainer::GetGlobal ());
}

} // namespace ndn

} // namespace ns
