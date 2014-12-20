/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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

// custom-app.cc

#include "custom-app.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndn-fib-helper.hpp"
#include "ns3/ndn-l3-protocol.hpp"
#include "ns3/ndn-app-face.hpp"

#include "ns3/random-variable.h"

NS_LOG_COMPONENT_DEFINE ("CustomApp");

namespace ns3 {

using nfd::ControlParameters;
using ndn::FibHelper;

NS_OBJECT_ENSURE_REGISTERED (CustomApp);

// register NS-3 type
TypeId
CustomApp::GetTypeId ()
{
  static TypeId tid = TypeId ("CustomApp")
    .SetParent<ndn::App> ()
    .AddConstructor<CustomApp> ()
    ;
  return tid;
}

CustomApp::CustomApp ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

// Processing upon start of the application
void
CustomApp::StartApplication ()
{
  // initialize ndn::App
  ndn::App::StartApplication ();

  Simulator::Schedule (Seconds (1.0), &CustomApp::SendInterest, this);

  // Create a name components object for name ``/prefix/sub``
  shared_ptr<Name> prefix = make_shared<Name> (); // now prefix contains ``/``
  prefix->append ("prefix"); // now prefix contains ``/prefix``
  prefix->append ("sub"); // now prefix contains ``/prefix/sub``

  /////////////////////////////////////////////////////////////////////////////
  // Creating FIB entry that ensures that we will receive incoming Interests //
  /////////////////////////////////////////////////////////////////////////////
  ControlParameters parameters;
  parameters.setName(*prefix);
  parameters.setFaceId(m_face->getId ());
  parameters.setCost (0);

  FibHelper fibHelper;
  Ptr<Node> node = GetNode();
  fibHelper.AddNextHop(parameters, node);
}

// Processing when application is stopped
void
CustomApp::StopApplication ()
{
  // cleanup ndn::App
  ndn::App::StopApplication ();
}

void
CustomApp::SendInterest ()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  shared_ptr<Name> prefix = make_shared<Name> ("/prefix/sub"); // another way to create name

  // Create and configure ndn::Interest
  shared_ptr<Interest> interest = make_shared<Interest> ();
  UniformVariable rand (0,std::numeric_limits<uint32_t>::max ());
  interest->setNonce            (rand.GetValue ());
  interest->setName             (*prefix);
  interest->setInterestLifetime (::ndn::time::seconds (uint64_t (1.0)));

  NS_LOG_DEBUG ("Sending Interest packet for " << *prefix);

  // Call trace (for logging purposes)
  m_transmittedInterests (interest, this, m_face);

  m_face->onReceiveInterest (*interest);
}

// Callback that will be called when Interest arrives
void
CustomApp::OnInterest (shared_ptr<const Interest> interest)
{
  ndn::App::OnInterest (interest);

  NS_LOG_DEBUG ("Received Interest packet for " << interest->getName ());

  // Note that Interests send out by the app will not be sent back to the app !

  Name dataName(interest->getName());

  auto data = make_shared<Data>(dataName);
  data->setFreshnessPeriod(::ndn::time::milliseconds(uint64_t (1000)));

  data->setContent(make_shared<::ndn::Buffer>(1024));

  Signature signature;
  SignatureInfo signatureInfo(static_cast<::ndn::tlv::SignatureTypeValue>(255));

  int sign = 0;

  signature.setInfo (signatureInfo);
  signature.setValue(Block(&sign, sizeof(sign)));

  data->setSignature(signature);

  // to create real wire encoding
  data->wireEncode();

  NS_LOG_DEBUG ("Sending Data packet for " << data->getName ());

  // Call trace (for logging purposes)
  m_transmittedDatas (data, this, m_face);

  m_face->onReceiveData (*data);
}

// Callback that will be called when Data arrives
void
CustomApp::OnData (shared_ptr<const Data> contentObject)
{
  NS_LOG_DEBUG ("Receiving Data packet for " << contentObject->getName ());

  std::cout << "DATA received for name " << contentObject->getName () << std::endl;
}

} // namespace ns3
