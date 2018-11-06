from distutils.core import setup, Extension

module1 = Extension('qcircuit',
                    include_dirs = [],
                    libraries = ['pthread'],
                    sources = ['qcircuitmodule.cpp', 'circuitList.cpp'])
                    
setup ( name = 'qcircuit',
        version = '2',
        description = 'A c++ extension for optimizing quantum circuits',
        author = 'William Simon',
        url = 'personalwebsite-william.herokuapp.com',
        ext_modules = [module1])
        
