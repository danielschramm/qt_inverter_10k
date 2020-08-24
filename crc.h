#ifndef CRC_H
#define CRC_H

#include <cstdint>
#include <QByteArray>
#include <QString>
#include <QtDebug>

/*
 * CRC:
 * Idea for this Class:
 * https://github.com/vaipatel/InfiniSolarP18/blob/master/src/InfiniCRC.cpp
 */

class Crc
{
private:
    const uint16_t crcTable[16]= {
      0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
      0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef
    };

    uint16_t crcResult=0;

public:
    Crc() {

    }

    void add(uint8_t inByte) {
        uint8_t da;
        da=((uint8_t)(crcResult>>8))>>4;
        crcResult<<=4;
        crcResult^=crcTable[da^(inByte>>4)];
        da=((uint8_t)(crcResult>>8))>>4;
        crcResult<<=4;
        crcResult^=crcTable[da^(inByte&0x0f)];
    }

    void add(uint8_t *c, uint16_t len) {
        for(uint16_t i=0; i<len; i++) {
            add(c[i]);
        }
    }

    bool checkResponse(QByteArray &response) {
        if(response.size() < 5) {
            return(0);
        }
        uint16_t crcTrans=0;
        crcTrans|=response[response.size()-3]&0xff;
        crcTrans<<=8;
        crcTrans|=response[response.size()-2]&0xff;

        for(int i=0; i<response.size()-3; i++) {
            add(response[i]);
        }

        uint16_t crcCalc = getCrc();

        //qDebug() << QString::number( crcTrans, 16 ) << "  " << QString::number( crcCalc, 16 );
        return (crcTrans == crcCalc);
    }

    void appendCrc(QByteArray &str) {
        for(int i=0; i<str.size(); i++) {
            add(str[i]);
        }

        str.append((getCrc()>>8)&0xff);
        str.append((getCrc())&0xff);
    }

    uint16_t getCrc() {
        uint8_t bCRCHigh;
        uint8_t bCRCLow;

        // Make bCRCLow the bottom 8 bits of crc
        bCRCLow = crcResult;
        // Make bCRCHigh the top 8 bits of crc
        bCRCHigh= (uint8_t)(crcResult>>8);

        // Apparently, we just increment (, \r and LineFeed (which I guess is \n)
        if(bCRCLow==0x28||bCRCLow==0x0d||bCRCLow==0x0a)
        {
            bCRCLow++;
        }
        // Apparently, we just increment (, \r and LineFeed (which I guess is \n)
        if(bCRCHigh==0x28||bCRCHigh==0x0d||bCRCHigh==0x0a)
        {
            bCRCHigh++;
        }
        // Reassemble crc as the (bCRCHigh,bCRCLow)
        crcResult = ((uint8_t)bCRCHigh)<<8;
        crcResult += bCRCLow;
        return(crcResult);
    }
};

#endif // CRC_H

