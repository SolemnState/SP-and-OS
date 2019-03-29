#!/bin/bash
function count () {
    cat /dev/null > log.txt
    x=0
    SECONDS=0
    while [ $SECONDS -ne 60 ]; do
            sudo tcpdump -c 50 host $1 >> log.txt
            x=$(wc -l log.txt | awk '{print $1}')
            if [[ $x -eq 100 ]]; then
                 echo "Packet limit!"
                 sudo iptables -A INPUT -s $1 -j DROP
                 ((tts = 60 - $SECONDS))
                 echo "Waiting for $tts seconds"
                 sleep $tts
                 sudo iptables -D INPUT -s $1 -j DROP
                 return 
            fi    
    done
    echo 'time is over'
    echo "number of captured packets: $x"
    return
}

cleanup () { 
    kill -s SIGTERM $! 
    exit 0 
}

if [ $# -eq 0 ]; then
     echo 'No host address!'
     exit
fi
trap cleanup SIGINT SIGTERM
while [ 1 ]; do
    count $1
done

#Example:
#./L1.sh 91.198.174.192 (wikipedia.org)


