cp ./SConstruct_release ./SConstruct
cp ./common_secured.h ./common.h
scons TARGET_OS=linux TARGET_ARCH=x86_64 TARGET_TRANSPORT=IP RELEASE=1 SECURED=1
