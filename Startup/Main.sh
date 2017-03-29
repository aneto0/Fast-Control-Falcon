if [ -z ${MARTe2_DIR+x} ]; then echo "Please set the MARTe2_DIR environment variable"; exit; fi
if [ -z ${MARTe2_Components_DIR+x} ]; then echo "Please set the MARTe2_Components_DIR environment variable"; exit; fi
if [ -z ${EFDA_MARTe_DIR+x} ]; then echo "Please set the EFDA_MARTe_DIR environment variable"; exit; fi

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/FilterDownsamplingGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/WaveformTestGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/TriggerTestGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/TriggerMaskGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/linux/Core/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/NI6259/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/NI6368/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/SDN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/UDP/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/MDSWriter/
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

#export mds_falconf_path=../Configurations/Tree
#export mds_falconf_path="192.168.130.211:8020::/home/aneto/Projects/Fast-Control-Falcon/Configurations/Tree"
export mds_falconf_path="192.168.130.46:8020::/home/aneto/Projects/Fast-Control-Falcon/Configurations/Tree"

caput FALCON::FAST::STATUS 0
caput FALCON::FAST::PULSE 0
caput FALCON::FAST::ERROR 0
caput FALCON::FAST::STATUS_CMD 0
caput FALCON::FAST::ERROR_RST 0

echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
#cgdb --args ../Build/linux/Startup/Main.ex $1 $2 $3 $4
#strace -o/tmp/strace.err ../Build/linux/Startup/Main.ex $1 $2  $3 $4

#Disable CPU speed changing
service cpuspeed stop

#Allocate dynamic ticks to CPU #0
for i in `pgrep rcu[^c]` ; do taskset -pc 0 $i ; done

#Assign IRQ to correct CPU
tuna -q nixseries -c 3 -x -m

#Isolate cpus 1-3 (tasks and interrupts)
tuna -c 1-3 --isolate

taskset 1 ../Build/linux/Startup/Main.ex $1 $2 $3 $4 
