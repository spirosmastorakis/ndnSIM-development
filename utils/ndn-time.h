#ifndef NDN_TIME_H
#define NDN_TIME_H

#include <ndn-cxx/util/time-custom-clock.hpp>
#include <ndn-cxx/util/time.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "ns3/simulator.h"

using ndn::time::CustomClock;
using ndn::time::system_clock;
using ndn::time::steady_clock;

namespace ns3 {

namespace ndn {

class CustomSystemClock : public CustomClock<system_clock>
{
public:
  CustomSystemClock ();

  ~CustomSystemClock ();

  system_clock::time_point
  getNow() const;

  std::string
  getSince() const;

  boost::posix_time::time_duration
  toPosixDuration(const system_clock::duration& duration) const;

};

class CustomSteadyClock : public CustomClock<steady_clock>
{
public:
  CustomSteadyClock ();

  ~CustomSteadyClock ();

  steady_clock::time_point
  getNow() const;

  std::string
  getSince() const;

  boost::posix_time::time_duration
  toPosixDuration(const steady_clock::duration& duration) const;

};

} // namespace ndn

} // namespace ns3

#endif // NDN_TIME_HPP
