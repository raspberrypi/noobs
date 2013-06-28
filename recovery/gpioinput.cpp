#include "gpioinput.h"
#include <QFile>
#include <QDebug>

/* Class to query the value of a gpio input using /sys/class/gpio
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

GpioInput::GpioInput(int number)
    : _number(number)
{
    _numberStr = QByteArray::number(_number);
    QFile f("/sys/class/gpio/export");
    f.open(f.WriteOnly);
    f.write(_numberStr+"\n");
    f.close();
    f.setFileName("/sys/class/gpio/gpio"+_numberStr+"/direction");
    f.open(f.WriteOnly);
    f.write("in\n");
    f.close();
}

GpioInput::~GpioInput()
{
    QFile f("/sys/class/gpio/unexport");
    f.open(f.WriteOnly);
    f.write(_numberStr+"\n");
    f.close();
}

int GpioInput::value()
{
    bool ok;

    QFile f("/sys/class/gpio/gpio"+_numberStr+"/value");
    f.open(f.ReadOnly);
    int value = f.readAll().trimmed().toInt(&ok);
    f.close();
    if (!ok)
        value = -1;

    qDebug() << "gpio" << _number << "value" << value;
    return value;
}
