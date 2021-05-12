memcached=$1;

echo "Memcached: $memcached";
$memcached -p 11211 -u 11211 -m 4096 -d 

config="-F /home/cs261/NVMcache/c_memcached_interface/memaslap.cnf"
threads=1

# /home/cs261/NVMcache/libmemcached-1.0.18/clients/memaslap -s 127.0.0.1:11211 -t 30s  –facebook –division=50 --threads=1 -B $config

/home/cs261/NVMcache/libmemcached-1.0.18/clients/memaslap -s 127.0.0.1:11211 -t 30s --threads=$threads -B $config

/home/cs261/NVMcache/libmemcached-1.0.18/clients/memaslap -s 127.0.0.1:11211 -t 120s --threads=$threads -B $config

# running= basename $memcached
# killall $running
killall $(basename $memcached)
