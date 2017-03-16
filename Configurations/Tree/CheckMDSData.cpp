#include "stdio.h"
#include "mdsobjects.h"
#include "string.h" //adds strcmp function

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD 		   "\x1b[1m"
#define ERROR_FORMAT 	   ANSI_COLOR_RED ANSI_BOLD 

#define TOLERANCE_COMPARISON 0.0001 // to compare data with the pattern
//#define NARROW_TOLERANCE 0.0001
#define TOLERANCE_RELAX 0.1 //to count positive negative samples
#define JUMP_SAMPLES 4 // jump samples when transitions is found. Just ignore sampls and not comapre with the pattern.
#define INT_TOLERANCE 70
#define SAMPLES_TOLERANCE 30
#define SIGNAL_FREQ 10
#define FAST_SAMPLE_FREQ 2000000
#define SLOW_SAMPLE_FREQ 1000
#define SLOW_SAMPLE_PERIOD 1/SLOW_SAMPLE_FREQ

bool CheckCounter(int *data1, int nOfElements, int currentSegment, int nOfSegments){
	bool retVal = true;
	static unsigned int ref = data1[0];
	for(int i = 0; i < nOfElements; i++){
		if (data1[i] != ref){
			if(currentSegment != nOfSegments - 1){
				if(i != nOfElements -1){
					printf(ANSI_COLOR_RED "checkFalconf::CheckCounter. Error. ref = %d, data1 = %d, segment position = %i\n" ANSI_COLOR_RESET, int(ref), int(data1[i]), i);
					retVal = false;
					ref = data1[i];
				}
			}
		}
		ref++;
	}
	return retVal;
}

bool CheckSlowSignal(float *data, int nOfElements,float *pattern, int samplesPerSignalPeriod,int &indexPattern, int &tr1, int &tr2){
	bool retVal = true;
	float error = 0;
	float tolerance = TOLERANCE_COMPARISON;
	static bool firstIteration = true;
	static int initialSample = 0;
	int jumpSamples = JUMP_SAMPLES;
	if(firstIteration){
		int maxArbritraryIndex = 9; //It is funcion of the cut-off frequnecy of the filter
		tolerance = 0.5;
		int m = 1; //The first sample is not check
		indexPattern = m;
		while(m < maxArbritraryIndex){
			//skip transitions
			if(indexPattern == tr1 || indexPattern == tr2){
				m += jumpSamples;
				indexPattern += jumpSamples;
			}
			//Check the limits of the pattern index.. it should never happen due to is the first iteration..
			if(indexPattern >= samplesPerSignalPeriod){
					indexPattern -= samplesPerSignalPeriod;
				}
			if(m < maxArbritraryIndex){
				error = data[m]- pattern[indexPattern];
				if(error < 0){
					error = -error;
				}
				if(error > tolerance){
					retVal = false;
					printf(ANSI_COLOR_RED "CheckSlowSignal: Error the data does not match the pattern. index position error = %d, error = %f\n" ANSI_COLOR_RESET, m, error);
				}
				indexPattern ++;
				if(indexPattern >= samplesPerSignalPeriod){
					indexPattern -= samplesPerSignalPeriod;
				}
				m++;
			}
		}
		initialSample = m;
		tolerance = TOLERANCE_COMPARISON;
		firstIteration = false;
	}
	int i = initialSample;
	while(i < nOfElements){
		//skip transitions
		if(indexPattern == tr1 || indexPattern == tr2){
			i += jumpSamples;
			indexPattern += jumpSamples;
			if(indexPattern >= samplesPerSignalPeriod){
				indexPattern -= samplesPerSignalPeriod;
			}
		}
		if(i < nOfElements){
			error = data[i] - pattern[indexPattern];
			if(error < 0){
				error = -error;
			}
			if(error > tolerance){
				retVal = false;
				printf(ANSI_COLOR_RED "CheckSlowSignal: Error the data does not match the pattern. index position error = %d, error = %f, \n" ANSI_COLOR_RESET, i, error);
			}
			indexPattern ++;
			i++;
			if(indexPattern >= samplesPerSignalPeriod){
				indexPattern -= samplesPerSignalPeriod;
			}
		}
	}
	initialSample = i-nOfElements;
	return retVal;
}

bool learnPattern(float *data, float *pattern,int samplesPerSignalPeriod, int &tr1, int &tr2){
	bool retVal = true;
	int end = samplesPerSignalPeriod*2;
	for(int i = samplesPerSignalPeriod; i < end; i++){
		pattern[i-samplesPerSignalPeriod] = data[i];
	}
	/*********************/
	/*validation pattern */
	/*********************/
	//check period
	float error = data[end]-pattern[0];
	if(error < 0.0){
		error = -error;
	} 
/*
	float narrowTolerance = NARROW_TOLERANCE;
	if(error > narrowTolerance){
		printf("LearnPattern: Error. Incorrect period. pattern[0] - data[samplesPerSignalPeriod*2] > narrowTolerance. narrowTolerance =%f. error = %f\n", narrowTolerance, error);
		retVal = false;
	}
*/
	//Check that there are the same number of positive and negative samples
	int negativeSamples = 0;
	int positiveSamples = 0;

	float tolerance = TOLERANCE_RELAX;
	bool setTr1 = true;
	bool setTr2 = true;
	float error1 = 0.0;
	float error2 = 0.0;
	for(int i = 0; i < samplesPerSignalPeriod; i++){
		error1 = 0.5 - pattern[i];
		error2 = -0.5 - pattern[i];
		if(error1 < 0){
			error1 = -error1;
		}
		if(error2 < 0){
			error2 = -error2;
		}
		if(error1 < tolerance){
			positiveSamples++;
		}else if(error2 < tolerance){
			negativeSamples++;
		}
	}
	for(int i = 0; i < samplesPerSignalPeriod; i++){
		error1 = 0.5 - pattern[i];
		error2 = -0.5 - pattern[i];
		if(error1 < 0){
			error1 = -error1;
		}
		if(error2 < 0){
			error2 = -error2;
		}
		if(error1 < tolerance){
		}else if(error2 < tolerance){
		}
		else{
			if(setTr1){
				if(i == 0){
					i += JUMP_SAMPLES;
				}
				else{
					tr1 = i;
					setTr1 = false;
				}
			}else{
				if(setTr2 && i > tr1 + 4){
					tr2 = i;
					setTr2 = false;
				}else if (!setTr2 && (i > tr2 + 4) && tr1 > 3){
					printf(ANSI_COLOR_RED "LearnPattern:Error.An extra transition where detected. tr1 = %d, tr2 = %d new transition i = %d\n" ANSI_COLOR_RESET, tr1, tr2, i);
					retVal = false;
				}else{
					//Do nohing is just the first transition where found and still is on the transition
				}
			}
		}
	}
	if(positiveSamples != negativeSamples){
		retVal = false;
		printf(ANSI_COLOR_RED"learnPattern:ERROR. The numer of positive samples is different of the number of negative samples. negativeSamples = %d, positiveSamples = %d\n" ANSI_COLOR_RESET, negativeSamples, positiveSamples); 
	}
	else{
		printf("learnPattern: Information. negativeSamples = %d, positiveSamples = %d\n", negativeSamples, positiveSamples);
	}

	if(!setTr1 && setTr2){// it means that the transition where the first samples but it was known until the end due to no more transitions where found.
		tr2 = 0;
		setTr2 = true;
	}
	if(setTr1 && setTr2){
		printf(ANSI_COLOR_RED "learnPattern: error the transitions where not found\n" ANSI_COLOR_RESET);
		retVal = false;
	}
	return retVal;
}

bool CheckContinuosTime(double *slowTime,double lastSlowTime, double slowSamplePeriod, int nOfSamples){
	bool retVal = true;
	double timeDiff;
	double timeTolerance = 1e-9;
	timeDiff = (slowTime[0] - lastSlowTime) - slowSamplePeriod;
	if(timeDiff < 0){
		timeDiff = -timeDiff;
	}
	if(timeDiff > timeTolerance){
		printf(ERROR_FORMAT "CheckContinuosTime. Error. Time discontinuity.  slowTime[0] = %lf, lastSlowTime = %lf, slowSamplePeriod = %lf, timeDiff = %lf,\n" ANSI_COLOR_RESET, slowTime[0], lastSlowTime, slowSamplePeriod, timeDiff);
		retVal = false;
	}
	for(int i = 1; i < nOfSamples; i++){
		timeDiff = (slowTime[i] - slowTime[i-1])- slowSamplePeriod;
		if(timeDiff < 0){
			timeDiff = -timeDiff;
		}
		if(timeDiff > timeTolerance){
			printf(ERROR_FORMAT "CheckContinuosTime. Error. Time discontinuity. slowTime[%d] = %lf, slowTime[%d] = %lf, slowSamplePeriod = %lf, timeDiff = %lf\n" ANSI_COLOR_RESET, i, slowTime[i], i-1,  slowTime[i-1], slowSamplePeriod, timeDiff);
			retVal = false;
		}
	}
	return retVal;
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("Incorrect number of inputs. Usage ./%s <pulseNumber> <ADCnumber[0:15]>", argv[0]);
		return -1;
	}
	printf(ANSI_COLOR_GREEN"Input parameters:TOLERANCE_COMPARISON = %lf,  TOLERANCE_RELAX = %lf, INT_TOLERANCE = %d, JUMP_SAMPLES = %d, SAMPLES_TOLERANCE = %d, SIGNAL_FREQ = %d, FAST_SAMPLE_FREQ = %d, SLOW_SAMPLE_FREQ = %d\n" ANSI_COLOR_RESET, TOLERANCE_COMPARISON, TOLERANCE_RELAX, INT_TOLERANCE, JUMP_SAMPLES, SAMPLES_TOLERANCE, SIGNAL_FREQ, FAST_SAMPLE_FREQ, SLOW_SAMPLE_FREQ);
	char *nameProgram  = argv[0];
	int pulseNumber    = atoi(argv[1]);
	//int ADCnumber 	   = atoi(argv[2]);
	char *strADCnumber = argv[2];
	
	const char *const treeName = "mds_falconf";
	char fullSignalFast[100];
	char fullSignalSlow[100];
	sprintf(fullSignalFast,"ADC%s:FAST", strADCnumber);
	sprintf(fullSignalSlow,"ADC%s:SLOW", strADCnumber);
	MDSplus::Tree *myTree = NULL;
	try{
		myTree = new MDSplus::Tree(treeName, pulseNumber);
	}catch(MDSplus::MdsException &exc){
		printf(ANSI_COLOR_RED "%s::Cannot open %s with the pulse %d.Check that the pulse number exists in the Tree folder\n" ANSI_COLOR_RESET, nameProgram, treeName, pulseNumber);
		return -1;
	}
	MDSplus::TreeNode *node = NULL;
	try{
		node = myTree->getNode(fullSignalSlow);
	}catch(MDSplus::MdsException &exc){
		printf(ANSI_COLOR_RED "%s::Cannot read %s node. Check if it is correct\n" ANSI_COLOR_RESET, nameProgram, fullSignalSlow);
		return 0;
	}

	printf("\n*************************************************************\n");
	printf("*******************Checking slow signal**********************\n");
	printf("*************************************************************\n");	

	//Check if there are data in the node
	int maxNumberOfSegments = node->getNumSegments();
	if(maxNumberOfSegments == 0){
		printf(ANSI_COLOR_RED "%s:: No data in the node %s\n" ANSI_COLOR_RESET,nameProgram, fullSignalSlow);
	}else{
		printf("maxNumberOfSegments = %d\n",maxNumberOfSegments);
		int currentSegment = 0;
		MDSplus::Data *dataD = NULL;
		MDSplus::Data *slowTimeD = NULL; 
		int nOfElements = 0;
		float *data = NULL;
		double *slowTime = NULL;
		double lastSlowTime;
		bool ok = true;
		int signalFrequency = SIGNAL_FREQ;
		int lowSamplingFrequency = SLOW_SAMPLE_FREQ;
		int samplesPerSignalPeriod= lowSamplingFrequency/signalFrequency;
		double slowSamplePeriod = (double)SLOW_SAMPLE_PERIOD;
		printf("%lf\n",slowSamplePeriod);
		if(samplesPerSignalPeriod*signalFrequency != lowSamplingFrequency){
			printf(ERROR_FORMAT "Slow:Error. lowSamplingFrequency is not a multiple of the signalFrequency. lowSamplingFrequency = %d, signalFrequency = %d\n" ANSI_COLOR_RESET,lowSamplingFrequency, signalFrequency);
		}
		float *pattern = new float[samplesPerSignalPeriod];
		bool initialized = false;
		int indexPattern = 0;
		int tr1;
		int tr2;

		/****************/
		/*Read time data*/
		/****************/
		char dataType[20];
		sprintf(dataType, "%s", node->getDType());	
		if((strcmp(dataType, "DTYPE_L") == 0)){
			printf("Type of data is int32\n");
		}else if ((strcmp(dataType, "DTYPE_FS") == 0)){
			printf("Type of data is float\n");
		}
		else if ((strcmp(dataType, "DTYPE_FT") == 0)){
			printf("Type of data is double\n");
		}else{
			printf("error known type. dataType = %s\n", dataType);
			ok = false;
		}
		while((currentSegment < maxNumberOfSegments && ok)) {
			dataD = node->getSegment(currentSegment);
			data  = dataD->getFloatArray(&nOfElements);
			slowTimeD = node->getSegmentDim(currentSegment);
			slowTime = slowTimeD->getDoubleArray(&nOfElements);
			if(!initialized){
				printf("nOfElements =%d\n", nOfElements);
				//Verify that al least there are two signal cycle in one segment the pattern learns from the seconds period
				if(samplesPerSignalPeriod * 2 > nOfElements){
					printf(ANSI_COLOR_RED "%s::Error. One signal cycle cannot be fit in one segment. nOfElements per segment = %d number of samples per period = %dn" ANSI_COLOR_RESET, argv[0],nOfElements, samplesPerSignalPeriod);
					ok = false;
				}
				if(ok){
					ok &= learnPattern(data, pattern, samplesPerSignalPeriod, tr1, tr2);
				}
				initialized = true;
				printf("tr1 = %d, tr2 = %d\n", tr1, tr2);
				lastSlowTime = slowTime[0] - slowSamplePeriod;
			}
			if(ok){
				ok = ok & CheckSlowSignal(data, nOfElements, pattern, samplesPerSignalPeriod, indexPattern, tr1, tr2);
				if(!ok){
					printf(ANSI_COLOR_RED "Segment where CheckSlowSignal failed = %d\n" ANSI_COLOR_RESET, currentSegment);
				}				
			}
			CheckContinuosTime(slowTime, lastSlowTime, slowSamplePeriod, nOfElements);
			
			lastSlowTime = slowTime[nOfElements-1];	
			currentSegment++;
			deleteData(dataD);
			delete data;
		}
		delete[](pattern);
	}
	delete(node); 

    node = NULL;
	printf("\n*************************************************************\n");
	printf("*******************Checking counter**************************\n");
	printf("*************************************************************\n");
	try{
		node = myTree->getNode("COUNTER");
	}catch(MDSplus::MdsException &exc){
		printf(ANSI_COLOR_RED "%s::Cannot read COUNTER node. Check if it is correct\n" ANSI_COLOR_RESET,nameProgram);
		return 0;
	}	
	//Check if there are data in the node
	maxNumberOfSegments = node->getNumSegments();
	if(maxNumberOfSegments == 0){
		printf(ANSI_COLOR_RED "%s:: No data in the node COUNTER\n"ANSI_COLOR_RESET,nameProgram);
	}else{
		printf("maxNumberOfSegments = %d\n",maxNumberOfSegments);
		int currentSegment = 0;
		MDSplus::Data *dataD = NULL; 
		int nOfElements = 0;
		bool ok = true;
		/****************/
		/*Read time data*/
		/****************/
		char dataType[20];
		sprintf(dataType, "%s", node->getDType());	
		if((strcmp(dataType, "DTYPE_L") == 0)){
			printf("Type of data is int32\n");
		}else if ((strcmp(dataType, "DTYPE_FS") == 0)){
			printf("Type of data is float\n");
		}
		else if ((strcmp(dataType, "DTYPE_FT") == 0)){
			printf("Type of data is double\n");
		}else if(strcmp(dataType, "DTYPE_LU") == 0){
			printf("Type of data is uint32\n");
		}else if(strcmp(dataType, "DTYPE_W") == 0){
			printf("Type of data is int16\n");
		}
		else{
			printf(ANSI_COLOR_RED "error known type. dataType = %s\n" ANSI_COLOR_RESET, dataType);
			ok = false;
		}
		int *data1 = NULL;
		while((currentSegment < maxNumberOfSegments)) {
			dataD = node->getSegment(currentSegment);
			data1  = dataD->getIntArray(&nOfElements);
			ok = CheckCounter(data1, nOfElements, currentSegment, maxNumberOfSegments);
			if(!ok){
				printf(ANSI_COLOR_RED"Segment where COUNTER failed = %d\n" ANSI_COLOR_RESET, currentSegment);
				ok = true;
			}					
			currentSegment++;
			deleteData(dataD);
			delete data1;
		}
	}

	delete(node); 
    node = NULL;
	printf("\n*************************************************************\n");
	printf("*******************Checking fast signal**********************\n");
	printf("*************************************************************\n");
	try{
		node = myTree->getNode(fullSignalFast);
	}catch(MDSplus::MdsException &exc){
		printf(ANSI_COLOR_RED "%s::Cannot read %s node. Check if it is correct\n"ANSI_COLOR_RESET,nameProgram, fullSignalFast);
		return 0;
	}	
	//Check if there are data in the node
	maxNumberOfSegments = node->getNumSegments();
	if(maxNumberOfSegments == 0){
		printf(ANSI_COLOR_RED "%s:: No data in the node %s\n" ANSI_COLOR_RESET,nameProgram, fullSignalFast);
	}else{
		printf("maxNumberOfSegments = %d\n",maxNumberOfSegments);
		int currentSegment = 0;
		MDSplus::Data *dataD = NULL; 
		int nOfElements = 0;
		/****************/
		/*Read time data*/
		/****************/
		char dataType[20];
		sprintf(dataType, "%s", node->getDType());	
		if((strcmp(dataType, "DTYPE_L") == 0)){
			printf("Type of data is int32\n");
		}else if ((strcmp(dataType, "DTYPE_FS") == 0)){
			printf("Type of data is float\n");
		}
		else if ((strcmp(dataType, "DTYPE_FT") == 0)){
			printf("Type of data is double\n");
		}else if(strcmp(dataType, "DTYPE_LU") == 0){
			printf("Type of data is uint32\n");
		}
		else if(strcmp(dataType, "DTYPE_W") == 0){
			printf("Type of data is int16\n");
		}
		else{
			printf(ANSI_COLOR_RED "error known type. dataType = %s\n" ANSI_COLOR_RESET, dataType);
		}
		bool newTrigger = true;
		MDSplus::Data *timeD = NULL;
		double *time = 0;
		int nOfElementsT = 0;
		short int *data1 = NULL;
		bool exit = false;
		int countSamplesPerTrigger = 0;
		int positiveSamplesPerTrigger = 0;
		int negativeSamplesPerTrigger = 0;
		int positiveSamplesPerPeriod = 0;
		int negativeSamplesPerPeriod = 0;
		int positiveRef = 3203;
		int negativeRef = -3197;
		int error = 0;
		int error1 = 0;
		int firstPosition = 0;
		int result = FAST_SAMPLE_FREQ/SIGNAL_FREQ;
		if((result*SIGNAL_FREQ) != FAST_SAMPLE_FREQ){
			printf(ANSI_COLOR_RED "Error. The FAST_SAMPLE_FREQ must be a multiple of SIGNAL_FREQ\n" ANSI_COLOR_RESET);
			return -1;
		}
		const int samplesPerChoppingPeriod = (FAST_SAMPLE_FREQ/SIGNAL_FREQ)-(2*JUMP_SAMPLES);// samples frequency 2e6, square signal frequency 10 Hz. If some of the frequencies change here the values.
		int countSamplesPerChoppingPeriod = 0;
		int countSamplesBetweenTransitions  = 0;
		int errorSamples = 0;
		int errorSamples1 = 0;
		while((currentSegment < maxNumberOfSegments && !exit)) {
			dataD = node->getSegment(currentSegment);
			timeD = node->getSegmentDim(currentSegment);
			data1  = dataD->getShortArray(&nOfElements);
			time = timeD->getDoubleArray(&nOfElementsT);
			static double lastEndTimeTrigger = time[0]	;
			static double lastStartTimeTrigger = time[0];
			static double lastStartSegmentTime = time[0];
			static double lastEndSegmentTime =  time[0];
			if((time[0] - lastEndSegmentTime) < 0){
				printf(ANSI_COLOR_RED "%s::FAST. Error.  current StartTimeSegment = %lf, lastEndSegmentTime = %lf. currentSegment = %d, lastStartSegmentTime = %lf, currentEndSegmentTime = %lf\n" ANSI_COLOR_RESET, nameProgram, time[0], lastEndSegmentTime, currentSegment, lastStartSegmentTime, time[nOfElements-1]);
			}
			if(nOfElements != nOfElementsT){
				printf(ANSI_COLOR_RED "%s::FAST. Error. nOfElements = %d, nOfElementsT = %d\n"ANSI_COLOR_RESET, nameProgram, nOfElements, nOfElementsT);
			}
			newTrigger = ((time[0] - lastEndSegmentTime) > 1/double(FAST_SAMPLE_FREQ)+2e-9); //sample frequency is 2 MSPS. The two is to avoid rounding problems
			int i = firstPosition;
			int leftToJump = 0;
			while( i < nOfElements){
				if(newTrigger){
					lastEndTimeTrigger = lastEndSegmentTime;	
					printf("%s::FAST. Information. Number of elements per tirgger = %d. lastStartTimeTrigger = %lf, lastEndTimeTrigger = %lf. number of positive samples = %d, number of negative samples = %d, positive + negative samples = %d \n", nameProgram, countSamplesPerTrigger, lastStartTimeTrigger, lastEndTimeTrigger, positiveSamplesPerTrigger, negativeSamplesPerTrigger, positiveSamplesPerTrigger+negativeSamplesPerTrigger);
					newTrigger = false;
					countSamplesPerTrigger = 0;
					positiveSamplesPerTrigger = 0;
					negativeSamplesPerTrigger = 0;
					countSamplesPerChoppingPeriod = 0;
					positiveSamplesPerPeriod = 0;
					negativeSamplesPerPeriod = 0;
					countSamplesBetweenTransitions = 0;
					i = 0;
					lastStartTimeTrigger = time[0];
				}
				if(!(leftToJump > 0)){
					error = data1[i]- positiveRef;
					error1 = data1[i] - negativeRef;
					if(error < 0){
						error = -error;				
					}
					if(error1 < 0){
						error1 = -error1;				
					}
					if(error < INT_TOLERANCE){
						positiveSamplesPerTrigger++;
						positiveSamplesPerPeriod++;
						countSamplesPerChoppingPeriod++;
					}else if (error1 < INT_TOLERANCE){
						negativeSamplesPerTrigger++;
						negativeSamplesPerPeriod++;
						countSamplesPerChoppingPeriod++;
					}else{
						//printf("transition in t = %1.10lf\n", time[i]);
						countSamplesBetweenTransitions = 0;
						leftToJump = JUMP_SAMPLES;
					}
					if(countSamplesPerChoppingPeriod >= samplesPerChoppingPeriod){
						errorSamples = positiveSamplesPerPeriod - samplesPerChoppingPeriod/2;
						errorSamples1 = negativeSamplesPerPeriod - samplesPerChoppingPeriod/2;
						if(errorSamples < 0 ){
							errorSamples = -errorSamples;
						}
						if(errorSamples1 < 0 ){
							errorSamples1 = -errorSamples1;
						}
						if(errorSamples > SAMPLES_TOLERANCE || errorSamples1 > SAMPLES_TOLERANCE){
							printf(ANSI_COLOR_RED "%s::FAST. Error checking period. positiveSamplesPerPeriod = %d,  negativeSamplesPerPeriod = %d, (should be around %d +- %d), timeError = %lf, currentSegment = %d\n" ANSI_COLOR_RESET, nameProgram, positiveSamplesPerPeriod, negativeSamplesPerPeriod, samplesPerChoppingPeriod/2,  SAMPLES_TOLERANCE, time[i], currentSegment);
						}
						countSamplesPerChoppingPeriod = 0;
						positiveSamplesPerPeriod = 0;
						negativeSamplesPerPeriod = 0;
					}
				}else{
					leftToJump--;
				}
				countSamplesPerTrigger++;
				countSamplesBetweenTransitions++;

				i++;

			}
			firstPosition = leftToJump;
			lastStartSegmentTime = time[0];
			lastEndSegmentTime = time[nOfElements - 1];		
			currentSegment++;
			deleteData(dataD);
			deleteData(timeD);
			delete time;
			delete data1;
		}
	}

	delete(myTree);

	return 1;
}


