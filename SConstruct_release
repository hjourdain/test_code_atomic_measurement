import os
SConscript('../iotivity-1.3.1/build_common/SConscript')
Import('env')
server_env = env.Clone()

server_env.AppendUnique(CPPPATH=[
    '../iotivity-1.3.1/resource/csdk/logger/include',
    '../iotivity-1.3.1/resource/csdk/include',
    '../iotivity-1.3.1/resource/csdk/stack/include',
    '../iotivity-1.3.1/resource/csdk/security/include',
    '../iotivity-1.3.1/resource/oc_logger/include',
    '../iotivity-1.3.1/resource/csdk/include',
    '../iotivity-1.3.1/resource/c_common',
    '../iotivity-1.3.1/resource/c_common/oic_malloc/include',
    '../iotivity-1.3.1/resource/c_common/oic_string/include',
    '../iotivity-1.3.1/out/linux/x86_64/release/include/c_common'
])

server_env.AppendUnique(LIBPATH=[
    '../iotivity-1.3.1/out/linux/x86_64/release'
])

server_env.AppendUnique(CXXFLAGS=['-std=c++0x', '-Wall', '-pthread'])
server_env.AppendUnique(LIBS=['pthread'])
server_env.Append(LINKFLAGS=['-Wl,--no-as-needed'])
server_env.PrependUnique(LIBS=['c_common'])
server_env.PrependUnique(LIBS=['logger'])
server_env.PrependUnique(LIBS=['octbstack'])
server_env.AppendUnique(LIBS=['mbedtls', 'mbedx509', 'mbedcrypto'])
server_env.AppendUnique(CPPDEFINES=['TB_LOG'])

# Build Blood Pressure Monitor
server = server_env.Program(
    'server', [
        'common.cpp', 

        'device/bloodpressure0.cpp',
        'device/bloodpressure1.cpp',
        'device/bloodpressure2.cpp',

        'server.cpp'
        ])
