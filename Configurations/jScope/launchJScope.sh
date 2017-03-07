#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Please specify the jScope configuration file (jscp file)"
    exit
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export mds_falconf_path=$DIR/../Tree
if pgrep "mdsip" > /dev/null
then
    echo "mdsip is already running"
else
    xterm -hold -e mdsip -p 8020 -m -h $MDSPLUS_DIR/etc/mdsip.hosts &
    sleep 2
fi
jScope $1 &

