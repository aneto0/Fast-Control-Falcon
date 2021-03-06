#!/bin/bash
#Arguments -f FILENAME -m MESSAGE [-d cgdb|strace]
#-f FILENAME=MARTe configuration file
#-m MESSAGE=Start message
#-d cgdb=Run with cgdb
#-d strace=Run with strace
#-x DAN_CONFIG_LOCATION=Location of the DANConfig.xml (e.g. ~/Projects/Fast-Control-Falcon/Configurations/DANTestConfig.xml)
#-p MDS_FALCON_PATH=location of the mds tree (e.g. -p="192.168.130.46:8020::/home/aneto/Projects/Fast-Control-Falcon/Configurations/Tree")

#Run with cgdb or strace?
DEBUG=""

#Consume input arguments
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -f|--file)
    FILE="$2"
    shift # past argument
    ;;
    -m|--message)
    MESSAGE="$2"
    shift # past argument
    ;;
    -d|--debug)
    DEBUG="$2"
    shift # past argument
    ;;
    -x|--dan_config)
    DAN_CONFIG_LOCATION="$2"
    shift # past argument
    ;;
    -p|--mds_falcon_path)
    MDS_FALCON_PATH="$2"
    shift # past argument
    ;;
    -e|--mdsevent_interface)
    MDS_EVENT_INTERFACE="$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done

if [ -z ${MARTe2_DIR+x} ]; then echo "Please set the MARTe2_DIR environment variable"; exit; fi
if [ -z ${MARTe2_Components_DIR+x} ]; then echo "Please set the MARTe2_Components_DIR environment variable"; exit; fi
if [ -z ${EFDA_MARTe_DIR+x} ]; then echo "Please set the EFDA_MARTe_DIR environment variable"; exit; fi

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/FilterDownsamplingGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/WaveformTestGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/TriggerTestGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/TriggerMaskGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/TimeCorrectionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/linux/Core/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/EPICSCA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/DAN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/NI6259/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/NI6368/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/SDN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/UDP/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/MDSWriter/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/RealTimeThreadSynchronisation/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/IOGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/BaseLib2GAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/ConversionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/FilterGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/Interfaces/BaseLib2Wrapper/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/Interfaces/SysLogger/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/Interfaces/EPICS/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/BaseLib2/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/MARTe/MARTeSupportLib/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/Interfaces/HTTP/CFGUploader/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/Interfaces/HTTP/FlotPlot/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/Interfaces/HTTP/MATLABHandler/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/Interfaces/HTTP/SignalHandler/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/GAMs/WaveformGenerator2009/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/GAMs/WebStatisticGAM/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/GAMs/PlottingGAM/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/GAMs/DataCollectionGAM/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EFDA_MARTe_DIR/Interfaces/BaseLib2Adapter/linux/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SDN_CORE_LIBRARY_DIR
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EPICS_BASE/lib/$EPICS_HOST_ARCH
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/mdsplus/lib64/

#Set the default value if it not set
if [ -z "$MDS_FALCON_PATH" ]
then
export falcon_fast_path="../Configurations/Tree"
else
export falcon_fast_path="$MDS_FALCON_PATH"
fi
echo $mds_falcon_path
export mdsevent_interface=$MDS_EVENT_INTERFACE

#Resets the PV values
caput TEST-AUX-FDAQ:Fast_Status 0
caput TEST-AUX-FDAQ:Fast_Error 0
caput TEST-AUX-FDAQ:Fast_Status_CMD 0
caput TEST-AUX-FDAQ:Fast_Error_Rst 0

echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

#Disable CPU speed changing
service cpuspeed stop

#Allocate dynamic ticks to CPU #0
for i in `pgrep rcu[^c]` ; do taskset -pc 0 $i ; done

#Assign IRQ to correct CPU
tuna -q nixseries -c 3 -x -m

#Isolate cpus 1-3 (tasks and interrupts)
tuna -c 1-3 --isolate

#Starts the DAN services only if required
if [ ! -z "$DAN_CONFIG_LOCATION" ]
then
/opt/codac/bin/danApiTool api init $DAN_CONFIG_LOCATION
fi

#Start with cgdb or with strace
if [ "$DEBUG" = "cgdb" ]
then
    cgdb --args ../Build/linux/Startup/Main.ex -f $FILE -m $MESSAGE
elif [ "$DEBUG" = "strace" ]
then
    strace -o/tmp/strace.err ../Build/linux/Startup/Main.ex -f $FILE -m $MESSAGE
else
    taskset 1 ../Build/linux/Startup/Main.ex -f $FILE -m $MESSAGE &
fi

if [ ! -z "$DAN_CONFIG_LOCATION" ]
then
/opt/codac/bin/danApiTool api close
fi

