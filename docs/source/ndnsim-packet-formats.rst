.. _ndnSIM packet format:

ndnSIM packet format
====================

Tha packet format of this version of ndnSIM is the latest packet format supported by the
`ndn-cxx library <http://named-data.net/doc/ndn-tlv/>`_.

Interest Packet
---------------

Interest packet is TLV defined as follows:

Interest ::= INTEREST-TYPE TLV-LENGTH
               Name
               Selectors?
               Nonce
               Scope?
               InterestLifetime?

Name and Nonce are the only two required elements in an Interest packet. Selectors are
optional elements that further qualify Data that may match the Interest. They are used for
discovering and selecting the Data that matches best to what the application wants. Selectors
are placed right after the Name to facilitate implementations that may use continuous memory
block of Name and Selectors TLVs together as the index for PIT lookup. By using a TLV to
group all the Selectors, an implementation can easily skip them to find Nonce, which is used
together with Name to identify looping Interests. If Selectors TLV is present in the
Interest, it MUST contain at least one selector.

The two other optional elements, Scope and InterestLifetime, are referred to as Guiders. They
affect Interest forwarding behavior, e.g., how far the Interest may be forwarded, and how
long an Interest may be kept in the PIT. They are not grouped.

Data Packet
-----------

Data packet is TLV defined as follows:

Data ::= DATA-TLV TLV-LENGTH
           Name
           MetaInfo
           Content
           Signature

The Data packet represents some arbitrary binary data (held in the Content element) together
with its Name, some additional bits of information (MetaInfo), and a digital Signature of the
other three elements. The Name is the first element since all NDN packet processing starts
with the name. Signature is put at the end of the packet to ease the implementation because
signature computation covers all the elements before Signature.
