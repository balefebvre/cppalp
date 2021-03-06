// checkerboard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"


int main()
{
	using namespace std;

	ALP_ID nDevId, nSeqId1, nSeqId2;
	long nDmdType, nSizeX, nSizeY;
	long nReturn;
	//const long nPictureTime = 33000;	// in µs
	UCHAR *pImageData = NULL;

	const long nFrames = 300;		// max 43690 binary frames

	//FILE *file;  // unreferenced local variable
	//char *filename;  // unreferenced local variable
	//int filesize;  // unreferenced local variable
	UCHAR *block1, *block2;

	//long nCurrent;  // unreferenced local variable
	long nRepeat;

	long nBit = 8;

	tAlpProjProgress QueueInfo;

	// Add version number and important note.
	cout << "Build Version 1.01" << " : Uses inverted mirror command for the DMD setup 2" << endl;
	cout << endl;

	// Allocate the ALP high-speed device
	cout << "clef..." << endl; // modif
	long ret_val = AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &nDevId);
	if (ALP_OK != ret_val) {
		cout << "AlpDevAlloc raised error " << ret_val << endl;
		// Wait for key stroke
		std::_tprintf(_T("Press any key to close this window..."));
		do { _getch(); } while (_kbhit());
		return 1;
	}
	cout << " detectee" << endl; // modif

										   //SD : Reverses black into white. Caution : takes two frames to be complete
										   //if (ALP_OK != AlpProjControl(nDevId, ALP_PROJ_INVERSION, 1)) printf("errorProjInv\n");


										   // Inquire DMD type
	cout << "DMD..."; // modif
	if (ALP_OK != AlpDevInquire(nDevId, ALP_DEV_DMDTYPE, &nDmdType)) return 1;
	cout << " detecte" << endl << endl << endl; // modif



	switch (nDmdType) {
	case ALP_DMDTYPE_XGA_055A:
	case ALP_DMDTYPE_XGA_055X:
	case ALP_DMDTYPE_XGA_07A:
		nSizeX = 1024; nSizeY = 768;
		break;
	case ALP_DMDTYPE_DISCONNECT:
	case ALP_DMDTYPE_1080P_095A:
		cout << "\t\t>>>>> Coucou j'ai trouve ton DMD <<<<<" << endl << endl; // modif

		nSizeX = 1920; nSizeY = 1080;
		break;
	case ALP_DMDTYPE_WUXGA_096A:
		nSizeX = 1920; nSizeY = 1200;
		break;
	default:
		printf("...\n");
		system("PAUSE"); // modif
						 // unsupported DMD type
		return 1;
	}

	cout << "" << endl;
	cout << "usals values:\t|| check fin\tcheck grossier\tcheck qui fonctionne!!\t||" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	cout << "frame rate\t||   40\t\t   40\t\t   40\t\t||" << endl;
	cout << "size square\t||   48\t\t   80\t\t   30\t\t||" << endl;
	cout << "nb/side\t\t||   15\t\t   9\t\t   24\t\t||" << endl;
	cout << "nb/side+\t||   17\t\t   11\t\t   25\t\t||" << endl << endl;

	//prompt frame rate (uncomment if useful)
	cout << "Enter the frame rate : ";
	double rate;
	cin >> rate;
	printf("\n");
	const long nPictureTime = long(1.0 / rate * 1000000.0);


	//checkerboard parameters

	long CheckSizeX, CheckSizeY, NbCheckX, NbCheckY, LengthRepeat, FrameNb;

	FILE *fileCB;
	errno_t err;
	const char *filenameCB;
	int filesizeCB;
	UCHAR *blockBCB, *frameCB;

	LengthRepeat = 30 * 20;

	//prompt checkerboard parameters
	cout << "Number of pixels to make one side of a single square : ";
	cin >> CheckSizeX;
	CheckSizeY = CheckSizeX;
	printf("\n");

	cout << "Number of squares to make one side of the checkerboard : ";
	cin >> NbCheckX;
	NbCheckY = NbCheckX;
	printf("\n");


	/* --------- Read binary video file uint8 ----------- */

	// filename
	filenameCB = "../../data/binarysource1000Mbits";

	// check valid file
	err = fopen_s(&fileCB, filenameCB, "rb");
	if (fileCB == NULL)
	{
		printf("Failed to open CB file for read access.");
	}

	// check file size
	fseek(fileCB, 0L, SEEK_END);
	filesizeCB = ftell(fileCB);
	rewind(fileCB);

	// allocation buffer
	blockBCB = NULL;
	blockBCB = (UCHAR *)malloc(filesizeCB);

	frameCB = NULL;
	frameCB = (UCHAR *)malloc(nSizeX*nSizeY);

	if (blockBCB == NULL)
	{
		printf("Failed to read data.\n");
	}

	if (frameCB == NULL)
	{
		printf("Failed to read data.\n");
	}

	// read file
	fread((blockBCB), 2, filesizeCB / 2, fileCB);
	fclose(fileCB);

	//printf("%d \n",blockBCB[0]);
	//printf("%d \n",blockBCB[1]);
	//printf("%d \n",blockBCB[16]);

	/*long ByteNb,j;
	long BitNb, isWhite;
	for ( j=1;j<=16 ; j++){
	ByteNb =floor(double( j / 8 ));
	BitNb = (j) % 8;
	isWhite = ( blockBCB[ByteNb] & (1<<BitNb) ) >> BitNb;
	printf("%d \n", isWhite);
	}*/

	/* --------- END Read binary video file uint8 ----------- */


	///////////////////////////////////////////////////////////////////////////
	// SEQ 1 Allocate a first sequence of nFrames
	if (ALP_OK != AlpSeqAlloc(nDevId, nBit, nFrames, &nSeqId1)) printf("errorSeqAlloc\n"); // 1bit


																						   // allocation buffer
	block1 = NULL;
	block1 = (UCHAR *)malloc(nSizeX*nSizeY*nFrames);

	if (block1 == NULL)
	{
		printf("Memory block1 allocation failed.\n");
	}

	/////create frames

	FrameNb = 0;

	FrameNb = CreateFrames(block1, blockBCB, frameCB, nSizeX, nSizeY, CheckSizeX, CheckSizeY, NbCheckX, NbCheckY, LengthRepeat, FrameNb, nFrames);


	// set up repeat parameters
	nRepeat = 1; // nbre of repititions

	if (// set ALP_SEQ_REPEAT
		nRepeat>0 &&
		ALP_OK != AlpSeqControl(nDevId, nSeqId1, ALP_SEQ_REPEAT, nRepeat))
	{
		printf("error (ALP_SEQ_REPEAT).\r\n");
		return 1;
	}

	// Set up image timing
	//if (ALP_OK != AlpSeqControl( nDevId, nSeqId1, ALP_BIN_MODE, ALP_BIN_UNINTERRUPTED ) ) return 1;

	if (ALP_OK != AlpSeqTiming(nDevId, nSeqId1, ALP_DEFAULT, nPictureTime,
		ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)) return 1;

	//////////////////////////////////////////////////////////////////////////////////////
	// SEQ 2 Allocate a second sequence of nFrames
	if (ALP_OK != AlpSeqAlloc(nDevId, nBit, nFrames, &nSeqId2)) printf("errorSeqAlloc\n");	//8bit

																							// buffer 2
	block2 = NULL;
	block2 = (UCHAR *)malloc(nSizeX*nSizeY*nFrames);

	if (block2 == NULL)
	{
		printf("Failed to read data.\n");
	}

	// Create more frames

	/*// For debuging
	CheckSizeX = 160*2;
	CheckSizeY = 160*2;


	NbCheckX = 2;
	NbCheckY = 2;*/

	///////////////////////

	FrameNb = CreateFrames(block2, blockBCB, frameCB, nSizeX, nSizeY, CheckSizeX, CheckSizeY, NbCheckX, NbCheckY, LengthRepeat, FrameNb, nFrames);


	// set up repeat parameters

	if (// set ALP_SEQ_REPEAT
		nRepeat>0 &&
		ALP_OK != AlpSeqControl(nDevId, nSeqId2, ALP_SEQ_REPEAT, nRepeat))
	{
		printf("error (ALP_SEQ_REPEAT).\r\n");
		return 1;
	}

	// Set up image timing
	if (ALP_OK != AlpSeqTiming(nDevId, nSeqId2, ALP_DEFAULT, nPictureTime,
		ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)) return 1;



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PLAY ON DMD

	//Set up queue mode
	if (ALP_OK != AlpProjControl(nDevId, ALP_PROJ_QUEUE_MODE, ALP_PROJ_SEQUENCE_QUEUE)) return 1;

	// Transmit images into ALP memory
	nReturn = AlpSeqPut(nDevId, nSeqId1, ALP_DEFAULT, ALP_DEFAULT, block1);
	if (ALP_OK != nReturn) return 1;

	// Start Sequence 1 in queue mode
	if (ALP_OK != AlpProjStart(nDevId, nSeqId1)) return 1;
	_tprintf(_T("Seq1 \n"));

	// Transmit images into ALP memory
	nReturn = AlpSeqPut(nDevId, nSeqId2, ALP_DEFAULT, ALP_DEFAULT, block2);
	if (ALP_OK != nReturn) return 1;

	// Start sequence 2
	if (ALP_OK != AlpProjStart(nDevId, nSeqId2)) return 1;
	_tprintf(_T("Seq2 \n"));

	_tprintf(_T("infernal loop start \n"));

	long i;
	int ts = 30;

	for (i = 0; i<3 * 360; i++)
	{	/////////////////////// SEQ1
		////// create more frames

		// For debuging
		/*CheckSizeX = 160*2;
		CheckSizeY = 160*2;

		NbCheckX = 2;
		NbCheckY = 2;*/

		///////////////////////

		FrameNb = CreateFrames(block1, blockBCB, frameCB, nSizeX, nSizeY, CheckSizeX, CheckSizeY, NbCheckX, NbCheckY, LengthRepeat, FrameNb, nFrames);

		/// test sequence in play
		AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
		printf("SeqID = %d , Queue NWaiting = %d, Nframes = %d . \n", QueueInfo.SequenceId, QueueInfo.nWaitingSequences, QueueInfo.nFramesPerSubSequence);

		while (QueueInfo.nWaitingSequences == 1)
		{
			AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
			Sleep(ts);
		}

		AlpProjControl(nDevId, ALP_PROJ_RESET_QUEUE, ALP_DEFAULT);
		printf("Seq 1 Finished . \n");

		if (_kbhit()) break;

		// Free Seq1
		nReturn = AlpSeqFree(nDevId, nSeqId1);
		_tprintf(_T(" ALP STATUS %d \n"), nReturn);

		//Reallocate Sequence 1
		if (ALP_OK != AlpSeqAlloc(nDevId, nBit, nFrames, &nSeqId1)) printf("errorSeqAlloc\n"); // 1bit
		_tprintf(_T("okalloc \n"));

		// Set up image timing
		if (ALP_OK != AlpSeqTiming(nDevId, nSeqId1, ALP_DEFAULT, nPictureTime,
			ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)) return 1;

		// Transmit images into Seq1
		nReturn = AlpSeqPut(nDevId, nSeqId1, ALP_DEFAULT, ALP_DEFAULT, block1);
		_tprintf(_T(" ALP STATUS %d \n"), nReturn);

		// Start Sequence 1 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId1)) return 1;
		_tprintf(_T("Seq1 \n"));


		if (_kbhit()) break;

		///////////////// SEQ2

		////// create more frames

		// For debuging
		/*CheckSizeX = 160*2;
		CheckSizeY = 160*2;

		NbCheckX = 2;
		NbCheckY = 2;*/

		///////////////////////

		FrameNb = CreateFrames(block2, blockBCB, frameCB, nSizeX, nSizeY, CheckSizeX, CheckSizeY, NbCheckX, NbCheckY, LengthRepeat, FrameNb, nFrames);

		AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
		printf("SeqID = %d , Queue NWaiting = %d, Nframes = %d . \n", QueueInfo.SequenceId, QueueInfo.nWaitingSequences, QueueInfo.nFramesPerSubSequence);

		while (QueueInfo.nWaitingSequences == 1)
		{
			AlpProjInquireEx(nDevId, ALP_PROJ_PROGRESS, &QueueInfo);
			Sleep(ts);
		}

		AlpProjControl(nDevId, ALP_PROJ_RESET_QUEUE, ALP_DEFAULT);
		printf("Seq 1 Finished . \n");

		if (_kbhit()) break;

		// Free Seq2
		nReturn = AlpSeqFree(nDevId, nSeqId2);
		_tprintf(_T(" ALP STATUS %d \n"), nReturn);

		//Reallocate Sequence 2
		if (ALP_OK != AlpSeqAlloc(nDevId, nBit, nFrames, &nSeqId2)) printf("errorSeqAlloc\n"); // 1bit
		_tprintf(_T("okalloc \n"));

		// Set up image timing
		if (ALP_OK != AlpSeqTiming(nDevId, nSeqId2, ALP_DEFAULT, nPictureTime,
			ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)) return 1;

		// Transmit images into Seq2
		nReturn = AlpSeqPut(nDevId, nSeqId2, ALP_DEFAULT, ALP_DEFAULT, block2);
		_tprintf(_T(" ALP STATUS %d \n"), nReturn);

		// Start Sequence 2 in queue mode
		if (ALP_OK != AlpProjStart(nDevId, nSeqId2)) return 1;
		_tprintf(_T("Seq1 \n"));

		if (_kbhit()) break;

	}


	// Wait for key stroke
	_tprintf(_T("Press any key to stop ALP projection"));
	do { _getch(); } while (_kbhit());


	// Done
	AlpDevHalt(nDevId);
	AlpDevFree(nDevId);

	return 0;
}

