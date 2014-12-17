.. ndnSIM: NS-3 based NDN simulator
.. ============================================================

============
Introduction
============

.. sidebar:: ndnSIM components

    .. image:: _static/ndnsim-components.*
        :width: 100%

The ndnSIM is a NS-3 module that implements Named Data Networking (NDN) communication model,
the clean slate Internet design. ndnSIM is specially optimized for simulation purposes and
has a clean and extensible internal structure.

Following the NDN architecture, ndnSIM is implemented as a new network-layer protocol model,
which can run on top of any available link-layer protocol model (point-to-point, CSMA,
wireless, etc.).

The current version of the simulator provides full integration with the Named Data Networking
Forwarding Daemon (NFD) using also the main features of the ndn-cxx library. The simulator is
implemented in a modular way and uses separate C++ classes to model the behavior of each
network-layer entity in NDN.

Specifically, the simulator fully interacts with NFD and its managers in order to use the
:nfd:`pending Interest table (PIT) <Pit>`, :nfd:`forwarding information base (FIB) <Fib>`,
:nfd:`content store <ContentStore>` and :nfd:`Interest forwarding strategies
<ForwardingStrategy>`. However, it provides the option to the user to choose between the
:nfd:`content store <ContentStore>` and the :ndnsim:`content store <ContentStore>` along
with its cache replacement policies of the previous simulator version.

This modular structure allows any component to be easily modified or replaced with no or
minimal impact on other components.
In addition, the simulator provides an extensive collection of interfaces and helpers to
perform detailed tracing behavior of every component, as well as NDN traffic flow.

The current version of the simulator uses the latest packet format, which is specified in
the ndn-cxx library.

More documentation
------------------

Overall structure of ndnSIM is described in our `technical report <http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ndnSIM-TR.pdf>`_.

`ndnSIM API documentation <doxygen/index.html>`_

Also, you can `join our mailing list
<http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_ to see and participate in
discussions about ndnSIM implementation and simulations in general.
Do not forget to check mailling list `archives <http://www.lists.cs.ucla.edu/pipermail/ndnsim/>`_.


Support
-------

The code of ndnSIM is in active development.  Bug reports (issues) as well as new feature implementation are always welcome.

To file a bug report, please use `GitHub Issues <https://github.com/NDN-Routing/ndnSIM/issues>`_.

To create new feature, please fork the code and submit Pull Request on GitHub.

And of course, `our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_ is the best way to communicate with and get support from ndnSIM team and other users of ndnSIM.

A very short guide to the code
------------------------------

All the NDN related code is in ``ns-3/src/ndnSIM``

+-----------------+---------------------------------------------------------------------+
| Folder          | Description                                                         |
+=================+=====================================================================+
| ``model/``      | implementation of NDN base: :ndnsim:`L3Protocol`, faces             |
|                 | (:ndnsim:`Face`, :ndnsim:`NetDeviceFace`, forwarding                |
|                 | :ndnsim:`AppFace`),                                                 |
|                 | etc.                                                                |
+-----------------+---------------------------------------------------------------------+
| ``apps/``       | applications (in NS-3 sense) that can be installed on the nodes.    |
|                 | Right now we have one producer (:ndnsim:`Producer`) and a           |
|                 | collection  of consumer (:ndnsim:`ConsumerCbr`,                     |
|                 | :ndnsim:`ConsumerWindow`,                                           |
|                 | :ndnsim:`ConsumerBatches`).  See doxygen documentation or           |
|                 | source  code for details                                            |
+-----------------+---------------------------------------------------------------------+
| ``helper/``     | a number of :doc:`useful helpers <helpers>`                         |
+-----------------+---------------------------------------------------------------------+
| ``NFD/``        | contains the NFD-0.2.0 code base with some modifications to make it |
|                 | more lightweight and compatible with the simulator                  |
+-----------------+---------------------------------------------------------------------+
| ``examples/``   | contain :doc:`several example scenarios <examples>`                 |
+-----------------+---------------------------------------------------------------------+
| ``utils/``      | helper classes, including implementation of generalized data        |
|                 | structures                                                          |
+-----------------+---------------------------------------------------------------------+
| ``plugins/``    | a number of plugins that may be helpful to run simulation scenarios |
+-----------------+---------------------------------------------------------------------+

Logging
-------

Almost every component in ndnSIM exports logging interface, so in debug compilation it is possible to track many internal details.
For example, logging of :ndnsim:`Face` and :ndnsim:`Consumer` shows everything what happens in :ndnsim:`Face` and :ndnsim:`Consumer` classes::

    NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

Refer to the source code and NS-3 documentation to see what logging interfaces are available and about details how enable one or more logging interfaces.
