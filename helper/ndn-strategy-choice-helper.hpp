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
