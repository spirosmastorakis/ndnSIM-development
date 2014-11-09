## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#
# Copyright (c) 2011-2013, Regents of the University of California
#                          Alexander Afanasyev
#
# GNU 3.0 license, See the LICENSE file for more information
#
# Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
#
import os
from waflib import Logs, Utils, Options, TaskGen, Task
from waflib.Errors import WafError

import wutils

def options(opt):
    opt = opt.add_option_group ('ndnSIM Options')
    opt.add_option('--enable-ndn-plugins',
                   help="""Enable NDN plugins (may require patching).  topology plugin enabled by default""",
                   dest='enable_ndn_plugins')

    opt.add_option('--disable-ndn-plugins',
                   help="""Enable NDN plugins (may require patching).  topology plugin enabled by default""",
                   dest='disable_ndn_plugins')

    opt.add_option('--pyndn-install-path', dest='pyndn_install_path',
                   help="""Installation path for PyNDN (by default: into standard location under PyNDN folder""")

REQUIRED_BOOST_LIBS = ['graph']

def required_boost_libs(conf):
    conf.env.REQUIRED_BOOST_LIBS += REQUIRED_BOOST_LIBS

def configure(conf):
    conf.env['ENABLE_NDNSIM']=False;

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    ##conf.check_cxx(function_name='pcap_set_immediate_mode', header_name='pcap/pcap.h',
                       ##cxxflags='-Wno-error', use='LIBPCAP', mandatory=False)

    if not conf.env['LIB_BOOST']:
        conf.report_optional_feature("ndnSIM", "ndnSIM", False,
                                     "Required boost libraries not found")
        Logs.error ("ndnSIM will not be build as it requires boost libraries of version at least 1.48")
        conf.env['MODULES_NOT_BUILT'].append('ndnSIM')
        return
    else:
        present_boost_libs = []
        for boost_lib_name in conf.env['LIB_BOOST']:
            if boost_lib_name.startswith("boost_"):
                boost_lib_name = boost_lib_name[6:]
            if boost_lib_name.endswith("-mt"):
                boost_lib_name = boost_lib_name[:-3]
            present_boost_libs.append(boost_lib_name)

        missing_boost_libs = [lib for lib in REQUIRED_BOOST_LIBS if lib not in present_boost_libs]

        if missing_boost_libs != []:
            conf.report_optional_feature("ndnSIM", "ndnSIM", False,
                                         "ndnSIM requires boost libraries: %s" % ' '.join(missing_boost_libs))
            conf.env['MODULES_NOT_BUILT'].append('ndnSIM')

            Logs.error ("ndnSIM will not be build as it requires boost libraries: %s" % ' '.join(missing_boost_libs))
            Logs.error ("Please upgrade your distribution or install custom boost libraries (http://ndnsim.net/faq.html#boost-libraries)")
            return

        boost_version = conf.env.BOOST_VERSION.split('_')
        if int(boost_version[0]) < 1 or int(boost_version[1]) < 48:
            conf.report_optional_feature("ndnSIM", "ndnSIM", False,
                                         "ndnSIM requires at least boost version 1.48")
            conf.env['MODULES_NOT_BUILT'].append('ndnSIM')

            Logs.error ("ndnSIM will not be build as it requires boost libraries of version at least 1.48")
            Logs.error ("Please upgrade your distribution or install custom boost libraries (http://ndnsim.net/faq.html#boost-libraries)")
            return

    conf.env['NDN_plugins'] = ['topology', 'ip-faces']
    if Options.options.enable_ndn_plugins:
        conf.env['NDN_plugins'] = conf.env['NDN_plugins'] + Options.options.enable_ndn_plugins.split(',')

    if Options.options.disable_ndn_plugins:
        conf.env['NDN_plugins'] = conf.env['NDN_plugins'] - Options.options.disable_ndn_plugins.split(',')

    if Options.options.pyndn_install_path:
        conf.env['PyNDN_install_path'] = Options.options.pyndn_install_path

    conf.env['ENABLE_NDNSIM']=True;
    conf.env['MODULES_BUILT'].append('ndnSIM')

    conf.report_optional_feature("ndnSIM", "ndnSIM", True, "")

def build(bld):
    deps = ['core', 'network', 'point-to-point']
    deps.append ('internet') # Until RttEstimator is moved to network module
    if 'ns3-visualizer' in bld.env['NS3_ENABLED_MODULES']:
        deps.append ('visualizer')

    if 'topology' in bld.env['NDN_plugins']:
        deps.append ('topology-read')
        deps.append ('mobility')

    if 'mobility' in bld.env['NDN_plugins']:
        deps.append ('mobility')

    ##if bld.env['HAVE_LIBPCAP']:
        ##nfd_objects.source += bld.path.ant_glob('daemon/face/ethernet-*.cpp')
        ##nfd_objects.use += ' LIBPCAP'

    module = bld.create_ns3_module ('ndnSIM', deps)
    module.module = 'ndnSIM'
    module.features += ' ns3fullmoduleheaders'
    module.uselib = 'NDN_CXX BOOST BOOST_IOSTREAMS'

    headers = bld (features='ns3header')
    headers.module = 'ndnSIM'

    if not bld.env['ENABLE_NDNSIM']:
        bld.env['MODULES_NOT_BUILT'].append('ndnSIM')
        return

    module.source = bld.path.ant_glob(['model/**/*.cc',
                                       'apps/*.cc',
                                       'utils/**/*.cc',
                                       'helper/**/*.cc',
                                       'helper/**/*.cpp',
                                       'model/**/*.cpp',
                                       'NFD/**/*.cpp',
                                       'ndn-cxx/src/**/*.cpp'
                                       ])
    module.full_headers = [p.path_from(bld.path) for p in bld.path.ant_glob([
                           'utils/**/*.h',
                           'model/**/*.h',
                           'apps/**/*.h',
                           'helper/**/*.h',
                           'helper/**/*.hpp',
                           'model/**/*.hpp',
                           'NFD/**/*.hpp',
                           'ndn-cxx/src/**/*.hpp'
                           ])]

    headers.source = [

        "helper/ndn-stack-helper.h",
        "helper/ndn-app-helper.h",
        "helper/ndn-face-container.h",
        "helper/ndn-global-routing-helper.h",
        "helper/ndn-link-control-helper.h",
        "helper/ndn-header-helper.h",
        "apps/ndn-app.h",
        "apps/callback-based-app.h",

        "model/ndn-common.h",
        "model/ndn-forwarder.h",
        "model/ndn-face.h",
        "model/ndn-app-face.h",
        "model/ndn-net-device-face.h",

#         "model/cs/ndn-content-store.h",

#         "model/fib/ndn-fib.h",
#         "model/fib/ndn-fib-entry.h",

#         "model/pit/ndn-pit.h",
#         "model/pit/ndn-pit-entry.h",
#         "model/pit/ndn-pit-entry-incoming-face.h",
#         "model/pit/ndn-pit-entry-outgoing-face.h",

        # "model/fw/ndn-forwarding-strategy.h",
        # "model/fw/ndn-fw-tag.h",

        "utils/ndn-limits.h",
        "utils/ndn-rtt-estimator.h",

        "NFD/common.hpp",

        "NFD/daemon/table/measurements-accessor.hpp",
        "NFD/daemon/table/measurements.hpp",
        "NFD/daemon/table/measurements-entry.hpp",
        "NFD/daemon/table/strategy-info-host.hpp",
        "NFD/daemon/table/cs-entry.hpp",
        "NFD/daemon/table/cs.hpp",
        "NFD/daemon/table/dead-nonce-list.hpp",
        "NFD/daemon/table/fib-entry.hpp",
        "NFD/daemon/table/fib-nexthop.hpp",
        "NFD/daemon/table/fib.hpp",
        "NFD/daemon/table/name-tree-entry.hpp",
        "NFD/daemon/table/name-tree.hpp",
        "NFD/daemon/table/pit-entry.hpp",
        "NFD/daemon/table/pit-face-record.hpp",
        "NFD/daemon/table/pit-in-record.hpp",
        "NFD/daemon/table/pit-out-record.hpp",
        "NFD/daemon/table/pit.hpp",
        "NFD/daemon/table/strategy-choice-entry.hpp",
        "NFD/daemon/table/strategy-choice.hpp",
        "NFD/daemon/table/strategy-info-host.hpp",

        "NFD/core/city-hash.hpp",
        "NFD/core/config-file.hpp",
        "NFD/core/face-uri.hpp",
        "NFD/core/global-io.hpp",
        "NFD/core/logger-factory.hpp",
        "NFD/core/logger.hpp",
        "NFD/core/map-value-iterator.hpp",
        "NFD/core/random.hpp",
        "NFD/core/scheduler.hpp",
        "NFD/core/ethernet.hpp",
        "NFD/core/network-interface.hpp",
        "NFD/core/notification-stream.hpp",
        "NFD/core/privilege-helper.hpp",
        "NFD/core/resolver.hpp",
        "NFD/core/segment-publisher.hpp",

        "NFD/daemon/face/face-counters.hpp",
        "NFD/daemon/face/channel.hpp",
        "NFD/daemon/face/protocol-factory.hpp",
        "NFD/daemon/face/local-face.hpp",
        "NFD/daemon/face/stream-face.hpp",
        "NFD/daemon/face/tcp-channel.hpp",
        "NFD/daemon/face/tcp-face.hpp",
        "NFD/daemon/face/tcp-factory.hpp",
        "NFD/daemon/face/udp-channel.hpp",
        "NFD/daemon/face/udp-factory.hpp",
        "NFD/daemon/face/multicast-udp-face.hpp",
        "NFD/daemon/face/datagram-face.hpp",
        "NFD/daemon/face/udp-face.hpp",
        "NFD/daemon/face/ndnlp-tlv.hpp",
        "NFD/daemon/face/ndnlp-parse.hpp",
        "NFD/daemon/face/ndnlp-partial-message-store.hpp",
        "NFD/daemon/face/ndnlp-sequence-generator.hpp",
        "NFD/daemon/face/ndnlp-slicer.hpp",
        "NFD/daemon/face/null-face.hpp",
        #"NFD/daemon/face/ethernet-face.hpp",
        #"NFD/daemon/face/ethernet-factory.hpp",

        "NFD/daemon/fw/face-table.hpp",
        "NFD/daemon/fw/forwarder-counters.hpp",
        "NFD/daemon/fw/forwarder.hpp",
        "NFD/daemon/fw/strategy-info.hpp",
        "NFD/daemon/fw/strategy.hpp",
        "NFD/daemon/fw/available-strategies.hpp",
        "NFD/daemon/fw/best-route-strategy.hpp",
        "NFD/daemon/fw/best-route-strategy2.hpp",
        "NFD/daemon/fw/broadcast-strategy.hpp",
        "NFD/daemon/fw/client-control-strategy.hpp",
        "NFD/daemon/fw/ncc-strategy.hpp",
        "NFD/daemon/fw/rtt-estimator.hpp",

        "NFD/daemon/mgmt/app-face.hpp",
        "NFD/daemon/mgmt/channel-status-publisher.hpp",
        "NFD/daemon/mgmt/command-validator.hpp",
        "NFD/daemon/mgmt/internal-face.hpp",
        "NFD/daemon/mgmt/manager-base.hpp",
        "NFD/daemon/mgmt/face-status-publisher.hpp",
        "NFD/daemon/mgmt/face-manager.hpp",
        "NFD/daemon/mgmt/fib-enumeration-publisher.hpp",
        "NFD/daemon/mgmt/fib-manager.hpp",
        "NFD/daemon/mgmt/general-config-section.hpp",
        "NFD/daemon/mgmt/status-server.hpp",
        "NFD/daemon/mgmt/strategy-choice-manager.hpp",
        "NFD/daemon/mgmt/strategy-choice-publisher.hpp",
        "NFD/daemon/mgmt/tables-config-section.hpp",

        "NFD/rib/face-entry.hpp",
        "NFD/rib/fib-update.hpp",
        "NFD/rib/rib-entry.hpp",
        "NFD/rib/rib-manager.hpp",
        "NFD/rib/rib-status-publisher.hpp",
        "NFD/rib/rib.hpp",

        "ndn-cxx/src/common-pch.hpp",
        "ndn-cxx/src/common.hpp",
        "ndn-cxx/src/data.hpp",
        "ndn-cxx/src/exclude.hpp",
        "ndn-cxx/src/face.hpp",
        "ndn-cxx/src/interest-filter.hpp",
        "ndn-cxx/src/interest.hpp",
        "ndn-cxx/src/key-locator.hpp",
        "ndn-cxx/src/meta-info.hpp",
        "ndn-cxx/src/name-component.hpp",
        "ndn-cxx/src/name.hpp",
        "ndn-cxx/src/selectors.hpp",
        "ndn-cxx/src/signature-info.hpp",
        "ndn-cxx/src/signature.hpp",

        "ndn-cxx/src/detail/face-impl.hpp",
        "ndn-cxx/src/detail/interest-filter-record.hpp",
        "ndn-cxx/src/detail/pending-interest.hpp",
        "ndn-cxx/src/detail/registered-prefix.hpp",

        "ndn-cxx/src/util/event-emitter.hpp",
        "ndn-cxx/src/util/ethernet.hpp",
        "ndn-cxx/src/util/time.hpp",
        "ndn-cxx/src/util/face-uri.hpp",
        "ndn-cxx/src/util/scheduler.hpp",
        "ndn-cxx/src/util/command-interest-validator.hpp",
        "ndn-cxx/src/util/concepts.hpp",
        "ndn-cxx/src/util/config-file.hpp",
        "ndn-cxx/src/util/crypto.hpp",
        "ndn-cxx/src/util/digest.hpp",
        "ndn-cxx/src/util/dns.hpp",
        "ndn-cxx/src/util/in-memory-storage-entry.hpp",
        "ndn-cxx/src/util/in-memory-storage-fifo.hpp",
        "ndn-cxx/src/util/in-memory-storage-lfu.hpp",
        "ndn-cxx/src/util/in-memory-storage-lru.hpp",
        "ndn-cxx/src/util/in-memory-storage-persistent.hpp",
        "ndn-cxx/src/util/in-memory-storage.hpp",
        "ndn-cxx/src/util/io.hpp",
        "ndn-cxx/src/util/monotonic_deadline_timer.hpp",
        "ndn-cxx/src/util/notification-stream.hpp",
        "ndn-cxx/src/util/notification-subscriber.hpp",
        "ndn-cxx/src/util/random.hpp",
        "ndn-cxx/src/util/regex.hpp",
        "ndn-cxx/src/util/segment-fetcher.hpp",
        "ndn-cxx/src/util/string-helper.hpp",

        "ndn-cxx/src/util/regex/regex-backref-manager.hpp",
        "ndn-cxx/src/util/regex/regex-backref-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-component-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-component-set-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-pattern-list-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-pseudo-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-repeat-matcher.hpp",
        "ndn-cxx/src/util/regex/regex-top-matcher.hpp",

        "ndn-cxx/src/management/nfd-control-parameters.hpp",
        "ndn-cxx/src/management/nfd-rib-entry.hpp",
        "ndn-cxx/src/management/nfd-controller.hpp",
        "ndn-cxx/src/management/nfd-control-command.hpp",
        "ndn-cxx/src/management/nfd-control-response.hpp",
        "ndn-cxx/src/management/nfd-face-monitor.hpp",
        "ndn-cxx/src/management/nfd-face-status.hpp",
        "ndn-cxx/src/management/nfd-forwarder-status.hpp",
        "ndn-cxx/src/management/nfd-strategy-choice.hpp",
        "ndn-cxx/src/management/nfd-channel-status.hpp",
        "ndn-cxx/src/management/nfd-command-options.hpp",
        "ndn-cxx/src/management/nfd-face-event-notification.hpp",
        "ndn-cxx/src/management/nfd-face-query-filter.hpp",
        "ndn-cxx/src/management/nfd-face-traits.hpp",
        "ndn-cxx/src/management/nfd-local-control-header.hpp",
        "ndn-cxx/src/management/nfd-rib-flags.hpp",

        "ndn-cxx/src/encoding/tlv.hpp",
        "ndn-cxx/src/encoding/encoding-buffer.hpp",
        "ndn-cxx/src/encoding/block-helpers.hpp",
        "ndn-cxx/src/encoding/block.hpp",
        "ndn-cxx/src/encoding/buffer-stream.hpp",
        "ndn-cxx/src/encoding/endian.hpp",
        "ndn-cxx/src/encoding/nfd-constants.hpp",
        "ndn-cxx/src/encoding/oid.hpp",
        "ndn-cxx/src/encoding/tlv-nfd.hpp",
        "ndn-cxx/src/encoding/tlv-security.hpp",
        "ndn-cxx/src/encoding/cryptopp/asn_ext.hpp",

        "ndn-cxx/src/security/key-chain.hpp",
        "ndn-cxx/src/security/validator-config.hpp",
        "ndn-cxx/src/security/certificate-cache-ttl.hpp",
        "ndn-cxx/src/security/certificate-cache.hpp",
        "ndn-cxx/src/security/certificate-extension.hpp",
        "ndn-cxx/src/security/certificate-subject-description.hpp",
        "ndn-cxx/src/security/certificate.hpp",
        "ndn-cxx/src/security/cryptopp.hpp",
        "ndn-cxx/src/security/digest-sha256.hpp",
        "ndn-cxx/src/security/encryption-manager.hpp",
        "ndn-cxx/src/security/identity-certificate.hpp",
        "ndn-cxx/src/security/key-chain.hpp",
        "ndn-cxx/src/security/key-params.hpp",
        "ndn-cxx/src/security/openssl.hpp",
        "ndn-cxx/src/security/public-key.hpp",
        "ndn-cxx/src/security/sec-public-info-sqlite3.hpp",
        "ndn-cxx/src/security/sec-public-info.hpp",
        "ndn-cxx/src/security/sec-rule-relative.hpp",
        "ndn-cxx/src/security/sec-rule-specific.hpp",
        "ndn-cxx/src/security/sec-rule.hpp",
        "ndn-cxx/src/security/sec-tpm-file.hpp",
        "ndn-cxx/src/security/sec-tpm-osx.hpp",
        "ndn-cxx/src/security/sec-tpm.hpp",
        "ndn-cxx/src/security/secured-bag.hpp",
        "ndn-cxx/src/security/security-common.hpp",
        "ndn-cxx/src/security/signature-sha256-with-ecdsa.hpp",
        "ndn-cxx/src/security/signature-sha256-with-rsa.hpp",
        "ndn-cxx/src/security/signature-sha256.hpp",
        "ndn-cxx/src/security/validation-request.hpp",
        "ndn-cxx/src/security/validator-config.hpp",
        "ndn-cxx/src/security/validator-null.hpp",
        "ndn-cxx/src/security/validator-regex.hpp",
        "ndn-cxx/src/security/validator.hpp",

        "ndn-cxx/src/security/conf/checker.hpp",
        "ndn-cxx/src/security/conf/common.hpp",
        "ndn-cxx/src/security/conf/filter.hpp",
        "ndn-cxx/src/security/conf/key-locator-checker.hpp",
        "ndn-cxx/src/security/conf/rule.hpp",

        "ndn-cxx/src/transport/stream-transport.hpp",
        "ndn-cxx/src/transport/tcp-transport.hpp",
        "ndn-cxx/src/transport/transport.hpp",
        "ndn-cxx/src/transport/unix-transport.hpp",

#         "utils/tracers/ipv4-app-tracer.h",
#         "utils/tracers/ipv4-l3-tracer.h",
#         "utils/tracers/ipv4-rate-l3-tracer.h",
#         "utils/tracers/ipv4-seqs-app-tracer.h",

#         "utils/tracers/l2-rate-tracer.h",
#         "utils/tracers/l2-tracer.h",
#         "utils/tracers/ndn-app-delay-tracer.h",
#         "utils/tracers/ndn-cs-tracer.h",
#         "utils/tracers/ndn-l3-aggregate-tracer.h",
#         "utils/tracers/ndn-l3-tracer.h",
#         "utils/tracers/ndn-l3-rate-tracer.h",
        ]

    if 'topology' in bld.env['NDN_plugins']:
        headers.source.extend ([
            "plugins/topology/rocketfuel-weights-reader.h",
            "plugins/topology/annotated-topology-reader.h",
            ])
        module.source.extend (bld.path.ant_glob(['plugins/topology/*.cc']))
        module.full_headers.extend ([p.path_from(bld.path) for p in bld.path.ant_glob(['plugins/topology/**/*.h'])])

    if 'mobility' in bld.env['NDN_plugins']:
        headers.source.extend ([
            "plugins/mobility/spring-mobility-model.h",
            "plugins/mobility/spring-mobility-helper.h",
            ])
        module.source.extend (bld.path.ant_glob(['plugins/mobility/*.cc']))
        module.full_headers.extend ([p.path_from(bld.path) for p in bld.path.ant_glob(['plugins/mobility/**/*.h'])])

    if 'ip-faces' in bld.env['NDN_plugins']:
        headers.source.extend ([
            "plugins/ip-faces/ndn-ip-faces-helper.h",
            ])
        module.source.extend (bld.path.ant_glob(['plugins/ip-faces/*.cc']))
        module.full_headers.extend ([p.path_from(bld.path) for p in bld.path.ant_glob(['plugins/ip-faces/**/*.h'])])

    # bld.install_files('${UDEDIR}/%s%s/ns3/ndnSIM' % (wutils.APPNAME, wutils.VERSION), ndnSIM_headers, relative_trick=True)
    # bld.install_files('$PREFIX/include', ndnSIM_headers)

    tests = bld.create_ns3_module_test_library('ndnSIM')
    tests.source = bld.path.ant_glob('test/*.cc')

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse ('examples')

    bld.recurse ('tools')

    #bld.ns3_python_bindings()

    #if bld.env['ENABLE_PYTHON_BINDINGS']:
        #if bld.env['PyNDN_install_path']:
            #bld (features = "py",
                 #source = bld.path.ant_glob (["PyNDN/**/*.py"]),
                 #install_from = "PyNDN",
                 #install_path = bld.env['PyNDN_install_path'])

@TaskGen.feature('ns3fullmoduleheaders')
@TaskGen.after_method('process_rule')
def apply_ns3fullmoduleheaders(self):
    # ## get all of the ns3 headers
    ns3_dir_node = self.bld.path.find_or_declare("ns3")

    mode = getattr(self, "mode", "install")

    for filename in set(self.to_list(self.full_headers)):
        src_node = self.path.find_resource(filename)
        if src_node is None:
            raise WafError("source ns3 header file %s not found" % (filename,))
        dst_node = ns3_dir_node.find_or_declare(src_node.path_from(self.bld.path.find_dir('src')))
        assert dst_node is not None

        relpath = src_node.parent.path_from(self.bld.path.find_dir('src'))

        task = self.create_task('ns3header')
        task.mode = getattr(self, 'mode', 'install')
        if task.mode == 'install':
            self.bld.install_files('${INCLUDEDIR}/%s%s/ns3/%s' % (wutils.APPNAME, wutils.VERSION, relpath),
                                   [src_node])
            task.set_inputs([src_node])
            task.set_outputs([dst_node])
        else:
            task.header_to_remove = dst_node
