/***************************************************************************
                          tracesprovider.h  -  description
                             -------------------
    begin                : Mon Mar 1 2004
    copyright            : (C) 2004 by Lynn Hazan
    email                : lynn.hazan.myrealbox.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRACESPROVIDER_H
#define TRACESPROVIDER_H

//include files for the application
#include <dataprovider.h>
#include <array.h>
#include <types.h>

// include files for QT
#include <QObject>
#include <QStringList>

/**Class providing the row recorded data (contained in a .dat or .eeg file).
  *@author Lynn Hazan
  */


class TracesProvider : public DataProvider  {
    Q_OBJECT
public:

    /**Constructor.
  * @param fileUrl the url of the file containing the data provided by this class.
  * @param nbChannels the number of channels.
  * @param resolution resolution of the acquisition system used to record the data contained in the file identified by fileUrl.
  * @param samplingRate sampling rate used to record the data contained in the file identified by fileUrl.
  * @param offset offset to apply to the data contained in the file identified by fileUrl.
  */
    TracesProvider(const QString &fileUrl, int nbChannels, int resolution, int voltageRange, int amplification, double samplingRate, int offset);
    virtual ~TracesProvider();

    /// Added by M.Zugaro to enable automatic forward paging
    void updateRecordingLength() { computeRecordingLength(); }

    /**Triggers the retrieve of the traces included in the time rate given by @p startTime and @p endTime.
  * @param startTime begining of the time frame from which to retrieve the data, given in milisecond.
  * @param endTime end of the time frame from which to retrieve the data, given in milisecond.
  * @param initiator instance requesting the data.
  * @param startTimeInRecordingUnits begining of the time interval from which to retrieve the data in recording units.
  */
    void requestData(long startTime,long endTime,QObject* initiator,long startTimeInRecordingUnits);

    /**Sets the number of channels corresponding to the file identified by fileUrl.
  * @param nb the number of channels.
  */
    virtual void setNbChannels(int nb){
        nbChannels = nb;
        computeRecordingLength();
    }

    /**Sets the resolution used to record the data contained in the file identified by fileUrl.
  * @param res resolution.
  */
   virtual void setResolution(int res){
        resolution = res;
        computeRecordingLength();
    }

    /**Sets the sampling rate used to record the data contained in the file identified by fileUrl.
  * @param rate the sampling rate.
  */
    virtual void setSamplingRate(double rate){
        samplingRate = rate;
        computeRecordingLength();
    }

    /**Sets the voltage range used to record the data contained in the file identified by fileUrl.
  * @param range the voltage range.
  */
    virtual void setVoltageRange(int range){
      voltageRange = range;
    }

    /**Sets the amplification used to record the data contained in the file identified by fileUrl.
  * @param value the amplification.
  */
    virtual void setAmplification(int value){
      amplification = value;
    }

    /**Sets the offset to apply to the data contained in the file identified by fileUrl.
  * @param newOffset offset.
  */
    void setOffset(int newOffset){offset =  newOffset;}

    /**Returns the number of channels corresponding to the file identified by fileUrl.
  */
    int getNbChannels() const {return nbChannels;}

    /**Returns the resolution used to record the data contained in the file identified by fileUrl.
  */
    int getResolution() const {return resolution;}

    /**Returns the sampling rate used to record the data contained in the file identified by fileUrl.
  */
    double getSamplingRate() const {return samplingRate;}

    /**Returns the voltage range used to record the data contained in the file identified by fileUrl.
  */
    double getVoltageRange() const {return voltageRange;}

    /**Returns the amplification used to record the data contained in the file identified by fileUrl.  */
    double getAmplification() const {return amplification;}

    /**Returns the offset to apply to the data contained in the file identified by fileUrl.
  */
    int getOffset() const {return offset;}

    /**Returns the total length of the document in miliseconds.*/
    qlonglong recordingLength()const{return length;}

    /**Computes the number of samples between @p startTime and @p endTime.
  * @param startTime begining of the time frame from which the data have been retrieved, given in milisecond.
  * @param endTime end of the time frame from which to retrieve the data, given in milisecond.
  * @return number of samples in the given time frame.
  * @param startTimeInRecordingUnits begining of the time frame from which the data have been retrieved, given in recording units.
  */
    virtual dataType getNbSamples(long startTime,long endTime,long startTimeInRecordingUnits);

    /**Returns the total number of samples in recorded contained in the file identified by fileUrl.*/
    long getTotalNbSamples();

    /** Return the label for each channel, by default just the ID of the channel. */
    virtual QStringList getLabels();

public Q_SLOTS:
    /** Called when paging is started.
     * Usefull for trace providers that have live data sources.
     */
    virtual void slotPagingStarted() {};

    /** Called when paging is stopped.
    * Usefull for trace providers that have live data sources.
    */
    virtual void slotPagingStopped() {};

Q_SIGNALS:
    /**Signals that the data have been retrieved.
  * @param data array of data in uV (number of channels X number of samples).
  * @param initiator instance requesting the data.
  */
    void dataReady(Array<dataType>& data, QObject* initiator);

protected:
    /**Number of channels used to record the data.*/
    int nbChannels;

    /**Resolution of the acquisition system used to record the data.*/
    int resolution;

    /**Voltage range of acquisition system used to record the data*/
    int voltageRange;

    /**Amplification of acquisition system used to record the data*/
    int amplification;

    /**Sampling rate used to record the data.*/
    double samplingRate;

    /**Offset to apply to the data before given them away.*/
    int offset;

    /**the total length of the document in miliseconds.*/
    qlonglong length;

    //Functions

    /**Retrieves the traces included in the time frame given by @p startTime and @p endTime.
  * @param startTime begining of the time frame from which to retrieve the data, given in milisecond.
  * @param endTime end of the time frame from which to retrieve the data, given in milisecond.
  * @param initiator instance requesting the data.
  * @param startTimeInRecordingUnits begining of the time interval from which to retrieve the data in recording units.
  */
    virtual void retrieveData(long startTime,long endTime,QObject* initiator,long startTimeInRecordingUnits);

    /**Computes the total length of the document in miliseconds.*/
    virtual void computeRecordingLength();

    static inline dataType round(double d) {
      return static_cast<dataType>( (d > 0.0) ? d + 0.5 : d - 0.5);
    }
};

#endif
