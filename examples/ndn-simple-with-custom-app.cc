/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

// ndn-simple-with-custom-app.cc

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;
using ns3::ndn::StackHelper;
using ns3::ndn::AppHelper;
using ns3::ndn::L3AggregateTracer;

/**
 * This scenario simulates a two-node one-custom-app scenario:
 *
 *                          +------+      +------+
 *  Custom Consumer App --> | Node | <--> | Node | <-- Producer App
 *                          +------+      +------+
 *
 *     NS_LOG=CustomApp:ndn.Producer ./waf --run=ndn-simple-with-custom-app
 */

int
main (int argc, char *argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create (2);

  // Connecting nodes using one links
  PointToPointHelper p2p;
  p2p.Install (nodes.Get (0), nodes.Get (1));

  // Install CCNx stack on all nodes
  StackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes (true);
  ccnxHelper.InstallAll ();

  // Installing applications

  // Consumer
  AppHelper consumerHelper ("CustomApp");
  consumerHelper.Install (nodes.Get (0));

  // Producer
  AppHelper producerHelper ("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix ("/prefix");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (nodes.Get (1));

  Simulator::Stop (Seconds (3.0));

  //Install tracer
  L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (0.5));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
