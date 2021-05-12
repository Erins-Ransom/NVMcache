config="-F /home/cs261/NVMcache/c_memcached_interface/memaslap.cnf"

threads=1
warmup="30s"
benchmark_time="60s"

memaslap="/home/cs261/NVMcache/libmemcached-1.0.18/clients/memaslap"

memcached_1_4_3="/home/cs261/NVMcache/memcached-1.4.34"
memcached_1_6_9='/home/cs261/NVMcache/memcached-1.6.9'
memcached_mod="/home/cs261/NVMcache/memcached-mod"

# ${memcached_1_6_9}/memcached
declare -a memcaches=("${memcached_1_4_3}/memcached-base"
		      "${memcached_1_4_3}/memcached-busy"
                      "${memcached_1_4_3}/memcached-clflush"
		      "${memcached_mod}/memcached-base"
		      "${memcached_mod}/memcached-busy"
		      "${memcached_mod}/memcached-clflush")

for memcached in "${memcaches[@]}"; do
	echo $memcached
	$memcached -p 11211 -u 11211 -m 4096 -d 
	sleep 3
	if pgrep -x $(basename $memcached) | head -c 14 >/dev/null
	then
	    echo "running"
	else
	    echo "stopped"
	    sleep 3
	    ps -ef | grep 'memcached' | grep -v grep | awk '{print $2}' | xargs -r kill -9
            $memcached -p 11211 -u 11211 -m 4096 -d 
	    sleep 3
	fi

	# Warmup
	printf "Warmup\n"
	$memaslap -s 127.0.0.1:11211 -t $warmup --threads=$threads -B $config  | tail -1

	printf "Benchmark\n"
	$memaslap -s 127.0.0.1:11211 -t $benchmark_time --threads=$threads -B $config | tail -1

	# Kill Server
	#echo $(basename $memcached)
	killall $(basename $memcached)
	ps -ef | grep 'memcached' | grep -v grep | awk '{print $2}' | xargs -r kill -9

	printf "\n---------------------------------------\n"
	sleep 3

done
