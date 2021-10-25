#include "AudioStreamer.h"
#include <stdio.h>

int16_t testData[] = {
    
0,	6398,	12551,	18220,	23188,	27263,	30288,	32146,	32767,	32127,	30249,	27207,	23117,	18136,	12458,	6300,	
-100,	-6497,	-12644,	-18304,	-23259,	-27318,	-30326,	-32166,	-32767,	-32107,	-30210,	-27150,	-23045,	-18052,	-12365,	-6201,	
201,	6596,	12737,	18387,	23330,	27374,	30364,	32185,	32767,	32087,	30171,	27094,	22973,	17968,	12271,	6102,	
-301,	-6694,	-12829,	-18470,	-23400,	-27429,	-30402,	-32204,	-32766,	-32066,	-30132,	-27037,	-22902,	-17884,	-12178,	-6003,	
402,	6793,	12922,	18553,	23470,	27484,	30439,	32222,	32764,	32045,	30092,	26980,	22830,	17800,	12085,	5904,	
-503,	-6891,	-13014,	-18636,	-23541,	-27538,	-30476,	-32240,	-32763,	-32024,	-30052,	-26923,	-22757,	-17715,	-11991,	-5805,	
603,	6989,	13106,	18719,	23610,	27593,	30513,	32258,	32761,	32003,	30012,	26866,	22685,	17630,	11897,	5706,	
-704,	-7088,	-13199,	-18801,	-23680,	-27647,	-30549,	-32275,	-32759,	-31981,	-29971,	-26808,	-22612,	-17546,	-11804,	-5607,	
804,	7186,	13291,	18884,	23750,	27701,	30586,	32293,	32756,	31959,	29930,	26750,	22539,	17461,	11710,	5508,	
-905,	-7284,	-13383,	-18966,	-23819,	-27755,	-30622,	-32310,	-32754,	-31936,	-29889,	-26692,	-22466,	-17375,	-11616,	-5409,	
1006,	7382,	13474,	19048,	23888,	27808,	30657,	32326,	32750,	31914,	29848,	26633,	22393,	17290,	11521,	5309,	
-1106,	-7480,	-13566,	-19130,	-23956,	-27861,	-30693,	-32343,	-32747,	-31891,	-29806,	-26574,	-22319,	-17204,	-11427,	-5210,	
1207,	7578,	13658,	19211,	24025,	27914,	30728,	32359,	32743,	31867,	29764,	26515,	22245,	17119,	11333,	5111,	
-1307,	-7676,	-13749,	-19293,	-24093,	-27966,	-30763,	-32374,	-32739,	-31844,	-29722,	-26456,	-22171,	-17033,	-11238,	-5011,	
1408,	7774,	13840,	19374,	24161,	28019,	30797,	32390,	32735,	31820,	29680,	26397,	22097,	16947,	11144,	4912,	
-1508,	-7871,	-13931,	-19455,	-24229,	-28071,	-30831,	-32405,	-32730,	-31796,	-29637,	-26337,	-22023,	-16860,	-11049,	-4812,	
1609,	7969,	14022,	19536,	24297,	28123,	30865,	32419,	32725,	31771,	29594,	26277,	21948,	16774,	10954,	4713,	
-1709,	-8067,	-14113,	-19616,	-24364,	-28174,	-30899,	-32434,	-32720,	-31747,	-29550,	-26217,	-21873,	-16688,	-10859,	-4613,	
1810,	8164,	14204,	19697,	24432,	28225,	30932,	32448,	32715,	31721,	29507,	26156,	21798,	16601,	10764,	4513,	
-1910,	-8262,	-14295,	-19777,	-24498,	-28276,	-30965,	-32462,	-32709,	-31696,	-29463,	-26095,	-21723,	-16514,	-10669,	-4414,	
2011,	8359,	14385,	19857,	24565,	28327,	30998,	32476,	32703,	31670,	29419,	26034,	21647,	16427,	10574,	4314,	
-2111,	-8456,	-14475,	-19937,	-24632,	-28377,	-31031,	-32489,	-32696,	-31644,	-29374,	-25973,	-21572,	-16340,	-10479,	-4214,	
2212,	8553,	14566,	20017,	24698,	28428,	31063,	32502,	32689,	31618,	29330,	25912,	21496,	16253,	10383,	4114,	
-2312,	-8650,	-14656,	-20097,	-24764,	-28478,	-31095,	-32514,	-32682,	-31592,	-29285,	-25850,	-21420,	-16165,	-10288,	-4015,	
2412,	8747,	14746,	20176,	24830,	28527,	31126,	32527,	32675,	31565,	29239,	25788,	21344,	16078,	10192,	3915,	
-2513,	-8844,	-14835,	-20255,	-24895,	-28577,	-31157,	-32539,	-32667,	-31538,	-29194,	-25726,	-21267,	-15990,	-10097,	-3815,	
2613,	8941,	14925,	20334,	24961,	28626,	31189,	32550,	32659,	31510,	29148,	25663,	21191,	15902,	10001,	3715,	
-2713,	-9038,	-15015,	-20413,	-25026,	-28674,	-31219,	-32562,	-32651,	-31482,	-29102,	-25601,	-21114,	-15814,	-9905,	-3615,	
2814,	9135,	15104,	20492,	25090,	28723,	31250,	32573,	32642,	31454,	29055,	25538,	21037,	15726,	9809,	3515,	
-2914,	-9231,	-15193,	-20570,	-25155,	-28771,	-31280,	-32584,	-32633,	-31426,	-29009,	-25475,	-20959,	-15637,	-9713,	-3415,	
3014,	9328,	15282,	20648,	25219,	28819,	31310,	32594,	32624,	31397,	28962,	25411,	20882,	15549,	9617,	3315,	
-3114,	-9424,	-15371,	-20726,	-25284,	-28867,	-31339,	-32604,	-32614,	-31368,	-28915,	-25347,	-20804,	-15460,	-9521,	-3214,	
3214,	9521,	15460,	20804,	25347,	28915,	31368,	32614,	32604,	31339,	28867,	25284,	20726,	15371,	9424,	3114,	
-3315,	-9617,	-15549,	-20882,	-25411,	-28962,	-31397,	-32624,	-32594,	-31310,	-28819,	-25219,	-20648,	-15282,	-9328,	-3014,	
3415,	9713,	15637,	20959,	25475,	29009,	31426,	32633,	32584,	31280,	28771,	25155,	20570,	15193,	9231,	2914,	
-3515,	-9809,	-15726,	-21037,	-25538,	-29055,	-31454,	-32642,	-32573,	-31250,	-28723,	-25090,	-20492,	-15104,	-9135,	-2814,	
3615,	9905,	15814,	21114,	25601,	29102,	31482,	32651,	32562,	31219,	28674,	25026,	20413,	15015,	9038,	2713,	
-3715,	-10001,	-15902,	-21191,	-25663,	-29148,	-31510,	-32659,	-32550,	-31189,	-28626,	-24961,	-20334,	-14925,	-8941,	-2613,	
3815,	10097,	15990,	21267,	25726,	29194,	31538,	32667,	32539,	31157,	28577,	24895,	20255,	14835,	8844,	2513,	
-3915,	-10192,	-16078,	-21344,	-25788,	-29239,	-31565,	-32675,	-32527,	-31126,	-28527,	-24830,	-20176,	-14746,	-8747,	-2412,	
4015,	10288,	16165,	21420,	25850,	29285,	31592,	32682,	32514,	31095,	28478,	24764,	20097,	14656,	8650,	2312,	
-4114,	-10383,	-16253,	-21496,	-25912,	-29330,	-31618,	-32689,	-32502,	-31063,	-28428,	-24698,	-20017,	-14566,	-8553,	-2212,	
4214,	10479,	16340,	21572,	25973,	29374,	31644,	32696,	32489,	31031,	28377,	24632,	19937,	14475,	8456,	2111,	
-4314,	-10574,	-16427,	-21647,	-26034,	-29419,	-31670,	-32703,	-32476,	-30998,	-28327,	-24565,	-19857,	-14385,	-8359,	-2011,	
4414,	10669,	16514,	21723,	26095,	29463,	31696,	32709,	32462,	30965,	28276,	24498,	19777,	14295,	8262,	1910,	
-4513,	-10764,	-16601,	-21798,	-26156,	-29507,	-31721,	-32715,	-32448,	-30932,	-28225,	-24432,	-19697,	-14204,	-8164,	-1810,	
4613,	10859,	16688,	21873,	26217,	29550,	31747,	32720,	32434,	30899,	28174,	24364,	19616,	14113,	8067,	1709,	
-4713,	-10954,	-16774,	-21948,	-26277,	-29594,	-31771,	-32725,	-32419,	-30865,	-28123,	-24297,	-19536,	-14022,	-7969,	-1609,	
4812,	11049,	16860,	22023,	26337,	29637,	31796,	32730,	32405,	30831,	28071,	24229,	19455,	13931,	7871,	1508,	
-4912,	-11144,	-16947,	-22097,	-26397,	-29680,	-31820,	-32735,	-32390,	-30797,	-28019,	-24161,	-19374,	-13840,	-7774,	-1408,	
5011,	11238,	17033,	22171,	26456,	29722,	31844,	32739,	32374,	30763,	27966,	24093,	19293,	13749,	7676,	1307,	
-5111,	-11333,	-17119,	-22245,	-26515,	-29764,	-31867,	-32743,	-32359,	-30728,	-27914,	-24025,	-19211,	-13658,	-7578,	-1207,	
5210,	11427,	17204,	22319,	26574,	29806,	31891,	32747,	32343,	30693,	27861,	23956,	19130,	13566,	7480,	1106,	
-5309,	-11521,	-17290,	-22393,	-26633,	-29848,	-31914,	-32750,	-32326,	-30657,	-27808,	-23888,	-19048,	-13474,	-7382,	-1006,	
5409,	11616,	17375,	22466,	26692,	29889,	31936,	32754,	32310,	30622,	27755,	23819,	18966,	13383,	7284,	905,	
-5508,	-11710,	-17461,	-22539,	-26750,	-29930,	-31959,	-32756,	-32293,	-30586,	-27701,	-23750,	-18884,	-13291,	-7186,	-804,	
5607,	11804,	17546,	22612,	26808,	29971,	31981,	32759,	32275,	30549,	27647,	23680,	18801,	13199,	7088,	704,	
-5706,	-11897,	-17630,	-22685,	-26866,	-30012,	-32003,	-32761,	-32258,	-30513,	-27593,	-23610,	-18719,	-13106,	-6989,	-603,	
5805,	11991,	17715,	22757,	26923,	30052,	32024,	32763,	32240,	30476,	27538,	23541,	18636,	13014,	6891,	503,	
-5904,	-12085,	-17800,	-22830,	-26980,	-30092,	-32045,	-32764,	-32222,	-30439,	-27484,	-23470,	-18553,	-12922,	-6793,	-402,	
6003,	12178,	17884,	22902,	27037,	30132,	32066,	32766,	32204,	30402,	27429,	23400,	18470,	12829,	6694,	301,	
-6102,	-12271,	-17968,	-22973,	-27094,	-30171,	-32087,	-32767,	-32185,	-30364,	-27374,	-23330,	-18387,	-12737,	-6596,	-201,	
6201,	12365,	18052,	23045,	27150,	30210,	32107,	32767,	32166,	30326,	27318,	23259,	18304,	12644,	6497,	100,	
-6300,	-12458,	-18136,	-23117,	-27207,	-30249,	-32127,	-32767,	-32146,	-30288,	-27263,	-23188,	-18220,	-12551,	-6398,	0,	
};

AudioStreamer::AudioStreamer()
{
    printf("Creating Audio streamer\n");
    m_RtpServerPort  = 0;
    m_RtcpServerPort = 0;

    m_SequenceNumber = 0;
    m_Timestamp      = 1611514144;
    m_SendIdx        = 0;

    m_RtpSocket = NULLSOCKET;
    m_RtcpSocket = NULLSOCKET;

    m_prevMsec = 0;

    m_udpRefCount = 0;

    m_streamingData = xQueueCreate(STREAMING_BUFFER_SIZE, sizeof(uint16_t));
    if (m_streamingData == NULL) {
        printf("ERROR: Queue for streaming data could not be created\n");
    }

    if (xTaskCreate(doRTPStream, "RTPTask", 4096, (void*)this, 1, &m_RTPTask) != pdPASS) {
        printf("ERROR: Task for streaming data could not be created\n");   
    }
};

AudioStreamer::~AudioStreamer()
{
    
};

int AudioStreamer::SendRtpPacket(unsigned const char* data, int len)
{
    // printf("CStreamer::SendRtpPacket offset:%d - begin\n", fragmentOffset);
#define KRtpHeaderSize 12           // size of the RTP header

#define MAX_FRAGMENT_SIZE 640 // this should match up to about 20ms

    static char RtpBuf[2048]; // Note: we assume single threaded, this large buf we keep off of the tiny stack
    if (len > MAX_FRAGMENT_SIZE) len = MAX_FRAGMENT_SIZE;

    int RtpPacketSize = len + KRtpHeaderSize;

    memset(RtpBuf,0x00,sizeof(RtpBuf));

    // Prepare the 12 byte RTP header
    RtpBuf[0]  = 0x80;                               // RTP version
    RtpBuf[1]  = 0x0b | 0x80;                               // L16 payload (11) and no marker bit
    RtpBuf[3]  = m_SequenceNumber & 0x0FF;           // each packet is counted with a sequence counter
    RtpBuf[2]  = m_SequenceNumber >> 8;
    RtpBuf[4]  = (m_Timestamp & 0xFF000000) >> 24;   // each image gets a timestamp
    RtpBuf[5]  = (m_Timestamp & 0x00FF0000) >> 16;
    RtpBuf[6] = (m_Timestamp & 0x0000FF00) >> 8;
    RtpBuf[7] = (m_Timestamp & 0x000000FF);
    RtpBuf[8] = 0x13;                               // 4 byte SSRC (sychronization source identifier)
    RtpBuf[9] = 0xf9;                               // we just an arbitrary number here to keep it simple
    RtpBuf[10] = 0x7e;
    RtpBuf[11] = 0x67;

    int headerLen = 12; 

    // append data to header
    memcpy(RtpBuf + headerLen, data, len);

    m_SequenceNumber++;                              // prepare the packet counter for the next packet

    //IPADDRESS otherip;
    //IPPORT otherport;

    // RTP marker bit must be set on last fragment
    //if (m_Client->m_streaming && !session->m_stopped) {
        // UDP - we send just the buffer by skipping the 4 byte RTP over RTSP header
        //socketpeeraddr(session->getClient(), &otherip, &otherport);
    udpsocketsend(m_RtpSocket, RtpBuf, RtpPacketSize, m_ClientIP, m_ClientPort);
    //}

    // printf("CStreamer::SendRtpPacket offset:%d - end\n", fragmentOffset);
    return len;
};

u_short AudioStreamer::GetRtpServerPort()
{
    return m_RtpServerPort;
};

u_short AudioStreamer::GetRtcpServerPort()
{
    return m_RtcpServerPort;
};

bool AudioStreamer::InitUdpTransport(IPADDRESS aClientIP, IPPORT aClientPort)
{
    m_ClientIP = aClientIP;
    m_ClientPort = aClientPort;

    m_SequenceNumber = getRandom();

    if (m_udpRefCount != 0)
    {
        ++m_udpRefCount;
        return true;
    }

    for (u_short P = 6970; P < 0xFFFE; P += 2)
    {
        m_RtpSocket     = udpsocketcreate(P);
        if (m_RtpSocket)
        {   // Rtp socket was bound successfully. Lets try to bind the consecutive Rtsp socket
            m_RtcpSocket = udpsocketcreate(P + 1);
            if (m_RtcpSocket)
            {
                m_RtpServerPort  = P;
                m_RtcpServerPort = P+1;
                break;
            }
            else
            {
                udpsocketclose(m_RtpSocket);
                udpsocketclose(m_RtcpSocket);
            };
        }
    };
    ++m_udpRefCount;

    printf("RTP Streamer set up with client IP %s and client Port %i\n", inet_ntoa(m_ClientIP), m_ClientPort);

    return true;
}

void AudioStreamer::ReleaseUdpTransport(void)
{
    --m_udpRefCount;
    if (m_udpRefCount == 0)
    {
        m_RtpServerPort  = 0;
        m_RtcpServerPort = 0;
        udpsocketclose(m_RtpSocket);
        udpsocketclose(m_RtcpSocket);

        m_RtpSocket = NULLSOCKET;
        m_RtcpSocket = NULLSOCKET;
    }
}

int AudioStreamer::AddToStream(uint16_t * data, int len) {
    for (int i=0; i < len; i++) {
        if (xQueueSend(m_streamingData, &data[i], 0) != pdTRUE) {
            return i;
        }
    }

    return 0;
}

void AudioStreamer::Start() {
    printf("Starting RTP Stream\n");
    vTaskResume(m_RTPTask);
}

void AudioStreamer::Stop() {
    printf("Stopping RTP Stream\n");
    vTaskSuspend(m_RTPTask);
}

void AudioStreamer::doRTPStream(void * audioStreamerObj) {
    AudioStreamer * streamer = (AudioStreamer*)audioStreamerObj;
    int bytes = 0;
    int sent = 0;
    int sampleCount = 0;
    TickType_t prevWakeTime =  xTaskGetTickCount();

    uint8_t * testDataNet = (uint8_t*) testData;
    uint8_t tmp;
    for (int i = 0; i < 2048; i += 2) {
        tmp = testDataNet[i];
        testDataNet[i] = testDataNet[i+1];
        testDataNet[i+1] = tmp;
    }

    vTaskSuspend(NULL);     // only start when Start() is called

    while(1) {
        
        // if something in buffer, stream it
        if (sent >= 1024) {
            sent = 0;
            sampleCount++;

            //streamer->m_Timestamp += sampleCount;//64

            printf("%i samples sent (%ims); timestamp: %i\n", sampleCount, sampleCount * 64, streamer->m_Timestamp);
        }
        bytes = streamer->SendRtpPacket((unsigned char*)&testData[sent], (1024-sent) * sizeof(int16_t));
        sent += bytes;
        streamer->m_Timestamp += bytes >> 1;        // no of samples sent
        vTaskDelayUntil(&prevWakeTime, 20/portTICK_PERIOD_MS);      // delay 20ms
    }
}