#ifndef NDN_NAME_H
#define NDN_NAME_H

#include <ndn-cxx/name.hpp>

namespace ns3 {

namespace ndn {

class Name : public SimpleRefCount<Name>, public ::ndn::Name
{
public:
  typedef ::ndn::name::Component partial_type;
};

}

}

#endif
