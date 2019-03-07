typedef struct _RTCTime
{
	unsigned char year;
	unsigned char month;
	unsigned char date;
	unsigned char weekday; // Monday is 0, Sunday is 6
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} RTCTime;

void setRTCTime(unsigned char year, unsigned char month, unsigned char date, unsigned char weekday, unsigned char hour, unsigned char minute, unsigned char second);
RTCTime getRTCTime();
