from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers


import pybindgen.settings
import warnings

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True
pybindgen.settings.error_handler = ErrorHandler()


import sys

def module_init():
    root_module = Module('ns.ndnSIM', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
        
    nested_module = module.add_cpp_namespace('ndn')
    register_types_ns3_ndn(nested_module)    

def register_types_ns3_ndn(module):
    root_module = module.get_root()
            
    ## Register a nested module for the namespace name
    
    nested_module = module.add_cpp_namespace('name')
    register_types_ns3_ndn_name(nested_module)

def register_types_ns3_ndn_name(module):
    root_module = module.get_root()
    
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component [class]
    module.add_class('Component')

    
def register_methods(root_module):
    # register_Ns3NdnName_methods(root_module, root_module['ns3::ndn::Name'])
    # register_Ns3NdnNameComponent_methods(root_module, root_module['ns3::ndn::name::Component'])
    return

def register_Ns3NdnName_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('+', root_module['ns3::ndn::Name'], root_module['ns3::ndn::Name'], param('ns3::ndn::Name const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name() [constructor]
    cls.add_constructor([])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name(ns3::ndn::Name const & other) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Name const &', 'other')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name(std::string const & url) [constructor]
    cls.add_constructor([param('std::string const &', 'url')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name(__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > begin, __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > end) [constructor]
    cls.add_constructor([param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'begin'), param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'end')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(ns3::ndn::name::Component const & comp) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('ns3::ndn::name::Component const &', 'comp')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > begin, __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > end) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'begin'), param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'end')], 
                   template_parameters=['__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > >'])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(ns3::ndn::Name const & comp) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('ns3::ndn::Name const &', 'comp')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(std::string const & compStr) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('std::string const &', 'compStr')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(void const * buf, size_t size) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('void const *', 'buf'), param('size_t', 'size')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendBlkId(uint64_t blkid) [member function]
    cls.add_method('appendBlkId', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'blkid')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendBySwap(ns3::ndn::name::Component & comp) [member function]
    cls.add_method('appendBySwap', 
                   'ns3::ndn::Name &', 
                   [param('ns3::ndn::name::Component &', 'comp')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendControlNum(uint64_t control) [member function]
    cls.add_method('appendControlNum', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'control')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendNumber(uint64_t number) [member function]
    cls.add_method('appendNumber', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'number')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendNumberWithMarker(uint64_t number, unsigned char marker) [member function]
    cls.add_method('appendNumberWithMarker', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'number'), param('unsigned char', 'marker')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendSeqNum(uint64_t seqno) [member function]
    cls.add_method('appendSeqNum', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'seqno')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendVersion(uint64_t version=ns3::ndn::Name::nversion) [member function]
    cls.add_method('appendVersion', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'version', default_value='ns3::ndn::Name::nversion')])
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::begin() const [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::begin() [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > >', 
                   [])
    ## name.h (module 'ndnSIM'): int ns3::ndn::Name::compare(ns3::ndn::Name const & name) const [member function]
    cls.add_method('compare', 
                   'int', 
                   [param('ns3::ndn::Name const &', 'name')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::end() const [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::end() [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > >', 
                   [])
    ## name.h (module 'ndnSIM'): ns3::ndn::name::Component const & ns3::ndn::Name::get(int index) const [member function]
    cls.add_method('get', 
                   'ns3::ndn::name::Component const &', 
                   [param('int', 'index')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::Name::get(int index) [member function]
    cls.add_method('get', 
                   'ns3::ndn::name::Component &', 
                   [param('int', 'index')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::Name::getPostfix(size_t len, size_t skip=0) const [member function]
    cls.add_method('getPostfix', 
                   'ns3::ndn::Name', 
                   [param('size_t', 'len'), param('size_t', 'skip', default_value='0')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::Name::getPrefix(size_t len, size_t skip=0) const [member function]
    cls.add_method('getPrefix', 
                   'ns3::ndn::Name', 
                   [param('size_t', 'len'), param('size_t', 'skip', default_value='0')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::Name::getSubName(size_t pos=0, size_t len=ns3::ndn::Name::npos) const [member function]
    cls.add_method('getSubName', 
                   'ns3::ndn::Name', 
                   [param('size_t', 'pos', default_value='0'), param('size_t', 'len', default_value='ns3::ndn::Name::npos')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rbegin() const [member function]
    cls.add_method('rbegin', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rbegin() [member function]
    cls.add_method('rbegin', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > > >', 
                   [])
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rend() const [member function]
    cls.add_method('rend', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rend() [member function]
    cls.add_method('rend', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > > >', 
                   [])
    ## name.h (module 'ndnSIM'): size_t ns3::ndn::Name::size() const [member function]
    cls.add_method('size', 
                   'size_t', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::string ns3::ndn::Name::toUri() const [member function]
    cls.add_method('toUri', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): void ns3::ndn::Name::toUri(std::ostream & os) const [member function]
    cls.add_method('toUri', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3NdnStackHelper_methods(root_module, cls):
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::ndn::StackHelper::StackHelper() [constructor]
    cls.add_constructor([])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetStackAttributes(std::string const & attr1="", std::string const & value1="", std::string const & attr2="", std::string const & value2="", std::string const & attr3="", std::string const & value3="", std::string const & attr4="", std::string const & value4="") [member function]
    cls.add_method('SetStackAttributes', 
                   'void', 
                   [param('std::string const &', 'attr1', default_value='""'), param('std::string const &', 'value1', default_value='""'), param('std::string const &', 'attr2', default_value='""'), param('std::string const &', 'value2', default_value='""'), param('std::string const &', 'attr3', default_value='""'), param('std::string const &', 'value3', default_value='""'), param('std::string const &', 'attr4', default_value='""'), param('std::string const &', 'value4', default_value='""')])
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::Install(std::string const & nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [param('std::string const &', 'nodeName')], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::Install(ns3::NodeContainer const & c) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [param('ns3::NodeContainer const &', 'c')], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::InstallAll() const [member function]
    cls.add_method('InstallAll', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [], 
                   is_const=True)
    return


def register_Ns3NdnNameComponent_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(ns3::ndn::name::Component const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::name::Component const &', 'arg0')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component() [constructor]
    cls.add_constructor([])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(std::string const & uri) [constructor]
    cls.add_constructor([param('std::string const &', 'uri')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(__gnu_cxx::__normal_iterator<char const*, std::string> begin, __gnu_cxx::__normal_iterator<char const*, std::string> end) [constructor]
    cls.add_constructor([param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'begin'), param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'end')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(void const * buf, size_t length) [constructor]
    cls.add_constructor([param('void const *', 'buf'), param('size_t', 'length')])
    ## name-component.h (module 'ndnSIM'): int ns3::ndn::name::Component::compare(ns3::ndn::name::Component const & other) const [member function]
    cls.add_method('compare', 
                   'int', 
                   [param('ns3::ndn::name::Component const &', 'other')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromNumber(uint64_t number) [member function]
    cls.add_method('fromNumber', 
                   'ns3::ndn::name::Component &', 
                   [param('uint64_t', 'number')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromNumberWithMarker(uint64_t number, unsigned char marker) [member function]
    cls.add_method('fromNumberWithMarker', 
                   'ns3::ndn::name::Component &', 
                   [param('uint64_t', 'number'), param('unsigned char', 'marker')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromUri(std::string const & uri) [member function]
    cls.add_method('fromUri', 
                   'ns3::ndn::name::Component &', 
                   [param('std::string const &', 'uri')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromUri(__gnu_cxx::__normal_iterator<char const*, std::string> begin, __gnu_cxx::__normal_iterator<char const*, std::string> end) [member function]
    cls.add_method('fromUri', 
                   'ns3::ndn::name::Component &', 
                   [param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'begin'), param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'end')])
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toBlkId() const [member function]
    cls.add_method('toBlkId', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): std::string ns3::ndn::name::Component::toBlob() const [member function]
    cls.add_method('toBlob', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): void ns3::ndn::name::Component::toBlob(std::ostream & os) const [member function]
    cls.add_method('toBlob', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toControlNum() const [member function]
    cls.add_method('toControlNum', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toNumber() const [member function]
    cls.add_method('toNumber', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toNumberWithMarker(unsigned char marker) const [member function]
    cls.add_method('toNumberWithMarker', 
                   'uint64_t', 
                   [param('unsigned char', 'marker')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toSeqNum() const [member function]
    cls.add_method('toSeqNum', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): std::string ns3::ndn::name::Component::toUri() const [member function]
    cls.add_method('toUri', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): void ns3::ndn::name::Component::toUri(std::ostream & os) const [member function]
    cls.add_method('toUri', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toVersion() const [member function]
    cls.add_method('toVersion', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    return


def main():
    out = FileCodeSink(sys.stdout)
    root_module = module_init()
    register_types(root_module)
    register_methods(root_module)
    # register_functions(root_module)
    root_module.generate(out)

if __name__ == '__main__':
    main()

    
