ndnSIM helpers
==============

Helpers are very important components of ndnSIM, especially for writing simulation scenarios.
The following summarizes helpers and their basic usage.

StackHelper
---------------

:ndnsim:`StackHelper` is used to install ndnSIM network stack on requested nodes, as well to provide a simple way configure several important parameters of NDN simulation.

Example:

.. code-block:: c++

        StackHelper ndnHelper;
        NodeContainer nodes;
        ...
        ndnHelper.Install (nodes);

Routing
+++++++

All forwarding strategies require knowledge of where Interests can be forwarded (Forwarding Information Base).

.. note::
   By default, all nodes have empty FIB.  You need either to manually configure routes, use global routing controller, or (not recommended) enable default routes.

Manually routes
^^^^^^^^^^^^^^^

Routes can be configured manually using :ndnsim:`FibHelper::AddRoute` static methods of :ndnsim:`FibHelper`:

  .. code-block:: c++

     FibHelper fibHelper;
     ...
     Ptr<Node> node = ...     // some node
     std::string prefix = ... // some prefix
     Ptr<ndn::Face> face = ... // NDN face that belongs to the node and through which prefix is accessible
     int32_t metric = ...     // some routing metric
     FibHelper::AddRoute (node, prefix, face, metric);

Global routing controller
^^^^^^^^^^^^^^^^^^^^^^^^^

To simplify FIB management in large topologies, ndnSIM contains a global routing controller (:ndnsim:`helper <GlobalRoutingHelper>` and :ndnsim:`special interface <GlobalRouter>`), similar in spirit to ``Ipv4GlobalRoutingHelper``.

There are several necessary steps, in order to take advantage of the global routing controller:

* install :ndnsim:`special interfaces <GlobalRouter>` on nodes

   .. code-block:: c++

     NodeContainer nodes;
     ...
     GlobalRoutingHelper ndnGlobalRoutingHelper;
     ndnGlobalRoutingHelper.Install (nodes);

* specify which node exports which prefix using :ndnsim:`GlobalRoutingHelper::AddOrigins`

   .. code-block:: c++

     Ptr<Node> producer; // producer node that exports prefix
     std::string prefix; // exported prefix
     ...
     ndnGlobalRoutingHelper.AddOrigins (prefix, producer);

* calculate and install FIBs on every node using :ndnsim:`GlobalRoutingHelper::CalculateRoutes`

   .. code-block:: c++

     cdnGlobalRoutingHelper.CalculateRoutes ();

Default routes
^^^^^^^^^^^^^^

In simple topologies, like in :doc:`examples <examples>`, or when
simulating broadcast environment, it is possible to set up *default*
FIB entries using :ndnsim:`StackHelper::SetDefaultRoutes` call.
More specifically, every installed NDN stack will have a FIB entry to ``/`` prefix, containing all available faces.

The following should be done before installing stack on a node:

  .. code-block:: c++

     ndnHelper.SetDefaultRoutes (true);
     ...
     ndnHelper.Install (nodes);


ndnSIM's Content Store
++++++++++++++++++++++

ndnSIM's Contest Store comes with several different in-memory :ndnsim:`content store <ndn::ContentStore>` implementations, featuring different cache replacement policies.

To select a particular content store and configure its capacity, use
:ndnsim:`SetContentStore <ndn::StackHelper::SetContentStore>` helper method, after that you
have specified that you want to use ndnSIM's Content Store and not NFD's.

      .. code-block:: c++

         ndnHelper.SetContentStoreChoice (false);
         ndnHelper.SetContentStore ("<content store implementation>",
                                    ["<optional parameter>", "<optional parameter's value>" [, ...]]);
	 ...
	 ndnHelper.Install (nodes);

In simulation scenarios it is possible to select one of :ref:`the existing implementations of the content store or implement your own <content store>`.

FIB helper
----------

The :ndnsim:`FIB helper <FibHelper>` interacts with the FIB manager of NFD by sending
special Interest commands to the manager in order to add/remove a next hop from FIB entries
or add routes to the FIB manually (manual configuration of FIB). Examples of its usage are shown below:

Adding a next hop to a FIB entry (if any) that matches a given name prefix for a topology node:

    .. code-block:: c++

       FibHelper fibHelper;
       ...
       Ptr<Node> node = // Get the desired node
       nfd::ControlParameters parameters;
       parameters.setName(prefix);
       fibHelper.AddNextHop(parameters, node);

Removing a next hop from a FIB entry (if any) that matches a given name prefix for a topology node:

    .. code-block:: c++

       FibHelper fibHelper;
       ...
       Ptr<Node> node = // Get the desired node
       nfd::ControlParameters parameters;
       parameters.setName(prefix);
       fibHelper.RemoveNextHop(parameters, node);

Adding a route to the FIB manually (as mentioned above as well):

    .. code-block:: c++

       FibHelper fibHelper;
       ...
       Ptr<Node> node = ...     // some node
       std::string prefix = ... // some prefix
       Ptr<ndn::Face> face = ... // NDN face that belongs to the node and through which prefix is accessible
       int32_t metric = ...     // some routing metric
       FibHelper::AddRoute (node, prefix, face, metric);

Strategy Choice helper
----------------------

The :ndnsim:`Strategy Choice helper <StrategyChoiceHelper>` interacts with the Strategy
Choice manager of NFD by sending special Interest commands to the manager in order to
specify the desired per-name prefix forwarding strategy for one, more or all the nodes of a topology.

This helper should be used as follows:

    .. code-block:: c++

       StrategyChoiceHelper strategyChoiceHelper;
       ...
       strategyChoiceHelper.Install (nodes, prefix, strategyName);

or (for a forwarding strategy to be installed in all the topology nodes):

    .. code-block:: c++

       StrategyChoiceHelper strategyChoiceHelper;
       ...
       strategyChoiceHelper.InstallAll (prefix, strategyName);

AppHelper
---------

:ndnsim:`AppHelper` simplifies task of creating, configuring, and installing ndnSIM applications.


The basic usage of the :ndnsim:`AppHelper`:

* Create helper for specific applications class:

   .. code-block:: c++

      // Create helper for the consumer generating Interests with constant rate
      AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");

* Assign prefix on which application operates (either generating Interests using this name or satisfying Interests for this name) using :ndnsim:`AppHelper::SetPrefix`:

   .. code-block:: c++

      consumerHelper.SetPrefix (prefix);

* Assign application-specific attributes using :ndnsim:`AppHelper::SetAttribute`:

   .. code-block:: c++

      // Set frequency parameter
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second

* Install application on one or more nodes:

   .. code-block:: c++

      NodeContainer nodes;
      ...
      consumerHelper.Install (nodes)


In simulation scenarios it is possible to select one of :ref:`the existing applications or implement your own <applications>`.
