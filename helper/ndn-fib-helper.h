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

#include <ndn-cxx/util/command-interest-generator.hpp>

namespace ns3 {

namespace ndn {

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;

class FibHelper
{
public:
  FibHelper ();

  FibHelper (ControlParameters parameters, Ptr<Face> face, Ptr<Node> node);

  ~FibHelper ();

  void
  SetFace (Ptr<Face> face);

  Ptr<Face>
  GetFace ();

  void
  SetControlParameters (ControlParameters parameters);

  ControlParameters
  GetControlParameters ();

  void
  SendCommand (Name commandName);

protected:
 CommandInterestGenerator m_generator;

private:
  Ptr<Face> m_face;
  ControlParameters m_parameters;
  Ptr<Node> m_node;
};

} // namespace ndn

} // namespace ns3

#endif // NDN_FIB_HELPER_H
