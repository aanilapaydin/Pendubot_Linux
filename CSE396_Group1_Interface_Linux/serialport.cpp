#include <iostream>    /* Standard input/output definitions */
#include <cstdlib>
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>


#include "serialport.h"


namespace {
    int openPort(const char * fileName, int flags) {
        return (open(fileName, flags));
    }

    size_t readData(int fileDes, char *data, size_t size) {
        return (size_t)(read(fileDes, data, size));
    }

    size_t writeData(int fileDes, const char * data, size_t size) {
        return (write(fileDes, data, size));
    }

    int closePort(int fileName) {
        return (close(fileName));
    }
}

SerialPort::SerialPort() {
    setPortName("");
    setBaudRate(SerialPort::BR_9600);
    init();
}

SerialPort::SerialPort(const std::string& portName) {
    setPortName(portName);
    setBaudRate(SerialPort::BR_9600);
    init();
}

SerialPort::SerialPort(const std::string& portName, BaudRate baudRate) {
    setPortName(portName);
    setBaudRate(baudRate);
    init();
}

SerialPort::~SerialPort() {
    close();
}

void SerialPort::init() {
#ifdef WIN32
    port = INVALID_HANDLE_VALUE;
#else
    port = -1;
#endif
}

std::string SerialPort::getPortName() const {
    return (portName);
}

SerialPort::BaudRate SerialPort::getBaudRate() const {
    return (baudRate);
}

void SerialPort::setPortName(const std::string& portName) {
    this->portName = portName;
}

void SerialPort::setBaudRate(BaudRate baudRate) {
    this->baudRate = baudRate;
}

void SerialPort::wait(long millisec) {
    SP_SLEEP(millisec);
}

bool SerialPort::open() {

    port = openPort(portName.c_str(),O_RDWR | O_NOCTTY | O_NDELAY);
    if(port == -1)  {
        std::cerr << "Unable to open port.\n" << std::endl;
        return (false);
    }
    return (true);

}

bool SerialPort::prepare() {

    struct termios toptions;
    if (tcgetattr(port, &toptions) < 0) {
        std::cerr << "Couldn't get term attributes.\n" << std::endl;
        return false;
    }

    cfsetispeed(&toptions, SerialPort::getBaudRateInOS(baudRate));
    cfsetospeed(&toptions, SerialPort::getBaudRateInOS(baudRate));

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;

    if(tcsetattr(port, TCSANOW, &toptions) < 0) {
        std::cerr << "Couldn't set term attributes.\n" << std::endl;
        return (false);
    }
    return (true);

}

bool SerialPort::close() {
    if(port < 0) {
        return (false);
    }

    closePort(port);

    return (true);
}

SP_BAUDRATE SerialPort::getBaudRateInOS(BaudRate baudRate) {

    switch(baudRate) {
        case BR_300: return B300;
        case BR_1200: return B1200;
        case BR_2400: return B2400;
        case BR_4800: return B4800;
        case BR_9600: return B9600;
        case BR_19200: return B19200;
        case BR_38400: return B38400;
        case BR_57600: return B57600;
        case BR_115200: return B115200;
        default: return B9600;
    }

}

bool SerialPort::write(char b) {

    int n = writeData(port, &b, 1);
    if(n != 1) {
        return (false);
    }
    return (true);
}

bool SerialPort::write(const char * str) {
    return (writeString(std::string(str)));
}

bool SerialPort::writeString(const std::string str) {

    int n = writeData(port, str.c_str(), str.size());
    if(n != str.size()) {
        return (false);
    }
    return (true);

}

bool SerialPort::read(char * byte) {

    int n = readData(port, byte, 1);  // read a char at a time
    if(n == 1) {
        return (false);
    }
    return (true);

}

bool SerialPort::readUntil(std::string& str, char until) {
    str = "";

    char b;
    do {
        int n = readData(port, &b, sizeof(char));  // read a char at a time
        if(n == -1) {
            continue;
        } else {
            str += b;
        }
    } while(b != until);
    return (true);

}

bool SerialPort::readString(std::string& str) {
    return (readUntil(str, '\0'));
}
