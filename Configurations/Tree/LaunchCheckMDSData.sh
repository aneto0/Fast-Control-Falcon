pulseNumber=$1
ADCnumber=$2
nargs=$#
if [ $nargs -lt 2 ]; then
	echo "invalid number of arguments. usage $0 <pulseNumber> <ADCnumber[0:15]>"	
	exit
fi

export mds_falconf_path=.


if [ $ADCnumber -gt 15 ];then
	echo "$0:: invalid ADCnumber. ADCnumber range [0:15]"
	exit
elif [ $ADCnumber -lt 0 ];then
	echo "$0:: invalid ADCnumber. ADCnumber range [0:15]"
	exit
fi


./CheckMDSData.ex $pulseNumber $ADCnumber
