/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/object.h"
#include "ndn-producer.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "ns3/ndn-app-face.h"
#include "ns3/ndnSIM/NFD/daemon/table/fib.hpp"
#include "ns3/ndn-ns3.hpp"
#include "ns3/ndn-forwarder.h"

#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include <memory>

namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ndn.Producer");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (Producer);

TypeId
Producer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::Producer")
    .SetGroupName ("Ndn")
    .SetParent<App> ()
    .AddConstructor<Producer> ()
    .AddAttribute ("Prefix","Prefix, for which producer has the data",
                    StringValue ("/"),
                    MakeNameAccessor (&Producer::m_prefix),
                    MakeNameChecker ())
     .AddAttribute ("Postfix", "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
                    StringValue ("/"),
                    MakeNameAccessor (&Producer::m_postfix),
                    MakeNameChecker ())
    .AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&Producer::m_virtualPayloadSize),
                   MakeUintegerChecker<uint32_t> ())
     .AddAttribute ("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
                    TimeValue (Seconds (0)),
                    MakeTimeAccessor (&Producer::m_freshness),
                    MakeTimeChecker ())
    .AddAttribute ("Signature", "Fake signature, 0 valid signature (default), other values application-specific",
                   UintegerValue (0),
                   MakeUintegerAccessor (&Producer::m_signature),
                   MakeUintegerChecker<uint32_t> ())
     .AddAttribute ("KeyLocator", "Name to be used for key locator.  If root, then key locator is not used",
                    NameValue (),
                    MakeNameAccessor (&Producer::m_keyLocator),
                    MakeNameChecker ())
    ;
  return tid;
}

Producer::Producer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

// inherited from Application base class.
void
Producer::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  //NS_ASSERT (GetNode ()->GetObject<L3Protocol> ()->GetForwarder ()->getFib ().m_nItems != 0);

  App::StartApplication ();

  NS_LOG_DEBUG ("NodeID: " << GetNode ()->GetId ());

  ::ndn::shared_ptr< ::nfd::fib::Entry> entry =
     GetNode ()->GetObject<L3Protocol> ()->GetForwarder ()->getFib ().insert (m_prefix).first;
  entry->addNextHop (m_face->shared_from_this (), 0);

  // fibEntry->UpdateStatus (m_face, fib::FaceMetric::NDN_FIB_GREEN);

  // // make face green, so it will be used primarily
  // StaticCast<fib::FibImpl> (fib)->modify (fibEntry,
  //                                         ll::bind (&fib::Entry::UpdateStatus,
  //                                                   ll::_1, m_face, fib::FaceMetric::NDN_FIB_GREEN));
}

void
Producer::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  //NS_ASSERT (GetNode ()->GetObject<Fib> () != 0);

  App::StopApplication ();
}


void
Producer::OnInterest (::ndn::shared_ptr<const ::ndn::Interest> interest)
{
  App::OnInterest (interest); // tracing inside

  NS_LOG_FUNCTION (this << interest);

  if (!m_active) return;

  ::ndn::shared_ptr<::ndn::Data> data(new ::ndn::Data(::ndn::Convert::FromPacket
                                                      (Create<Packet> (m_virtualPayloadSize))));

  ::ndn::shared_ptr<::ndn::Name> dataName(new ::ndn::Name(interest->getName ()));
  //::ndn::Name postfix = ::ndn::Name (m_postfix.toUri ());

  dataName->append (m_postfix);
  data->setName (*dataName);

  ::ndn::time::milliseconds freshness (m_freshness.GetMilliSeconds ());
  data->setFreshnessPeriod (freshness);

  uint64_t value = uint64_t (Simulator::Now().GetMilliSeconds ());
  ::ndn::Name::Component::Component component;
  data->setFinalBlockId (component.fromNumberWithMarker(uint8_t (0xFC), value));

  ::ndn::Block signatureValue = ::ndn::Block (&m_signature, sizeof(m_signature));
  ::ndn::Signature signature;
  signature.setValue (signatureValue);

  if (m_keyLocator.size () > 0) {
    ::ndn::SignatureInfo::SignatureInfo signatureInfo;
    ::ndn::KeyLocator keyLocator;
    keyLocator.setName (m_keyLocator);
    signatureInfo.setKeyLocator (keyLocator);
    signature.setInfo (signatureInfo);
  }

  data->setSignature (signature);

  NS_LOG_INFO ("node("<< GetNode()->GetId() <<") respodning with Data: " << data->getName ());


  // Echo back FwHopCountTag if exists
  // FwHopCountTag hopCountTag;
  // if (interest->GetPayload ()->PeekPacketTag (hopCountTag))
  //   {
  //      data->GetPayload ()->AddPacketTag (hopCountTag);
  //   }

  m_face->ReceiveData (data);
  m_transmittedDatas (data, this, m_face);
}

} // namespace ndn
} // namespace ns3
