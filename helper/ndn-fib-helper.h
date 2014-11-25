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

  static void
  GenerateCommand (Interest& interest);

  static void
  AddNextHop (ControlParameters parameters, Ptr<const Node> node);

  static void
  RemoveNextHop (ControlParameters parameters, Ptr<const Node> node);
};

} // namespace ndn

} // namespace ns3

#endif // NDN_FIB_HELPER_H
