/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-l3-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"

#include <boost/lexical_cast.hpp>

#include "ns3/ndn-face.h"

using namespace std;

namespace ns3 {
namespace ndn {

using ::ndn::Interest;
using ::ndn::Data;

L3Tracer::L3Tracer (Ptr<Node> node)
: m_nodePtr (node)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

L3Tracer::L3Tracer (const std::string &node)
: m_node (node)
{
  Connect ();
}

L3Tracer::~L3Tracer ()
{
};


void
L3Tracer::Connect ()
{
  Ptr<L3Protocol> l3Protocol = m_nodePtr->GetObject<L3Protocol> ();

  l3Protocol->TraceConnectWithoutContext ("OutInterests",  MakeCallback (&L3Tracer::OutInterests, this));
  l3Protocol->TraceConnectWithoutContext ("InInterests",   MakeCallback (&L3Tracer::InInterests, this));
  // l3Protocol->TraceConnectWithoutContext ("DropInterests", MakeCallback (&L3Tracer::DropInterests, this));

  l3Protocol->TraceConnectWithoutContext ("OutData",  MakeCallback (&L3Tracer::OutData, this));
  l3Protocol->TraceConnectWithoutContext ("InData",   MakeCallback (&L3Tracer::InData, this));
  // m_node->TraceConnectWithoutContext ("DropData", MakeCallback (&L3Tracer::DropData, this));
  /*
  // only for some strategies
  fw->TraceConnectWithoutContext ("OutNacks",  MakeCallback (&L3Tracer::OutNacks, this));
  fw->TraceConnectWithoutContext ("InNacks",   MakeCallback (&L3Tracer::InNacks, this));
  fw->TraceConnectWithoutContext ("DropNacks", MakeCallback (&L3Tracer::DropNacks, this));
  */
  // satisfied/timed out PIs
  // m_node->TraceConnectWithoutContext ("SatisfiedInterests", MakeCallback (&L3Tracer::SatisfiedInterests, this));
  // m_node->TraceConnectWithoutContext ("TimedOutInterests", MakeCallback (&L3Tracer::TimedOutInterests, this));
}

} // namespace ndn
} // namespace ns3
