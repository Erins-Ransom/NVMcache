memcached=$1;

echo "Memcached: $memcached";
$memcached -p 11211 -d

./memc_basic

killall memcached
