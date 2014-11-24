#include "ndn-fib-helper.h"

#include "ns3/log.h"

namespace ns3 {

namespace ndn {

NS_LOG_COMPONENT_DEFINE ("ndn.FibHelper");

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;
using ::nfd::FibManager;
using ::nfd::CommandValidator;

// const Name FibHelper::s_identityName("/add/FibEntry");
// shared_ptr<::ndn::IdentityCertificate> FibHelper::s_certificate;
//template void KeyChain::sign<Interest>(Interest);

FibHelper::FibHelper ()
{
  //s_certificate = m_keys.getCertificate(m_keys.createIdentity(s_identityName));
}

FibHelper::~FibHelper ()
{
  // s_certificate.reset();
  // m_keys.deleteIdentity(s_identityName);
}

void
FibHelper::GenerateCommand (Interest& interest)
{
  // m_generator.generateWithIdentity(interest, s_identityName);
}

void
FibHelper::AddNextHop (ControlParameters parameters, Ptr<Node> node)
{
  NS_LOG_DEBUG ("Add Next Hop command was initialized");
  Block encodedParameters(parameters.wireEncode());

  Name commandName("/localhost/nfd/fib");
  commandName.append("add-nexthop");
  commandName.append(encodedParameters);

  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  KeyChain keyChain;
  Name key;
  key=keyChain.generateRsaKeyPairAsDefault("add-nexthop");
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
  shared_ptr<FibManager> fibManager = L3protocol->GetFibManager ();
  // fibManager->addInterestRule(commandName.toUri(), key, *keyChain.getPublicKey (key));
  fibManager->onFibRequest(*command);
}

void
FibHelper::RemoveNextHop (ControlParameters parameters, Ptr<Node> node)
{
  NS_LOG_DEBUG ("Remove Next Hop command was initialized");
  Block encodedParameters(parameters.wireEncode());

  Name commandName("/localhost/nfd/fib");
  commandName.append("remove-nexthop");
  commandName.append(encodedParameters);

  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  KeyChain keyChain;
  Name key;
  key=keyChain.generateRsaKeyPairAsDefault("remove-nexthop");
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
  shared_ptr<FibManager> fibManager = L3protocol->GetFibManager ();
  // fibManager->addInterestRule(commandName.toUri(), key, *keyChain.getPublicKey (key));
  fibManager->onFibRequest(*command);
}

} // namespace ndn

} // namespace ns
