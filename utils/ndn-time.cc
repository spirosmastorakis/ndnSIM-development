#include "ndn-time.h"

namespace ns3 {

namespace ndn {

CustomSystemClock::CustomSystemClock ()
{
}

CustomSystemClock::~CustomSystemClock ()
{
}

system_clock::time_point
CustomSystemClock::getNow() const
{
  boost::chrono::nanoseconds ns(Simulator::Now ().GetNanoSeconds ());
  return system_clock::time_point(ns);
}

std::string
CustomSystemClock::getSince() const
{
  return "since last NFD operation";
}

boost::posix_time::time_duration
CustomSystemClock::toPosixDuration(const system_clock::duration& duration) const
{
  boost::posix_time::time_duration td(0,0,0,boost::chrono::duration_cast
                                             <boost::chrono::nanoseconds>(duration).count());
  return td;
}

CustomSteadyClock::CustomSteadyClock ()
{
}

CustomSteadyClock::~CustomSteadyClock ()
{
}

steady_clock::time_point
CustomSteadyClock::getNow() const
{
  boost::chrono::nanoseconds ns(Simulator::Now ().GetNanoSeconds ());
  return steady_clock::time_point(ns);
}

std::string
CustomSteadyClock::getSince() const
{
  return "since last NFD operation";
}

boost::posix_time::time_duration
CustomSteadyClock::toPosixDuration(const steady_clock::duration& duration) const
{
  boost::posix_time::time_duration td(0,0,0,boost::chrono::duration_cast
                                             <boost::chrono::nanoseconds>(duration).count());
  return td;
}


} // namespace ndn

} // namespace ns3
