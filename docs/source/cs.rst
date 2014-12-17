.. _content store:

Content Store
+++++++++++++

ndnSIM provides the option to the user to select between two possible content store structures. The
first choice is the content store structure included in NFD. This content store structure takes
selectors into consideration, but it is not flexible when it comes to cache replacement policies. The
second choice is the content store included in the previous version of the simulator. This content
store structure is very flexible and offers a number of already implement cache replacement policies.

The choice can be declared in the code of the simulation scenario. To use the NFD's content store, one
has to type:

      .. code-block:: c++

         ndnHelper.SetContentStoreChoice (true);
         ...
         ndnHelper.Install (nodes);

To use the content store of the original ndnSIM's content store:

      .. code-block:: c++

         ndnHelper.SetContentStoreChoice (false);
         ...
         ndnHelper.Install (nodes);

It should be noted that the default choice is the use of NFD's content store.

ndnSIM's original Content Store
+++++++++++++++++++++++++++++++

ndnSIM comes with several different in-memory :ndnsim:`content store <ndn::ContentStore>` implementations, featuring different cache replacement policies.

.. note:

    The default content store uses LRU replacement policity and constrained with 100 cached ContentObjects.

To select a particular content store and configure its capacity, use :ndnsim:`SetContentStore <ndn::StackHelper::SetContentStore>` helper method

Simple content stores
^^^^^^^^^^^^^^^^^^^^^

Least Recently Used (LRU) (default)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ndnsim:`ndn::cs::Lru`.

Usage example:

      .. code-block:: c++

         ndnHelper.SetContentStore ("ns3::ndn::cs::Lru",
                                    "MaxSize", "10000");
	 ...
	 ndnHelper.Install (nodes);

First-In-First-Out (FIFO)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ndnsim:`ndn::cs::Fifo`

Usage example:

      .. code-block:: c++

         ndnHelper.SetContentStore ("ns3::ndn::cs::Fifo",
                                    "MaxSize", "10000");
	 ...
	 ndnHelper.Install (nodes);

Random
~~~~~~

Implementation name: :ndnsim:`ndn::cs::Random`

Usage example:

      .. code-block:: c++

         ndnHelper.SetContentStore ("ns3::ndn::cs::Random",
                                    "MaxSize", "10000");
	 ...
	 ndnHelper.Install (nodes);

.. note::

    If ``MaxSize`` parameter is omitted, then will be used a default value (100).

.. note::

    If ``MaxSize`` is set to 0, then no limit on ContentStore will be enforced

Nocache
~~~~~~~

:ndnsim:`Trivial implementation <ndn::cs::Nocache>` of the ContentStore that does not really do any caching.

Usage example:

      .. code-block:: c++

         ndnHelper.SetContentStore ("ns3::ndn::cs::Nocache");
	 ...
	 ndnHelper.Install (nodes);

Content stores with entry lifetime tracking
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to evaluate lifetime of the content store entries, the special versions of the content store need to be used:

Least Recently Used (LRU)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ndnsim:`ndn::cs::Stats::Lru`.

Usage example:

      .. code-block:: c++

         void
         CacheEntryRemoved (std::string context, Ptr<const ndn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName () << " " << lifetime.ToDouble (Time::S) << "s" << std::endl;
         }

         ...

         ndnHelper.SetContentStore ("ns3::ndn::cs::Stats::Lru",
                                    "MaxSize", "10000");
	 ...
	 ndnHelper.Install (nodes);

         // connect to lifetime trace
         Config::Connect ("/NodeList/*/$ns3::ndn::cs::Stats::Lru/WillRemoveEntry", MakeCallback (CacheEntryRemoved));


First-In-First-Out (FIFO)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ndnsim:`ndn::cs::Stats::Fifo`.

Usage example:

      .. code-block:: c++

         void
         CacheEntryRemoved (std::string context, Ptr<const ndn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName () << " " << lifetime.ToDouble (Time::S) << "s" << std::endl;
         }

         ...

         ndnHelper.SetContentStore ("ns3::ndn::cs::Stats::Fifo",
                                    "MaxSize", "10000");
	 ...
	 ndnHelper.Install (nodes);

         // connect to lifetime trace
         Config::Connect ("/NodeList/*/$ns3::ndn::cs::Stats::Fifo/WillRemoveEntry", MakeCallback (CacheEntryRemoved));

Random
~~~~~~

Implementation name: :ndnsim:`ndn::cs::Stats::Random`

Usage example:

      .. code-block:: c++

         void
         CacheEntryRemoved (std::string context, Ptr<const ndn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName () << " " << lifetime.ToDouble (Time::S) << "s" << std::endl;
         }

         ...

         ndnHelper.SetContentStore ("ns3::ndn::cs::Stats::Random",
                                    "MaxSize", "10000");
	 ...
	 ndnHelper.Install (nodes);

         // connect to lifetime trace
         Config::Connect ("/NodeList/*/$ns3::ndn::cs::Stats::Random/WillRemoveEntry", MakeCallback (CacheEntryRemoved));

.. _Content Store respecting freshness field of ContentObjects:

Content stores respecting freshness field of ContentObjects
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If simulations need Content Store which respects freshness of ContentObjects, the following versions of content store should be used:

.. note:

    Please note that currently, Freshness granularity is 1 second and maximum value is 65535 second. Value means infinity.

Least Recently Used (LRU)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ndnsim:`ndn::cs::Freshness::Lru`.

Usage example:

      .. code-block:: c++

         ...

         ndnHelper.SetContentStore ("ns3::ndn::cs::Freshness::Lru",
                                    "MaxSize", "10000");
	 ...

First-In-First-Out (FIFO)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ndnsim:`ndn::cs::Freshness::Fifo`

Usage example:

      .. code-block:: c++

         ...

         ndnHelper.SetContentStore ("ns3::ndn::cs::Freshness::Fifo",
                                    "MaxSize", "10000");
	 ...

Random
~~~~~~

Implementation name: :ndnsim:`ndn::cs::Freshness::Random`

Usage example:

      .. code-block:: c++

         ...

         ndnHelper.SetContentStore ("ns3::ndn::cs::Freshness::Random",
                                    "MaxSize", "10000");
	 ...


NFD's Content Store
+++++++++++++++++++

The current implementation of CS uses a skip list as its underlying data structure. Skip lists are a
probabilistic alternative to balanced trees. Skip lists are balanced by virtue of a random number
generator. Its average insertion and lookup complexity is O(log n). CS entries are placed in the Skip List in ascending order (by Name).

The current implementation evicts CS entries based on prioritized FIFO (First In First Out) strategy.
The entries that get removed first are unsolicited Data packets, which are the Data packets that got
cached opportunistically without preceding forwarding of the corresponding Interest packet. Next, the
Data packets with expired freshness are removed. Lastly, the Data packets are removed from the
Content Store on a pure FIFO basis. This cache replacement policy is currently hard-coded; it is
intended to be replaceable in the future by the NFD developer team.

CS entry
~~~~~~~~

The Data packet, along with other necessary fields, are stored in a CS entry.
Each entry contains:

• the Data packet
• whether the Data packet is unsolicited
• the timestamp at which the cached Data becomes stale

CS
~~~

A multi-index container is maintained in order to support the prioritized FIFO cache replacement policy.
In this way, pointers to the Data packets in a particular order are kept. Note that this multi-index
container is completely separated from the skip list container, which indexes Content Store entries by
name.

The container (Cs::CleanupIndex) currently supports indexing of unsolicited Data packets, indexing by
packet staleness and indexing by packet arrival time. Calculation of the indexes is performed in the
container during the Data packet insertion (Cs::insert) in the Content Store.

Eviction (Cs::evictItem) is performed during the insertion when the CS is full, and when the capacity
is decreased by management. We decided not to perform periodical cleanups, because its CPU overhead
causes jitter in packet forwarding.

In the current version of NFD, cache replacement policy can be modified by adding different indexes in
the Cs::CleanupIndex container (refer to Boost.multiIndex documentation) and implementing
additional logic in Cs::evictItem function.

For detailed specification you are encouraged to read the `NFD Developer's Guide <http://named-data.net/wp-content/uploads/2014/07/NFD-developer-guide.pdf>`_, section 3.2.
