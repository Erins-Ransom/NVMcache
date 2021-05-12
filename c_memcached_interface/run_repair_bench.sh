memcached=../memcached-mod/memcached-base
config="-F /home/cs261/NVMcache/c_memcached_interface/memaslap.cnf"
threads=1

# number of items = size * 5461 (for item size 100)
for size in 64 128 256 512 1024 
do
	echo $memcached -p 11211 -m $size -d
	$memcached -p 11211 -m $size -d
	sleep 3
        if pgrep -x $(basename $memcached) | head -c 14 >/dev/null
        then
            echo "running"
        else
            echo "stopped"
            sleep 3
            ps -ef | grep 'memcached' | grep -v grep | awk '{print $2}' | xargs -r kill -9
            $memcached -p 11211 -m $size -d 
            sleep 3
	fi

	/home/cs261/NVMcache/libmemcached-1.0.18/clients/memaslap -s 127.0.0.1:11211 -t 120s --threads=$threads -B $config
	sleep 3

	./send_repair.expect

	killall memcached-base
	sleep 10
done
