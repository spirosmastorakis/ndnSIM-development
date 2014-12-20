#include "ndn-fib-helper.hpp"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/core-config.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/callback.h"
#include "ns3/node-list.h"
#include "ns3/data-rate.h"

#include "daemon/mgmt/fib-manager.hpp"

namespace ns3 {
namespace ndn {

NS_LOG_COMPONENT_DEFINE ("ndn.FibHelper");

using ::nfd::ControlParameters;
using ::nfd::FibManager;

FibHelper::FibHelper ()
{
}

FibHelper::~FibHelper ()
{
}

void
FibHelper::AddNextHop (const ControlParameters& parameters, Ptr<Node> node)
{
  NS_LOG_DEBUG ("Add Next Hop command was initialized");
  Block encodedParameters(parameters.wireEncode());

  Name commandName("/localhost/nfd/fib");
  commandName.append("add-nexthop");
  commandName.append(encodedParameters);

  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  StackHelper::getKeyChain().signWithSha256(*command);

  Ptr<L3Protocol> L3protocol = node->GetObject<L3Protocol> ();
  shared_ptr<FibManager> fibManager = L3protocol->GetFibManager ();
  fibManager->onFibRequest(*command);
}

void
FibHelper::RemoveNextHop (const ControlParameters& parameters, Ptr<Node> node)
{
  NS_LOG_DEBUG ("Remove Next Hop command was initialized");
  Block encodedParameters(parameters.wireEncode());

  Name commandName("/localhost/nfd/fib");
  commandName.append("remove-nexthop");
  commandName.append(encodedParameters);

  shared_ptr<Interest> command(make_shared<Interest>(commandName));
  StackHelper::getKeyChain().signWithSha256(*command);

  Ptr<L3Protocol> L3protocol = node->GetObject<L3Protocol> ();
  shared_ptr<FibManager> fibManager = L3protocol->GetFibManager ();
  // fibManager->addInterestRule(commandName.toUri(), key, *keyChain.getPublicKey (key));
  fibManager->onFibRequest(*command);
}

void
FibHelper::AddRoute (Ptr<Node> node, const std::string &prefix, Ptr<Face> face, int32_t metric)
{
  NS_LOG_LOGIC ("[" << node->GetId () << "]$ route add " << prefix << " via " << *face << " metric " << metric);

  //Get L3Protocol object
  Ptr<L3Protocol> L3protocol = node->GetObject<L3Protocol> ();
  //Get the forwarder instance
  shared_ptr<Forwarder> m_forwarder = L3protocol->GetForwarder();

  ControlParameters parameters;
  parameters.setName(prefix);
  parameters.setFaceId(face->getId ());
  parameters.setCost (metric);

  AddNextHop(parameters, node);
}

void
FibHelper::AddRoute (Ptr<Node> node, const std::string &prefix, uint32_t faceId, int32_t metric)
{
  Ptr<L3Protocol>     ndn = node->GetObject<L3Protocol> ();
  NS_ASSERT_MSG (ndn != 0, "Ndn stack should be installed on the node");

  Ptr<Face> face = ndn->GetFace (faceId);
  NS_ASSERT_MSG (face != 0, "Face with ID [" << faceId << "] does not exist on node [" << node->GetId () << "]");

  AddRoute (node, prefix, face, metric);
}

void
FibHelper::AddRoute (const std::string &nodeName, const std::string &prefix, uint32_t faceId, int32_t metric)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  NS_ASSERT_MSG (node != 0, "Node [" << nodeName << "] does not exist");

  Ptr<L3Protocol>     ndn = node->GetObject<L3Protocol> ();
  NS_ASSERT_MSG (ndn != 0, "Ndn stack should be installed on the node");

  Ptr<Face> face = ndn->GetFace (faceId);
  NS_ASSERT_MSG (face != 0, "Face with ID [" << faceId << "] does not exist on node [" << nodeName << "]");

  AddRoute (node, prefix, face, metric);
}

void
FibHelper::AddRoute (Ptr<Node> node, const std::string &prefix, Ptr<Node> otherNode, int32_t metric)
{
  for (uint32_t deviceId = 0; deviceId < node->GetNDevices (); deviceId ++)
    {
      Ptr<PointToPointNetDevice> netDevice = DynamicCast<PointToPointNetDevice> (node->GetDevice (deviceId));
      if (netDevice == 0)
        continue;

      Ptr<Channel> channel = netDevice->GetChannel ();
      if (channel == 0)
        continue;

      if (channel->GetDevice (0)->GetNode () == otherNode ||
          channel->GetDevice (1)->GetNode () == otherNode)
        {
          Ptr<L3Protocol> ndn = node->GetObject<L3Protocol> ();
          NS_ASSERT_MSG (ndn != 0, "Ndn stack should be installed on the node");

          Ptr<Face> face = ndn->GetFaceByNetDevice (netDevice);
          NS_ASSERT_MSG (face != 0, "There is no face associated with the p2p link");

          AddRoute (node, prefix, face, metric);

          return;
        }
    }

  NS_FATAL_ERROR ("Cannot add route: Node# " << node->GetId () << " and Node# " << otherNode->GetId () << " are not connected");
}

void
FibHelper::AddRoute (const std::string &nodeName, const std::string &prefix, const std::string &otherNodeName, int32_t metric)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  NS_ASSERT_MSG (node != 0, "Node [" << nodeName << "] does not exist");

  Ptr<Node> otherNode = Names::Find<Node> (otherNodeName);
  NS_ASSERT_MSG (otherNode != 0, "Node [" << otherNodeName << "] does not exist");

  AddRoute (node, prefix, otherNode, metric);
}

} // namespace ndn

} // namespace ns
