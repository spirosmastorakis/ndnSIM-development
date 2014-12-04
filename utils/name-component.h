#ifndef NDN_NAME_COMPONENT_H
#define NDN_NAME_COMPONENT_H

#include "blob.h"
#include <ndn-cxx/name-component.hpp>

namespace ns3 {

namespace ndn {

namespace name {

class Component : public Blob, public ::ndn::name::Component
{
public:
  Component ():
    ::ndn::name::Component ()
  {
  }

  Component (std::string str) :
    ::ndn::name::Component (str)
  {
    m_comp = str;
  }

  Component&
  operator= (const ::ndn::name::Component &other)
  {
    m_comp = other.toUri ();
    return *this;
  }

private:
  std::string m_comp;

};

}

}

}

#endif
