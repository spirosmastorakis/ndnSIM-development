#ifndef NDN_STRATEGY_CHOICE_HELPER_H
#define NDN_STRATEGY_CHOICE_HELPER_H

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-forwarder.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-stack-helper.h"

#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/common.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/command-validator.hpp"

#include <ndn-cxx/util/command-interest-generator.hpp>

namespace ns3 {

namespace ndn {

using ::ndn::CommandInterestGenerator;
using ::nfd::ControlParameters;

class StrategyChoiceHelper
{
public:
  StrategyChoiceHelper ();

  StrategyChoiceHelper (Name strategy);

  ~StrategyChoiceHelper ();

  void
  Install (const NodeContainer &c) const;

  void
  Install (Ptr<Node> node) const;

  void
  InstallAll ();

protected:
  void
  StrategyChoice (ControlParameters parameters, Ptr<Node> node) const;

  CommandInterestGenerator m_generator;

private:
  Name m_strategy;
  ::ndn::KeyChain m_keys;
};

} // namespace ndn

} // namespace ns3

#endif // NDN_STRATEGY_CHOICE_HELPER_H
