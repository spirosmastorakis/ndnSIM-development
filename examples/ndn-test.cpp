/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2014  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// ndn-simple-with-cs-lfu.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include <sys/time.h>
#include "ns3/ndnSIM/utils/mem-usage.hpp"
#include "ns3/ndnSIM/model/cs/ndn-content-store.hpp"

int interestRate = 1000;

namespace ns3 {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 * This scenario demonstrates how to use content store that responds to Freshness parameter set in
 *Datas.
 * That is, if producer set Freshness field to 2 seconds, the corresponding content object will not
 *be cached
 * more than 2 seconds (can be cached for a shorter time, if entry is evicted earlier)
 *
 *     NS_LOG=ndn.Consumer ./waf --run ndn-test
 */

void
PrintInterestStatsHeader(std::ostream& os)
{
  os << "SimulationTime"
     << "\t"
     << "RealTime"
     << "\t"
     << "NumberOfInterests"
     << "\n";
}

void
PrintInterestStats(std::ostream& os, Time nextPrintTime, double beginRealTime)
{
  ::timeval t;
  gettimeofday(&t, NULL);
  double realTime = t.tv_sec + (0.000001 * (unsigned)t.tv_usec) - beginRealTime;
  double simTime = Simulator::Now().ToDouble(Time::S);

  os << simTime << "\t";
  os << realTime << "\t";

  // os << ndn::L3Protocol::GetDataCounter () << "\t";
  // os << ndn::L3Protocol::GetInterestCounter () << "\t";

  uint64_t interestCount = 0;
  interestCount = interestRate * int(simTime);
  interestCount = interestCount / int(realTime);

  os << interestCount << "\n";

  Simulator::Schedule(nextPrintTime, PrintInterestStats, boost::ref(os), nextPrintTime, beginRealTime);
}

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(2);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue(std::to_string(interestRate)));
  consumerHelper.Install(nodes.Get(0));                        // first node

  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(1)); // last node

  Simulator::Stop(Seconds(2000.5));

  struct ::timeval t;
  gettimeofday(&t, NULL);
  double beginRealTime = t.tv_sec + (0.000001 * (unsigned)t.tv_usec);
  Simulator::Schedule(Seconds(0), PrintInterestStatsHeader, boost::ref(std::cout));
  Simulator::Schedule(Seconds(10), PrintInterestStats, boost::ref(std::cout), Seconds(10),
                      beginRealTime);

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
