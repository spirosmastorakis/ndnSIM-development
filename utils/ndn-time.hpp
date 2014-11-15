#ifndef NDN_TIME_HPP
#define NDN_TIME_HPP

#include "ns3/ndnSIM/ndn-cxx/src/util/time-custom-clock.hpp"
#include "ns3/ndnSIM/ndn-cxx/src/util/time.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

using ndn::time::CustomClock;
using ndn::time::system_clock;
using ndn::time::steady_clock;

namespace ndn {

namespace time {

template < >
class CustomClock <system_clock>
{
public:
  CustomClock ();

  ~CustomClock ();

  system_clock::time_point
  getNow() const
  {
    return  (ns3::Simulator::Now ().GetMilliSeconds ());
  }

  std::string
  getSince() const;

  boost::posix_time::time_duration
  toPosixDuration(const system_clock::duration& duration) const;

};

template < >
class CustomClock <steady_clock>
{
public:
  CustomClock ();

  ~CustomClock ();

  steady_clock::time_point
  getNow() const;

  std::string
  getSince() const;

  boost::posix_time::time_duration
  toPosixDuration(const steady_clock::duration& duration) const;

};


} // namespace time

} // namespace ndn

#endif // NDN_TIME_HPP
