.. _forwarding strategies:

Forwarding Strategies
=====================

ndnSIM through its integration with NFD provides simple ways to experiment with the custom
Interest/Data forwarding strategies of NFD.

A new forwarding strategy can be implement completely different processing or override just specific actions/events of the :nfd:`forwarding strategy interface <ForwardingStrategy>`.
NFD offers the maximum flexibility by allowing per-namespace selection of each specific
forwarding strategy.

Please refer to :nfd:`API documentation <ForwardingStrategy>` of the forwarding strategy interface, which lists all default actions/events. For a more detailed specification, you can
read `NFD Developer's Guide <http://named-data.net/wp-content/uploads/2014/07/NFD-developer-guide.pdf>`_, section 5.

It should be noted here that each custom forwarding strategy file has to be added under the
daemon/fw/ directory of NFD and also be listed in the available-strategies source file.
Moreover, the strategy choice helper has to be installed in the nodes in order to choose
the desirable default or custom forwarding strategy.


Available forwarding strategies
+++++++++++++++++++++++++++++++

Basic forwarding strategies
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Broadcast
#########

The broadcast strategy forwards every Interest to all upstreams, indicated by the supplied
FIB entry.

Implementation name: :nfd:`nfd::BroadcastStrategy` (default)

Usage example:

      .. code-block:: c++

         StrategyChoiceHelper strategyChoiceHelper;
         ...
         strategyChoiceHelper.Install (nodes, prefix,
                                   "/localhost/nfd/strategy/broadcast");

Client Control Strategy
#######################

The client control strategy allows a local consumer application to choose the outgoing face
of each Interest.

Implementation name :nfd:`nfd::ClientControlStrategy`

Usage example:

      .. code-block:: c++

         StrategyChoiceHelper strategyChoiceHelper;
         ...
         strategyChoiceHelper.Install (nodes, prefix,
                                   "/localhost/nfd/strategy/client-control");

Best Route Strategy
###################

The best route strategy forwards an Interest to the upstream with lowest routing cost.

Implementation name: :nfd:`nfd::BestRouteStrategy2`

Usage example:

      .. code-block:: c++

         StrategyChoiceHelper strategyChoiceHelper;
         ...
         strategyChoiceHelper.Install (nodes, prefix,
                                   "/localhost/nfd/strategy/best-route");

NCC Strategy
############

The NCC strategy is an reimplementation of CCNx 0.7.2 default strategy. It has similar
algorithm but is not guaranteed to be equivalent.

Implementation name: :nfd:`nfd::NccStrategy`

Usage example:

      .. code-block:: c++

         StrategyChoiceHelper strategyChoiceHelper;
         ...
         strategyChoiceHelper.Install (nodes, prefix,
                                   "/localhost/nfd/strategy/ncc");

.. _Writing your own custom strategy:

Writing your own custom strategy
++++++++++++++++++++++++++++++++

The initial step in creating a new strategy is to create a class, say MyStrategy that is
derived from nfd::Strategy. This subclass must at least override the triggers that are
marked pure virtual and may override other available triggers that are marked just virtual.

If the strategy needs to store information, it is needed to decide whether the information
is related to a namespace or an Interest. Information related to a namespace but not
specific to an Interest should be stored in Measurements entries; information related to an
Interest should be stored in PIT entries, PIT downstream records, or PIT upstream records.
After this decision is made, a data structure derived from StrategyInfo class needs to be
declared. In the existing implementation, such data structures are declared as nested
classes as it provides natural grouping and scope protection of the strategy-specific
entity, but it is not required to follow the same model. If timers are needed, EventId
fields needs to be added to such data structure(s).

The final step is to implement one or more of the triggers with the desired strategy logic. These triggers are listed below:

After Receive Interest Trigger
++++++++++++++++++++++++++++++

This trigger is declared as Strategy::afterReceiveInterest method. This method is pure
virtual, which must be overridden by a subclass.

When an Interest is received, passes necessary checks, and needs to be forwarded, Incoming
Interest pipeline (Section 4.2.1) invokes this trigger with the PIT entry, incoming Interest
packet, and FIB entry. At that time, the following conditions hold for the Interest:

• The Interest does not violate /localhost scope.
• The Interest is not looped.
• The Interest cannot be satisfied by ContentStore.
• The Interest is under a namespace managed by this strategy.

After being triggered, the strategy should decide whether and where to forward this
Interest. If the strategy decides to forward this Interest, it should invoke send Interest
action at least once. If the strategy concludes that this Interest cannot be forwarded, it should invoke reject pending Interest action, so that the PIT entry will be deleted shortly.

Before Satisfy Interest Trigger
+++++++++++++++++++++++++++++++

This trigger is declared as Strategy::beforeSatisfyPendingInterest method. The base class
provides a default implementation that does nothing; a subclass can override this method if
the strategy needs to be invoked for this trigger, e.g., to record data plane measurement
results for the pending Interest.

Before Expire Interest Trigger
++++++++++++++++++++++++++++++

This trigger is declared as Strategy::beforeExpirePendingInterest method. The base class
provides a default imple- mentation that does nothing; a subclass can override this method
if the strategy needs to be invoked for this trigger, e.g., to record data plane measurement
results for the pending Interest.

In this way, the recommended way to implement a custom forwarding strategy is to create a
new class for your strategy which would inherit the Strategy class of NFD :nfd:`nfd:Strategy`.

In an essence, this class can constitute the template for writing a custom forwarding
strategy:

.. literalinclude:: ../../NFD/daemon/fw/strategy.hpp
   :language: c++
   :linenos:
   :lines: 56-110

.. literalinclude:: ../../NFD/daemon/fw/strategy.cpp
   :language: c++
   :linenos:
   :lines: 35-41,47-59
   :emphasize-lines: 47,48,49,55,56

After having the template, we can fill the necessary functionality.

The following script implements a random load balancing forwarding strategy. This strategy
was created by Steve DiBenedetto and was found in one of his `git repositories
<https://github.com/dibenede/ndn-tutorial-gec21>`_.

.. literalinclude:: ../../NFD/daemon/fw/random-load-balancer-strategy.cpp
   :language: c++
   :linenos:
   :lines: 66-102

This forwarding strategy can be enabled for a specific name prefix when developing the
simulation scenario as follows:

      .. code-block:: c++

         StrategyChoiceHelper strategyChoiceHelper;
         ...
         strategyChoiceHelper.Install (nodes, prefix,
                                   "/localhost/nfd/strategy/random-load-balancer");

Example of using custom strategy
++++++++++++++++++++++++++++++++

Please refer to :ref:`this example <6-node topology with custom NFD forwarding strategy>`.
