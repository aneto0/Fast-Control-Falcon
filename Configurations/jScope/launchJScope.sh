#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export mds_falconf_path=$DIR/../Tree
if pgrep "mdsip" > /dev/null
then
    echo "mdsip is already running"
else
    xterm -hold -e mdsip -p 8020 -m -h $MDSPLUS_DIR/etc/mdsip.hosts &
    sleep 2
fi
#jScope $DIR/ADC0F_Trigger.jscp &
jScope $DIR/ADC0F.jscp &

