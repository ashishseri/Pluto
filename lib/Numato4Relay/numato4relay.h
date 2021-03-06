#ifndef NUMATO4RELAY_H
#define NUMATO4RELAY_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

#define DEFAULT_NUMATO4RELAY_PORT   "/dev/ttyACM0"
#define DEFAULT_BAUDRATE            115200

namespace Pluto
{

class Numato4Relay : public QObject
{
    Q_OBJECT
public:

    //-------------------------------------------------------------------
    // Public Api
    //-------------------------------------------------------------------

    // Constructor
    Numato4Relay(QObject *parent=0);

    class Status
    {
    public:
        int relay[4];    // 0-4 relay
        int gpio[8];     // 0-7 gpio
    };

    // Connects to the board.
    int connectToBoard(QString portName="", int baudrate=0);

    // Disconnects from the board.
    int disconnectFromBoard();

    // Returns true if connected to board else false.
    bool isConnected();

    // Returns the Status object containing updated data.
    Status getUpdatedData();

    // setRelay()
    // Sets the status of specified relay to on/off
    // on: true, off:false
    void setRelay(int relayNum, bool status);

    // setGpio()
    // Sets the status of specified gpio to on/off
    // on: true, off:false
    void setGpio(int gpioNum, bool status);


    //-------------------------------------------------------------------
    // Public Signals
    //-------------------------------------------------------------------

signals:

    // Emitted when new io data is available.
    void updateAvailable();


    //-------------------------------------------------------------------
    // Private
    //-------------------------------------------------------------------

private:
    bool mConnected;
    Status mStatus;
    QString mPortName;
    int mBaudrate;
    QSerialPort *mSerialPort;
    QTimer mStatusTimer;

    void _readUpdate();
    void _queryUpdate();
};

} // namespace Pluto

#endif // NUMATO4RELAY_H
