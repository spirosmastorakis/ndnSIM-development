#ifndef NDN_STRATEGY_CHOICE_HELPER_H
#define NDN_STRATEGY_CHOICE_HELPER_H

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-l3-protocol.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-stack-helper.h"

#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/strategy-choice.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/strategy.hpp"
#include "ns3/ndnSIM/NFD/common.hpp"

#include <ndn-cxx/util/command-interest-generator.hpp>

namespace ns3 {

namespace ndn {

using ::nfd::ControlParameters;

class StrategyChoiceHelper
{
public:
  StrategyChoiceHelper ();

  ~StrategyChoiceHelper ();

  static void
  Install (const NodeContainer& c, const Name& namePrefix, const Name& strategy);

  static void
  Install (Ptr<Node> node, const Name& namePrefix, const Name& strategy);

  static void
  InstallAll (const Name& namePrefix, const Name& strategy);

  // static void
  // InstallCustomStrategy (Ptr<Node> node, const Name& customStrategy);

  // static void
  // InstallCustomStrategy (const NodeContainer& c, const Name& customStrategy);

  // static void
  // InstallCustomStrategyAll (const Name& strategy);

protected:
  static void
  StrategyChoice (ControlParameters parameters, Ptr<Node> node);
};

} // namespace ndn

} // namespace ns3

#endif // NDN_STRATEGY_CHOICE_HELPER_H
