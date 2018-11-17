from distutils.core import setup, Extension

module1 = Extension('qcircuit',
                    include_dirs = [],
                    libraries = ['pthread'],
                    sources = ['qcircuitmodule.cpp', 'circuitList.cpp'])
                    
setup ( name = 'qcircuit',
        version = '4',
        description = 'A c++ extension for optimizing quantum circuits',
        author = 'William Simon',
        url = 'www.william-simon.com',
        ext_modules = [module1])