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

#include "ndn-link-control-helper.hpp"

#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/channel.h"
#include "ns3/log.h"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-face.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.LinkControlHelper");

namespace ns3 {
namespace ndn {

void
LinkControlHelper::FailLink(Ptr<Node> node1, Ptr<Node> node2)
{
  NS_LOG_FUNCTION(node1 << node2);

  NS_ASSERT(node1 != 0);
  NS_ASSERT(node2 != 0);

  Ptr<ndn::L3Protocol> ndn1 = node1->GetObject<ndn::L3Protocol>();
  Ptr<ndn::L3Protocol> ndn2 = node2->GetObject<ndn::L3Protocol>();

  NS_ASSERT(ndn1 != 0);
  NS_ASSERT(ndn2 != 0);

  // iterate over all faces to find the right one
  for (const auto& i : ndn1->getForwarder()->getFaceTable()) {
    shared_ptr<ndn::NetDeviceFace> ndFace = std::dynamic_pointer_cast<NetDeviceFace>(i);
    if (ndFace == 0)
      continue;

    Ptr<PointToPointNetDevice> nd1 = ndFace->GetNetDevice()->GetObject<PointToPointNetDevice>();
    if (nd1 == 0)
      continue;

    Ptr<Channel> channel = nd1->GetChannel();
    if (channel == 0)
      continue;

    Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel>(channel);

    Ptr<NetDevice> nd2 = ppChannel->GetDevice(0);
    if (nd2->GetNode() == node1)
      nd2 = ppChannel->GetDevice(1);

    if (nd2->GetNode() == node2) {
      shared_ptr<Face> face1 = ndn1->GetFaceByNetDevice(nd1);
      shared_ptr<Face> face2 = ndn2->GetFaceByNetDevice(nd2);

      // face1->SetUp(false);
      // face2->SetUp(false);
      break;
    }
  }
}
void
LinkControlHelper::FailLinkByName(const std::string& node1, const std::string& node2)
{
  FailLink(Names::Find<Node>(node1), Names::Find<Node>(node2));
}

void
LinkControlHelper::UpLink(Ptr<Node> node1, Ptr<Node> node2)
{
  NS_LOG_FUNCTION(node1 << node2);

  NS_ASSERT(node1 != 0);
  NS_ASSERT(node2 != 0);

  Ptr<ndn::L3Protocol> ndn1 = node1->GetObject<ndn::L3Protocol>();
  Ptr<ndn::L3Protocol> ndn2 = node2->GetObject<ndn::L3Protocol>();

  NS_ASSERT(ndn1 != 0);
  NS_ASSERT(ndn2 != 0);

  // iterate over all faces to find the right one
  for (const auto& i : ndn1->getForwarder()->getFaceTable()) {
    shared_ptr<ndn::NetDeviceFace> ndFace = std::dynamic_pointer_cast<NetDeviceFace>(i);
    if (ndFace == 0)
      continue;

    Ptr<PointToPointNetDevice> nd1 = ndFace->GetNetDevice()->GetObject<PointToPointNetDevice>();
    if (nd1 == 0)
      continue;

    Ptr<Channel> channel = nd1->GetChannel();
    if (channel == 0)
      continue;

    Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel>(channel);

    Ptr<NetDevice> nd2 = ppChannel->GetDevice(0);
    if (nd2->GetNode() == node1)
      nd2 = ppChannel->GetDevice(1);

    if (nd2->GetNode() == node2) {
      shared_ptr<Face> face1 = ndn1->GetFaceByNetDevice(nd1);
      shared_ptr<Face> face2 = ndn2->GetFaceByNetDevice(nd2);

      // face1->SetUp(true);
      // face2->SetUp(true);
      break;
    }
  }
}

void
LinkControlHelper::UpLinkByName(const std::string& node1, const std::string& node2)
{
  UpLink(Names::Find<Node>(node1), Names::Find<Node>(node2));
}
}
}
