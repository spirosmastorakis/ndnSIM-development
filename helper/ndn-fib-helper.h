#ifndef NDN_FIB_HELPER_H
#define NDN_FIB_HELPER_H

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-forwarder.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-stack-helper.h"

#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/common.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/command-validator.hpp"

#include <ndn-cxx/util/command-interest-generator.hpp>

namespace ns3 {

namespace ndn {

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;

class FibHelper
{
public:
  FibHelper ();

  ~FibHelper ();

  void
  GenerateCommand (Interest& interest);

  void
  AddNextHop (ControlParameters parameters, Ptr<Node> node);

  void
  RemoveNextHop (ControlParameters parameters, Ptr<Node> node);

protected:
 CommandInterestGenerator m_generator;

private:
  // Ptr<Face> m_face;
  // ControlParameters m_parameters;
  // Ptr<Node> m_node;
  ::ndn::KeyChain m_keys;
  // static const Name s_identityName;
  // static shared_ptr<::ndn::IdentityCertificate> s_certificate;
};

} // namespace ndn

} // namespace ns3

#endif // NDN_FIB_HELPER_H
