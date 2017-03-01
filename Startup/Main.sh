if [ -z ${MARTe2_DIR+x} ]; then echo "Please set the MARTe2_DIR environment variable"; exit; fi
if [ -z ${MARTe2_Components_DIR+x} ]; then echo "Please set the MARTe2_Components_DIR environment variable"; exit; fi
if [ -z ${EFDA_MARTe_DIR+x} ]; then echo "Please set the EFDA_MARTe_DIR environment variable"; exit; fi

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../Build/linux/GAMs/WaveformTestGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/linux/Core/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/NI6259/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/NI6368/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/SDN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/DataSources/UDP/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/IOGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/BaseLib2GAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/GAMs/ConversionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/linux/Components/Interfaces/BaseLib2Wrapper/
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

#Disable CPU speed changing
#cpupower frequency-set --governor performance
#service cpuspeed stop

echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
#cgdb --args ../Build/linux/Startup/Main.ex $1 $2 $3 $4
#strace -o/tmp/strace.err ../Build/linux/Startup/Main.ex $1 $2  $3 $4
../Build/linux/Startup/Main.ex $1 $2 $3 $4 
