#ifndef GPIOINPUT_H
#define GPIOINPUT_H

/* Class to query the value of a gpio input using /sys/class/gpio
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

#include <QByteArray>

class GpioInput
{
public:
    GpioInput(int number);
    virtual ~GpioInput();
    int value();

protected:
    int _number;
    QByteArray _numberStr;
};

#endif // GPIOINPUT_H
