#define SS PB0
void Init_SPI();
char SPI_MasterTransmit(char cData);
char SPI_MasterRead(void);
void SPI_sendbuf(unsigned char *buff,unsigned int len,unsigned char *rbuff);
