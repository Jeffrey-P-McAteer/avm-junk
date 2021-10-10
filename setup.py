from distutils.core import setup, Extension

module1 = Extension('vm',
                    sources = ['vm.cpp'])

setup (name = 'vm',
       version = '1.0',
       description = 'This is a vm package',
       ext_modules = [module1])

