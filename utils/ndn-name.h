#ifndef NDN_NAME2_H
#define NDN_NAME2_H

#include "ns3/ndn-common.h"
#include "ns3/simple-ref-count.h"
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include <ndn-cxx/name.hpp>

namespace ns3 {

namespace ndn {

 class Name : public SimpleRefCount<Name>, public ::ndn::Name
{
public:
  //Name ();

};

ATTRIBUTE_HELPER_HEADER (Name);

}

}

#endif
