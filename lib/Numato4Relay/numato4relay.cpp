#include "numato4relay.h"
#include <iostream>
#include <stdio.h>

#define DEBUG    1

namespace Pluto
{

Numato4Relay::Numato4Relay(QObject *parent) :
    QObject(parent),
    mConnected(false),
    mPortName("undefined"),
    mBaudrate(115200),
    mSerialPort(NULL)
{
    mSerialPort = new QSerialPort(this);
}

int Numato4Relay::connectToBoard(QString portName, int baudrate)
{
    if (mSerialPort->isOpen()) disconnectFromBoard();

    // Set to default if not specified.
    if (portName=="undefined") portName = DEFAULT_NUMATO4RELAY_PORT;
    if (baudrate==0)  baudrate = DEFAULT_BAUDRATE;

    // Update private variables.
    mPortName = portName;
    mBaudrate = baudrate;

    // Configure port.
    mSerialPort->setPortName(portName);
    mSerialPort->setBaudRate(baudrate);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    mSerialPort->open(QIODevice::ReadWrite);
    if( ! mSerialPort->isOpen() )
    {
        return 0;
    }

    connect(mSerialPort, &QSerialPort::readyRead,
            this, &Numato4Relay::_readUpdate);

    connect(&mStatusTimer, &QTimer::timeout,
            this, &Numato4Relay::_queryUpdate);

    mStatusTimer.start(10);

    mConnected = true;
    return 1;
}

int Numato4Relay::disconnectFromBoard()
{
    if (isConnected())
    {
        // Stop timer, disconnect signal, close port
        mStatusTimer.stop();
        disconnect(mSerialPort, &QSerialPort::readyRead,
                this, &Numato4Relay::_readUpdate);
        mSerialPort->close();
        mConnected = false;
    }
    return 1;
}

bool Numato4Relay::isConnected()
{
    return mConnected;
}

Numato4Relay::Status Numato4Relay::getUpdatedData()
{
    return mStatus;
}

void Numato4Relay::_readUpdate()
{
    QString a = mSerialPort->readAll().trimmed();

#if DEBUG==1
    std::cout<<qPrintable(a)<<std::endl;
    std::cout.flush();
#endif
}

void Numato4Relay::_queryUpdate()
{
    static int gpioId = 0;
    QString command("");

    if (gpioId < 4)
    {
        command = "relay read ";
        command += QString::number(gpioId) + "\r";
        mSerialPort->write(command.toStdString().c_str());
    }

    gpioId++;
    if(gpioId==4) gpioId=0;
}

} // namespace Pluto
