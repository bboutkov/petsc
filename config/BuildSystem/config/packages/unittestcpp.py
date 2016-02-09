import config.package
import os

class Configure(config.package.GNUPackage):
  def __init__(self, framework):
    config.package.GNUPackage.__init__(self, framework)
    self.download         = ['https://github.com/unittest-cpp/unittest-cpp/archive/v1.5.0.tar.gz']
    self.functions        = []
    self.includes         = ['UnitTest++.h']
    self.liblist          = [['libUnitTest++.a']]
    self.cxx              = 1    
    self.downloaddirname  = 'unittest-cpp'
    self.includedir       = os.path.join('include','UnitTest++','UnitTest++')