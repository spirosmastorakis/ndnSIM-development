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

#include "ndn-l3-aggregate-tracer.hpp"

#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"

#include "apps/ndn-app.hpp"
#include "model/ndn-app-face.hpp"

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/log.h"

#include <fstream>

NS_LOG_COMPONENT_DEFINE("ndn.L3AggregateTracer");

namespace ns3 {
namespace ndn {

static std::list<std::tuple<boost::shared_ptr<std::ostream>, std::list<Ptr<L3AggregateTracer>>>>
  g_tracers;

template<class T>
static inline void
NullDeleter(T* ptr)
{
}

void
L3AggregateTracer::Destroy()
{
  g_tracers.clear();
}

void
L3AggregateTracer::InstallAll(const std::string& file, Time averagingPeriod /* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3AggregateTracer>> tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    boost::shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = boost::shared_ptr<std::ostream>(&std::cout, NullDeleter<std::ostream>);
  }

  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    Ptr<L3AggregateTracer> trace = Install(*node, outputStream, averagingPeriod);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

void
L3AggregateTracer::Install(const NodeContainer& nodes, const std::string& file,
                           Time averagingPeriod /* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3AggregateTracer>> tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    boost::shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = boost::shared_ptr<std::ostream>(&std::cout, NullDeleter<std::ostream>);
  }

  for (NodeContainer::Iterator node = nodes.Begin(); node != nodes.End(); node++) {
    Ptr<L3AggregateTracer> trace = Install(*node, outputStream, averagingPeriod);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

void
L3AggregateTracer::Install(Ptr<Node> node, const std::string& file,
                           Time averagingPeriod /* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3AggregateTracer>> tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    boost::shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = boost::shared_ptr<std::ostream>(&std::cout, NullDeleter<std::ostream>);
  }

  Ptr<L3AggregateTracer> trace = Install(node, outputStream, averagingPeriod);
  tracers.push_back(trace);

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

Ptr<L3AggregateTracer>
L3AggregateTracer::Install(Ptr<Node> node, boost::shared_ptr<std::ostream> outputStream,
                           Time averagingPeriod /* = Seconds (0.5)*/)
{
  NS_LOG_DEBUG("Node: " << node->GetId());

  Ptr<L3AggregateTracer> trace = Create<L3AggregateTracer>(outputStream, node);
  trace->SetAveragingPeriod(averagingPeriod);

  return trace;
}

L3AggregateTracer::L3AggregateTracer(boost::shared_ptr<std::ostream> os, Ptr<Node> node)
  : L3Tracer(node)
  , m_os(os)
{
  Reset();
}

L3AggregateTracer::L3AggregateTracer(boost::shared_ptr<std::ostream> os, const std::string& node)
  : L3Tracer(node)
  , m_os(os)
{
  Reset();
}

L3AggregateTracer::~L3AggregateTracer(){};

void
L3AggregateTracer::SetAveragingPeriod(const Time& period)
{
  m_period = period;
  m_printEvent.Cancel();
  m_printEvent = Simulator::Schedule(m_period, &L3AggregateTracer::PeriodicPrinter, this);
}

void
L3AggregateTracer::PeriodicPrinter()
{
  Print(*m_os);
  Reset();

  m_printEvent = Simulator::Schedule(m_period, &L3AggregateTracer::PeriodicPrinter, this);
}

void
L3AggregateTracer::PrintHeader(std::ostream& os) const
{
  os << "Time"
     << "\t"

     << "Node"
     << "\t"
     << "FaceId"
     << "\t"
     << "FaceDescr"
     << "\t"

     << "Type"
     << "\t"
     << "Packets"
     << "\t"
     << "Kilobytes";
}

void
L3AggregateTracer::Reset()
{
  for (std::map<shared_ptr<const Face>, std::tuple<Stats, Stats>>::iterator stats = m_stats.begin();
       stats != m_stats.end(); stats++) {
    std::get<0>(stats->second).Reset();
    std::get<1>(stats->second).Reset();
  }
}

#define STATS(INDEX) std::get<INDEX>(stats->second)

#define PRINTER(printName, fieldName)                                                              \
  os << time.ToDouble(Time::S) << "\t" << m_node << "\t";                                          \
  if (stats->first) {                                                                              \
    os << stats->first->getId() << "\t" << stats->first->getLocalUri() << "\t";                    \
  }                                                                                                \
  else {                                                                                           \
    os << "-1\tall\t";                                                                             \
  }                                                                                                \
  os << printName << "\t" << STATS(0).fieldName << "\t" << STATS(1).fieldName / 1024.0 << "\n";

void
L3AggregateTracer::Print(std::ostream& os) const
{
  Time time = Simulator::Now();

  for (std::map<shared_ptr<const Face>, std::tuple<Stats, Stats>>::iterator stats = m_stats.begin();
       stats != m_stats.end(); stats++) {
    if (!stats->first)
      continue;

    PRINTER("InInterests", m_inInterests);
    PRINTER("OutInterests", m_outInterests);

    PRINTER("InData", m_inData);
    PRINTER("OutData", m_outData);
  }

  {
    std::map<shared_ptr<const Face>, std::tuple<Stats, Stats>>::iterator stats =
      m_stats.find(shared_ptr<const Face>(0));
    if (stats != m_stats.end()) {
      // PRINTER ("SatisfiedInterests", m_satisfiedInterests);
      // PRINTER ("TimedOutInterests", m_timedOutInterests);
    }
  }
}

void
L3AggregateTracer::OutInterests(const Interest& interest, const Face& face)
{
  std::get<0>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_outInterests++;
  if (interest.hasWire()) {
    std::get<1>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_outInterests +=
      interest.wireEncode().size();
  }
}

void
L3AggregateTracer::InInterests(const Interest& interest, const Face& face)
{
  std::get<0>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_inInterests++;
  if (interest.hasWire()) {
    std::get<1>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_inInterests +=
      interest.wireEncode().size();
  }
}

void
L3AggregateTracer::OutData(const Data& data, const Face& face)
{
  std::get<0>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_outData++;
  if (data.hasWire()) {
    std::get<1>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_outData +=
      data.wireEncode().size();
  }
}

void
L3AggregateTracer::InData(const Data& data, const Face& face)
{
  std::get<0>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_inData++;
  if (data.hasWire()) {
    std::get<1>(m_stats[(const_cast<Face*>(&face))->shared_from_this()]).m_inData +=
      data.wireEncode().size();
  }
}

// void
// L3AggregateTracer::SatisfiedInterests (Ptr<const pit::Entry>)
// {
//   m_stats[0].get<0> ().m_satisfiedInterests ++;
//   // no "size" stats
// }

// void
// L3AggregateTracer::TimedOutInterests (Ptr<const pit::Entry>)
// {
//   m_stats[0].get<0> ().m_timedOutInterests ++;
//   // no "size" stats
// }

} // namespace ndn
} // namespace ns3
