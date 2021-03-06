VERSION = '1.1.0'
APPNAME = 'jubadump'

def options(opt):
  opt.load('compiler_cxx')
  opt.load('unittest_gtest')

def configure(conf):
  conf.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']
  conf.load('compiler_cxx')
  conf.load('unittest_gtest')

  conf.check_cfg(package = 'jubatus_core', args = '--cflags --libs')
  conf.check_cxx(lib = 'msgpack')

def build(bld):
  bld.recurse('src')
