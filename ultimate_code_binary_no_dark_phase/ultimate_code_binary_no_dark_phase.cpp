// ultimate_code_binary_no_dark_phase.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	ALP_ID nDevId, nSeqId1, nSeqId2;
	long nDmdType, nSizeX, nSizeY;
	long nReturn;
	long nRepeat = 1;  // repetiton of each seq
	//std::printf(AlpProjInquire(ALP_ID DeviceId, long InquireType, long *UserVarPtr));
	long nOffset = 1024;  // divide the frame LUT in 2
	//const long nPictureTime = 0.016600*1000000;  // in µs, prompted from now on
	long nBit = 1;

	FILE *file;
	errno_t err;
	const char *filename, *filevec;
	UCHAR *block1;
	tFlutWrite LutArray;
	tAlpProjProgress QueueInfo;

	using namespace std;

	// Allocate the ALP high-speed device
	//if (ALP_OK != AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &nDevId)) return 1;  // TODO remove
	long ret_val = AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &nDevId);
	if (ret_val == ALP_ERROR_COMM) {
		cout << "AlpDevAlloc raised a communication error." << endl;
		// Wait for key stroke
		std::_tprintf(_T("Press any key to close this window..."));
		do { _getch(); } while (_kbhit());
		return 1;
	} else if (ret_val == ALP_DEVICE_REMOVED) {
		cout << "AlpDevAlloc raised a device removed error." << endl;
		// Wait for key stroke
		std::_tprintf(_T("Press any key to close this window..."));
		do { _getch(); } while (_kbhit());
		return 1;
	} else if (ret_val == ALP_NOT_ONLINE) {
		cout << "AlpDevAlloc raised a not online error." << endl;
		// Wait for key stroke
		std::_tprintf(_T("Press any key to close this window.."));
		do { _getch(); } while (_kbhit());
		return 1;
	} else if (ret_val != ALP_OK) {
		cout << "AlpDevAlloc raise error " << ret_val << endl;
		// Wait for key stroke
		std::_tprintf(_T("Press any key to close this window..."));
		do { _getch(); } while (_kbhit());
		return 1;
	}

	//SD : Reverses black into white. Caution : takes two frames to be complete
	//if (ALP_OK != AlpProjControl(nDevId, ALP_PROJ_INVERSION, 1)) std::printf("errorProjInv\n");

	// Inquire DMD type
	if (ALP_OK != AlpDevInquire(nDevId, ALP_DEV_DMDTYPE, &nDmdType)) return 1;
	switch (nDmdType) {
	case ALP_DMDTYPE_XGA_055A:
	case ALP_DMDTYPE_XGA_055X:
	case ALP_DMDTYPE_XGA_07A:
		nSizeX = 1024; nSizeY = 768;
		break;
	case ALP_DMDTYPE_DISCONNECT:
	case ALP_DMDTYPE_1080P_095A:
		nSizeX = 1920; nSizeY = 1080;
		break;
	case ALP_DMDTYPE_WUXGA_096A:
		nSizeX = 1920; nSizeY = 1200;
		break;
	default:
		// unsupported DMD type
		return 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	//Prompt .bin and .vec filenames
	//using namespace std;

	//const string Binvec_Path = "../../../BINVECS/";
	//const string Binvec_Path = "K:\BINVECS/";
	//const string Binvec_Path = "../../data/binvecs/";
	const string Binvec_Path = "C:/Program Files/ALP-4.1/ALP-4.1 high-speed API/Samples/BINVECS/";

	cout << "Hello Hola Bonjour\n\n";
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(Binvec_Path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		ent = readdir(dir);
		ent = readdir(dir);
		int j = 0;
		while ((ent = readdir(dir)) != NULL) {
			std::printf("%d   %s\n", j, ent->d_name);
			j = j + 1;
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("could not open binvecs directory");
		return EXIT_FAILURE;
	}
	cout << "Enter your user number : ";
	int user;
	cin >> user;	
	if ((dir = opendir(Binvec_Path.c_str())) != NULL) {
		ent = readdir(dir);
		ent = readdir(dir);
		int j = 0;
		while ((ent = readdir(dir)) != NULL && j< user) {
			j = j + 1;
		}
	}
	else {
		/* could not open directory */
		perror("could not open user binvecs directory");
		return EXIT_FAILURE;
	}
	std::printf("\n");
	const string user_name = (ent->d_name);
	std::printf("Welcome %s! \n", ent->d_name);
	closedir(dir);

	const string  Bin_Path = Binvec_Path + user_name + "/Bin/";
	const string  Vec_Path = Binvec_Path + user_name + "/Vec/";

	//stf::printf ("%s", Bin_Path);


	if ((dir = opendir(Bin_Path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		ent = readdir(dir);
		ent = readdir(dir);
		int j = 0;
		while ((ent = readdir(dir)) != NULL) {
			std::printf("%d   %s\n", j, ent->d_name);
			j = j + 1;
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("could not open user bin directory");
		return EXIT_FAILURE;
	}


	cout << "Enter the number of a .bin file : ";
	int bin;
	cin >> bin;

	if ((dir = opendir(Bin_Path.c_str())) != NULL) {
		ent = readdir(dir);
		ent = readdir(dir);
		int j = 0;
		while ((ent = readdir(dir)) != NULL && j< bin) {
			j = j + 1;
		}
	}
	else {
		/* could not open directory */
		perror("could not open user bin directory");
		return EXIT_FAILURE;
	}
	std::printf("\n");

	string bin_name;
	bin_name = (ent->d_name);
	bin_name = Bin_Path + bin_name;
	std::printf("Chosen .bin file : %s\n", ent->d_name);
	closedir(dir);

	std::printf("\n");

	if ((dir = opendir(Vec_Path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		ent = readdir(dir);
		ent = readdir(dir);
		int j = 0;
		while ((ent = readdir(dir)) != NULL) {
			std::printf("%d   %s\n", j, ent->d_name);
			j = j + 1;
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("could not open user vec directory");
		return EXIT_FAILURE;
	}

	//cout << "You choosed the bin file" << bin << ".\n";
	cout << "Enter the number of a .vec file : ";
	int vec;
	cin >> vec;

	if ((dir = opendir(Vec_Path.c_str())) != NULL) {
		ent = readdir(dir);
		ent = readdir(dir);
		int j = 0;
		while ((ent = readdir(dir)) != NULL && j< vec) {
			j = j + 1;
		}
	}
	else {
		/* could not open directory */
		perror("could not open user vec directory");
		return EXIT_FAILURE;
	}
	std::printf("\n");
	string vec_name;
	vec_name = (ent->d_name);
	vec_name = Vec_Path + vec_name;
	std::printf("Chosen .vec file : %s\n", ent->d_name);
	closedir(dir);
	std::printf("\n");


	//prompt Frequency of refresh
	cout << "Enter the frame rate (in Hz) : ";
	double rate;
	cin >> rate;

	std::printf("\n");

	//prompt Frequency of gray level refresh
	cout << "Enter the gray level refresh rate (put frame rate if no grays in movie, otherwise put 100 Hz, usually 1000 for Ultimate Code) : ";
	double gray_rate;
	cin >> gray_rate;

	//SD : the number of repeats of each frame is calculated to assure a gray level refreshed at least at gray_rate Hz.
	long nbRepeats = long(pow(2, ceil(log((double)(gray_rate / rate)) / log((double)2))));
	const long nPictureTime = long(1.0 / (rate * nbRepeats) * 1000000);

	std::printf("\n");

	cout << "Enter the bit depth (8 unless you want to go to high speeds) : ";
	double bitDepth;
	cin >> bitDepth;
	nBit = long(bitDepth);

	std::printf("\n");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Load frames

	// filename
	//filename="BarmovieGP_DS_Mk_0904_mag.bin"; 
	filename = bin_name.c_str();

	// check valid file
	err = fopen_s(&file, filename, "r");
	if (file == NULL)
	{
		std::printf("Invalid file name");
	}

	// read header
	short header[4];
	fread(header, sizeof(short), 4, file);
	std::printf("header SizeX = %d, SizeY =  %d, NbFrames = %d . \n", header[0], header[1], header[2]);
	std::fclose(file);
	std::printf("OK1 => The buffer for the BIN file will be allocated\n");



	// SD: the display window will be placed at the middle of the DMD screen
	//nBit = 8;
	int nFrames = header[2];
	int width = header[0];
	int height = header[1];

	// allocate buffer
	block1 = NULL;
	block1 = (UCHAR *)calloc(nFrames*nSizeX*nSizeY, 1);
	if (block1 == NULL)
	{
		std::printf("Failed to allocate buffer. \n");
	}

	std::printf("Allocation buffer OK2. \n");
	//////////

	// SD: coordinates of the display window
	int beg_w = (nSizeX - width) / 2;
	int beg_h = (nSizeY - height) / 2;


	// Read file
	err = fopen_s(&file, filename, "r");
	std::printf("Open OK. \n"); 	//do { _getch(); } while (_kbhit()) ; 

	fseek(file, 8, 0);
	std::printf("Fseek OK. \n"); //do { _getch(); } while (_kbhit()) ; 


							//SD: Centering frames in the middle of the screen
							// buffer
	int header_size = sizeof(short) * 4;
	// read file
	for (int i = 0; i<nFrames; i++)
	{
		fseek(file, i * width*height + header_size, SEEK_SET);
		// SD: the display window is located at the center of the screen of the DMD
		for (int line = 0; line < height; line++) {
			fread(block1 + i * (nSizeX*nSizeY) + (beg_h + line)*nSizeX + beg_w, 1, width, file);
		}
	}

	std::fclose(file);
	std::printf("Fclose OK. \n"); //do { _getch(); } while (_kbhit()) ; 


							 ////////////////////////////////////////////////////////////////////////////////////////////
							 //Read Vec file

	long int temp1, temp2, temp3, temp4, temp5;

	//	filevec="BarmovieGP_DS_Mk_0904.vec";
	filevec = vec_name.c_str();

	long int numberOfVecFrames = 0;

	err = fopen_s(&file, filevec, "r");
	if (file == NULL) {
		std::printf("Invalid file name. \n");
	}
	std::printf("File Vec opened. \n");

	fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5);
	while (fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5) != EOF)
	{
		numberOfVecFrames++;
	}
	std::fclose(file);

	std::printf("NbFrameToBePlayed = %d. \n", numberOfVecFrames);

	//SD: Modify slightly nOffset to take into account the gray level repetitions 
	//nOffset = nOffset - (nOffset % nbRepeats);
	long Nloop, Nremain;
	//SD: Nloop takes into account the gray level repetitions
	Nloop = (numberOfVecFrames * nbRepeats) / (nOffset * 2);
	Nremain = (numberOfVecFrames * nbRepeats) % (nOffset * 2);

	std::printf("Nloop = %d, Nremain = %d \n", Nloop, Nremain);

	///////////////////////////////////////////////////////////////////////
	// Fill the LUT

	LutArray.nSize = nOffset;
	LutArray.nOffset = 0;
	long i;


	err = fopen_s(&file, filevec, "r");

	fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5);

	for (i = 0; i < nOffset; i = i + nbRepeats)
	{
		fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5);
		// SD : the gray level refresh rate is guaranteed manually by introducing repetitions of the same frame.
		for (int j = 0; j < nbRepeats; j++)
		{
			LutArray.FrameNumbers[i + j] = temp2;
		}
		std::printf("%ld ", temp2);
	}

	AlpProjControlEx(nDevId, ALP_FLUT_WRITE_18BIT, &LutArray);

	// &TOTO pour passer l'adresse de la structure � la fonction
	// cela �vite de copier la structure (qui est grosse) quand on
	// appelle la fonction */

	std::printf(" Transfer Lut OK .\n");

	///////////////////////////////////////////////////////////////////////////
	// SEQ PARAMETERS

	long nframesavail;

	if (ALP_OK != AlpDevInquire(nDevId, ALP_AVAIL_MEMORY, &nframesavail)) return 1;
	std::printf("Memory 1 = %d\n", nframesavail);

	// Allocate two sequences of nFrames
	if (ALP_OK != AlpSeqAlloc(nDevId, nBit, nFrames, &nSeqId1)) std::printf("errorSeqAlloc\n");
	if (ALP_OK != AlpSeqAlloc(nDevId, nBit, nFrames, &nSeqId2)) std::printf("errorSeqAlloc\n");
	std::printf("alloc OK nFrames = %ld \n", nFrames);

	if (ALP_OK != AlpDevInquire(nDevId, ALP_AVAIL_MEMORY, &nframesavail)) return 1;
	std::printf("Memory 2 = %d\n", nframesavail);


	//SD We reduce the bitdepth of the display and we suppress the Dark Phase
	if (ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_BITNUM, 1)) return 1;
	if (ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_BIN_MODE, ALP_BIN_UNINTERRUPTED)) return 1;
	if (ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_BITNUM, 1)) return 1;
	if (ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_BIN_MODE, ALP_BIN_UNINTERRUPTED)) return 1;

	// Set timing
	if (ALP_OK != AlpSeqTiming(nDevId, nSeqId1, ALP_DEFAULT, nPictureTime,
		ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)) return 1;

	std::printf(" nPictureTime = %ld \n", nPictureTime);
	std::printf("One sequence loaded\n");

	if (ALP_OK != AlpSeqTiming(nDevId, nSeqId2, ALP_DEFAULT, nPictureTime,
		ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)) return 1;
	std::printf("timing OK \n");

	// Set repeat
	if (nRepeat>0 && ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_SEQ_REPEAT, nRepeat))
	{
		std::printf("error (ALP_SEQ_REPEAT).\r\n");
		return 1;
	}
	if (nRepeat>0 && ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_SEQ_REPEAT, nRepeat))
	{
		std::printf("error (ALP_SEQ_REPEAT).\r\n");
		return 1;
	}
	std::printf("repeat OK \n");

	// Set FLUT parameters
	//Seq1
	if (ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_FLUT_MODE, ALP_FLUT_18BIT))
	{
		std::printf("error (ALP_FLUT_MODE).\r\n");
		return 1;
	}

	if (ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_FLUT_ENTRIES9, nOffset * 2))
	{
		std::printf("error (ALP_FLUT_ENTRIES).\r\n");
		return 1;
	}

	//Seq2
	if (ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_FLUT_MODE, ALP_FLUT_18BIT))
	{
		std::printf("error (ALP_FLUT_MODE).\r\n");
		return 1;
	}

	if (ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_FLUT_ENTRIES9, nOffset * 2))
	{
		std::printf("error (ALP_FLUT_ENTRIES).\r\n");
		return 1;
	}//

	if (ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_FLUT_OFFSET9, nOffset * 2))
	{
		std::printf("error (ALP_FLUT_OFFSET).\r\n");
		return 1;
	}
	std::printf("offset ok\n");



	///////////////////////////////////////////////////////////////////////////////////
	//Set up queue mode
	if (ALP_OK != AlpProjControl(nDevId, ALP_PROJ_QUEUE_MODE, ALP_PROJ_SEQUENCE_QUEUE)) return 1;



	// Transmit images into ALP memory
	nReturn = AlpSeqPut(nDevId, nSeqId1, ALP_DEFAULT, ALP_DEFAULT, block1);
	std::printf("transmit 1 ok \n");
	std::printf(" nreturn = %d\n", nReturn);
	std::printf(" bloc 1 = %s\n", block1);
	nReturn = AlpSeqPut(nDevId, nSeqId2, ALP_DEFAULT, ALP_DEFAULT, block1);
	std::printf(" nreturn = %d\n", nReturn);
	std::printf(" bloc 1 = %s\n", block1);
	std::printf(" invalid = %d\n", ALP_ADDR_INVALID);
	std::printf(" invalid = %d\n", ALP_SEQ_IN_USE);
	std::printf(" invalid = %d\n", ALP_PARM_INVALID);
	free(block1);
	if (ALP_OK != nReturn) return 1;
	std::printf("Transmit OK. \n");

	long j;
	int ts = 30;
	for (j = 1; j <= Nloop; j++)
	{
		std::printf("j = %d. \n", j);

		// Start Sequence 1 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId1)) return 1;
		std::_tprintf(_T("Seq1 \n"));

		// Wait until Seq 1 is started to rewrite the LUT and reset the queue
		AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);

		while (QueueInfo.SequenceId == 2)
		{
			AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
			Sleep(ts);
		}

		AlpProjControl(nDevId, ALP_PROJ_RESET_QUEUE, ALP_DEFAULT);
		std::printf("Seq 2 Finished . \n");

		// Rewrite the LUT part 2
		LutArray.nSize = nOffset;
		LutArray.nOffset = nOffset;

		for (i = 0; i < nOffset; i = i + nbRepeats)
		{
			fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5);
			// SD : the gray level refresh rate is guaranteed manually by introducing repetitions of the same frame.
			for (int j = 0; j < nbRepeats; j++)
			{
				LutArray.FrameNumbers[i + j] = temp2;
			}
		}


		AlpProjControlEx(nDevId, ALP_FLUT_WRITE_18BIT, &LutArray);
		std::printf(" Transfer Lut part 2 OK .\n");

		// Start Sequence 2 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId2)) return 1;
		std::_tprintf(_T("Seq2 \n"));

		// Wait until Seq 2 is started to rewrite the LUT and reset the queue
		AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
		std::printf("SeqID = %d , Queue NWaiting = %d, Nframes = %d . \n", QueueInfo.SequenceId, QueueInfo.nWaitingSequences, QueueInfo.nFramesPerSubSequence);

		while (QueueInfo.SequenceId == 1)
		{
			AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
			Sleep(ts);
		}

		AlpProjControl(nDevId, ALP_PROJ_RESET_QUEUE, ALP_DEFAULT);
		std::printf("Seq 1 Finished . \n");

		// Rewrite the LUT part 1
		LutArray.nSize = nOffset;
		LutArray.nOffset = 0;

		for (i = 0; i < nOffset; i = i + nbRepeats)
		{
			fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5);
			// SD : the gray level refresh rate is guaranteed manually by introducing repetitions of the same frame.
			for (int j = 0; j < nbRepeats; j++)
			{
				LutArray.FrameNumbers[i + j] = temp2;
			}
		}

		AlpProjControlEx(nDevId, ALP_FLUT_WRITE_18BIT, &LutArray);
		std::printf(" Transfer Lut part 1 OK.\n");

		AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
		std::printf("SeqID = %d , Queue NWaiting = %d, Nframes = %d . \n", QueueInfo.SequenceId, QueueInfo.nWaitingSequences, QueueInfo.nFramesPerSubSequence);

		// loop
	}

	if (Nremain > nOffset)
	{
		std::printf("Nremain = %d.", Nremain);

		// Start Sequence 1 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId1)) return 1;
		std::_tprintf(_T("Seq1 \n"));

		// Wait until Seq 1 is started to rewrite the LUT and reset the queue
		AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);

		while (QueueInfo.SequenceId == 2)
		{
			AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
			Sleep(ts);
		}

		AlpProjControl(nDevId, ALP_PROJ_RESET_QUEUE, ALP_DEFAULT);
		std::printf("Seq 2 Finished . \n");

		// Rewrite the LUT part 2
		LutArray.nSize = nOffset;
		LutArray.nOffset = nOffset;

		for (i = 0; i < Nremain - nOffset; i = i + nbRepeats)
		{
			fscanf_s(file, "%ld %ld %ld %ld %ld", &temp1, &temp2, &temp3, &temp4, &temp5);
			for (int j = 0; j < nbRepeats; j++)
			{
				LutArray.FrameNumbers[i + j] = temp2;
			}
		}

		AlpProjControlEx(nDevId, ALP_FLUT_WRITE_18BIT, &LutArray);
		std::printf(" Transfer Lut part 2 OK .\n");

		if (ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_FLUT_ENTRIES9, (Nremain - nOffset) * 2))
		{
			std::printf("error (ALP_FLUT_ENTRIES).\r\n");
			return 1;
		}

		// Start Sequence 2 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId2)) return 1;
		std::_tprintf(_T("Seq2 \n"));
	}

	else
	{
		std::printf("Nremain = %d.", Nremain);

		if (ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_FLUT_ENTRIES9, Nremain * 2))
		{
			std::printf("error (ALP_FLUT_ENTRIES).\r\n");
			return 1;
		}

		// Start Sequence 1 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId1)) return 1;
		std::_tprintf(_T("Seq1 \n"));

	}


	// Wait for key stroke
	std::_tprintf(_T("Press any key to stop ALP projection"));
	do { _getch(); } while (_kbhit());

	// Done
	std::fclose(file);
	AlpDevHalt(nDevId);
	AlpDevFree(nDevId);

	return 0;
}
/*///Magnification

if( nSizeX != header[0] || nSizeY != header[1] )
{
	std::printf("size error. \n");
	long noffX, noffY, frameNb, i;
	
	noffX = (nSizeX-header[0])/2;
	noffY = (nSizeY-header[1])/2;
	FrameNb = 0;
	
	while (++ FrameNb < header[2])
	{ i = 0;
		while (++i<header[0])
		{
			fseek(file,8+header[0]*header[1]*FrameNb+i*header[0],0);
			fread(block1+(noffX+i)*nSizeX+noffY+FrameNb*nSizeX*nSizeY , 1 , header[0] , file );
		}
	}
}
else
{ */

