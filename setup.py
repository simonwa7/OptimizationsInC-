from distutils.core import setup, Extension
import sysconfig

extra_compile_args = sysconfig.get_config_var('CFLAGS').split()
extra_compile_args += ["-std=c++11", "-Wall", "-Wextra"]

module1 = Extension('qcircuit',
                    include_dirs = [],
                    libraries = ['pthread'],
                    sources = ['qcircuitmodule.cpp', 'circuitList.cpp'],
                    extra_compile_args=extra_compile_args,
            		language='c++11'
                    )
                    
setup ( name = 'qcircuit',
        version = '2',
        description = 'A c++ extension for optimizing quantum circuits',
        author = 'William Simon',
        url = 'personalwebsite-william.herokuapp.com',
        ext_modules = [module1])
        
