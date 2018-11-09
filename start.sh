cp ./oic_svr_db_server_justworks.dat ./server.dat
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../iotivity-1.3.1/out/linux/x86_64/release/ ./server
