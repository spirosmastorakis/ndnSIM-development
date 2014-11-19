#include "ndnSIM-ndn-ns3.h"

#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include <ndn-cxx/common.hpp>
#include <memory>

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/encoding/block.hpp>

#include "ns3/ndnSIM/model/ndn-ns3.hpp"
#include "ns3/packet.h"
#include "ns3/log.h"


NS_LOG_COMPONENT_DEFINE ("ndn.ndn-ns3-test");

using ndn::Interest;
using ndn::Convert;
using ndn::Block;

namespace ns3 {

using ::ndn::shared_ptr;

void NdnNs3Test::CheckInterestToPacket()
{
  static const struct {
    const char  *d_name;
  } VALUES [] = {
      { ""   },
      { "foo" },
      { "/my/my/my/sharona" },

  };
  const int NUM_VALUES = sizeof VALUES / sizeof *VALUES;
  for (int ti = 0; ti < NUM_VALUES; ++ti) {
    const char *NAME = VALUES[ti].d_name;

    Ptr<Packet> ns3_packet = Create<Packet>();
    shared_ptr<Interest> interest_p = ::ndn::make_shared<Interest>(NAME);
    shared_ptr<Block>    block_p    = ::ndn::make_shared<::ndn::Block>(interest_p->wireEncode());
    
    Convert::InterestToPacket(block_p, ns3_packet);
    Interest result_interest = Interest(Convert::FromPacket(ns3_packet));
    Interest exp_interest    = *interest_p;

    NS_TEST_ASSERT_MSG_EQ(result_interest, exp_interest,  "the packets do not match");

  }
}

void NdnNs3Test::DoRun()
{
  Create<Packet> (1024);
  CheckInterestToPacket();
  // Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  // Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  // Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  // Creating nodes
  // NodeContainer nodes;
  // nodes.Create (3);

  // Connecting nodes using two links
  // PointToPointHelper p2p;
  // p2p.Install (nodes.Get (0), nodes.Get (1));
  // p2p.Install (nodes.Get (1), nodes.Get (2));

  // Install NDN stack on all nodes
  // ndn::StackHelper ndnHelper;
  // ndnHelper.SetDefaultRoutes (true);
  // ndnHelper.InstallAll ();

  // Installing applications

  // Consumer
  // ndn::AppHelper consumerHelper ("ns3::ndn::test::ApiTestClient");
  // ApplicationContainer apps = consumerHelper.Install (nodes.Get (0)); // first node

  // // Producer
  // ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  // // Producer will reply to all requests starting with /prefix
  // producerHelper.SetPrefix ("/");
  // producerHelper.SetAttribute ("Postfix", StringValue ("/unique/postfix"));
  // producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  // producerHelper.Install (nodes.Get (2)).Stop (Seconds (4.0)); // last node

  // CheckInterestToPacket();

  // Simulator::Schedule (Seconds (0.0001), &NdnNs3Test::CheckInterestToPacket, this);

  // Simulator::Stop (Seconds (20.0));

  // Simulator::Run ();
  // Simulator::Destroy (); 

}

} // ns3 namespace