/* Rtc_Ds1307.h */
/*
Copyright (c) 2013 Henry Leinen (henry[dot]leinen [at] online [dot] de)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef __RTC_DS1307_H__
#define __RTC_DS1307_H__



/** Class Rtc_Ds1307 implements the real time clock module DS1307
 *
 * You can read the clock and set a new time and date.
 * It is also possible to start and stop the clock.
 * Rtc_Ds1307 allows you to display the time in a 12h or 24h format
 */
class Rtc_Ds1307
{
public:
    /** Structure which is used to exchange the time and date
     */
    typedef struct {
        int sec;        /*!< seconds [0..59] */
        int min;        /*!< minutes {0..59] */
        int hour;       /*!< hours [0..23] */
        int wday;       /*!< weekday [1..7, where 1 = sunday, 2 = monday, ... */
        int date;       /*!< day of month [0..31] */
        int mon;        /*!< month of year [1..12] */
        int year;       /*!< year [2000..2255] */
    } Time_rtc;


    /** RateSelect specifies the valid frequency values for the square wave output
     */
    typedef enum {
        RS1Hz = 0,
        RS4kHz = 1,
        RS8kHz = 2,
        RS32kHz = 3
    } SqwRateSelect_t;

protected:
    I2C*    m_rtc;

    static const char *m_weekDays[];

public:
    /** public constructor which creates the real time clock object
     *
     * @param sda : specifies the pin for the SDA communication line.
     *
     * @param scl : the pin for the serial clock
     *
     */
    Rtc_Ds1307(PinName sda, PinName scl);

    ~Rtc_Ds1307();

    /** Read the current time from RTC chip
     *
     * @param time : reference to a struct tm which will be filled with the time from rtc
     *
     * @returns true if successful, otherwise an acknowledge error occured
     */
    virtual bool getTime(Time_rtc& time);

    /** Write the given time onto the RTC chip
     *
     * @param time : refereence to a struct which contains valid date and time information
     *
     * @param start : contains true if the clock shall start (or keep on running).
     *
     * @param thm : 12-hour-mode if set to true, otherwise 24-hour-mode will be set.
     *
     * @returns true if successful, otherwise an acknowledge error occured
     */
    virtual bool setTime(Time_rtc& time, bool start, bool thm);

    /** Start the clock. Please note that the seconds register need to be read and
     * written in order to start or stop the clock. This can lead to an error
     * in the time value. The recommended way of starting and stoping the clock is
     * to write the actual date and time and set the start bit accordingly.
     *
     * @returns true if the clock was started, false if a communication error occured
     */
    bool startClock();

    /** Stop the clock. Please note that the seconds register need to be read and
     * written in order to start or stop the clock. This can lead to an error
     * in the time value. The recommended way of starting and stoping the clock is
     * to write the actual date and time and set the start bit accordingly.
     *
     * @returns true if the clock was stopped, false if a communication error occured
     */
    bool stopClock();

    /** Service function to convert a weekday into a string representation
     *
     * @param wday : day of week to convert (starting with sunday = 1, monday = 2, ..., saturday = 7
     *
     * @returns the corresponding string representation
     */
    const char* weekdayToString( int wday ) {
        return m_weekDays[wday%7];
    }

    /** Enable Square Wave output. The function enables or disables the square wave output
     * of the module and sets the desired frequency.
     *
     * @param ena : if set to true, the square wave output is enabled.
     *
     * @param rs : rate select, can be either one of the four values defined by type /c RateSelect_t
     *
     * @return true if the operation was successful or false otherwise
     */
    bool setSquareWaveOutput(bool ena, SqwRateSelect_t rs);

private:
    bool read(int address, char* buffer, int len);
    bool write(int address, char* buffer, int len);

    static int bcdToDecimal(int bcd) {
        return ((bcd&0xF0)>>4)*10 + (bcd&0x0F);
    }

    static int decimalToBcd(int dec) {
        return (dec%10) + ((dec/10)<<4);
    }
};



typedef void (*RtcCallback_t) (void);


class RtcCls : public Rtc_Ds1307
{
protected:
    InterruptIn                 m_sqw;
    bool                        m_bUseSqw;
    time_t                      m_time;             //  Only used in case SQW is used
    
    bool                        m_bAlarmEnabled;
    RtcCallback_t               m_alarmfunc;
    time_t                      m_alarmTime;
    
public:
    RtcCls(PinName sda, PinName scl, PinName sqw, bool bUseSqw);
    
protected:
    void _callback(void);
    
public:
    time_t                      getTime();
    virtual bool                getTime(Time_rtc& time)                         { return Rtc_Ds1307::getTime(time); }
    void                        setTime(time_t time);
    virtual bool                setTime(Time_rtc& time, bool start, bool thm)   { return Rtc_Ds1307::setTime(time, start, thm); }
public:
    void                        setAlarm(int nSeconds, RtcCallback_t alarmfunc) {
        m_alarmfunc = alarmfunc;
        m_alarmTime = m_time + nSeconds;
        m_bAlarmEnabled = (alarmfunc == NULL) ? false : true;
    }
};

#endif // __RTC_DS1307_H__
