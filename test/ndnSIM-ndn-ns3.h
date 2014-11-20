#ifndef NDNSIM_TEST_NDN_NS3_H
#define NDNSIM_TEST_NDN_NS3_H

#include "ns3/test.h"
#include "ns3/ptr.h"



namespace ns3 {

class NdnNs3Test : public TestCase
{
public:
	NdnNs3Test() : TestCase("NDN-NS3 test")
	{
	}

private:
	virtual void DoRun();

	void CheckInterestToPacket();

};

}

#endif