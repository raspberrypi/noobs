#ifndef KEYDETECTION_H
#define KEYDETECTION_H

/* Key detection class
 *
 * Initial author: Floris Bos
 * Maintained by Raspberry Pi
 *
 * See LICENSE.txt for license details
 *
 */

class KeyDetection
{
public:
    static bool isF10pressed();
    static bool waitForKeyboard();
protected:
    static int openKeyboard();
    static bool _isF10pressed(int fd);
};

#endif // KEYDETECTION_H
