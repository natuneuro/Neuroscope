/***************************************************************************
				cerebustracesprovider.h  -  description
                             -------------------
    copyright            : (C) 2015 by Florian Franzen
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CEREBUSTRACESPROVIDER_H
#define CEREBUSTRACESPROVIDER_H

// Include Qt Library files
#include <QObject>
#include <QMutex>
#include <QtDebug>

// Include cerebus sdk
#include <cbsdk.h>

// Include project files
#include "types.h"
#include "tracesprovider.h"
#include "clustersprovider.h"
#include "eventsprovider.h"


/** CerebusTracesProvider uses a Blackrock Cerebus NSP as data source.
  *
  * Continous recorded channels are grouped into so called sampling groups based
  * on their sampling rate. This class allows you to use one of these sampling
  * group as data source for traces.
  *
  * If the number of channels in the sampling group change, the is currently no
  * way to let the GUI know.
  *
  * @author Florian Franzen
  */

class CerebusTracesProvider : public TracesProvider  {
    Q_OBJECT

public:
    // Callback that reacts to new data
    static void packageCallback(UINT32 instance, const cbSdkPktType type,  const void* data, void* object);

    // Different sampling groups one can describe to
	enum SamplingGroup {
		RATE_500 = 1,
		RATE_1K = 2,
		RATE_2K = 3,
		RATE_10k = 4,
		RATE_30k = 5
	};

    /**
    * @param group is the sampling group to subscribe to.
    */
	CerebusTracesProvider(SamplingGroup group);
	virtual ~CerebusTracesProvider();

    /** Initializes the object by trying to connect to the Cerebus NSP.
    *
    * Fails on connection error or when there are no channels in sampling group.
    */
    bool init();

    /** Returns true if initialized */
    bool isInitialized() {
        return mInitialized;
    }

    /**Computes the number of samples between @p start and @p end.
    * @param start begining of the time frame from which the data have been retrieved, given in milisecond.
    * @param end end of the time frame from which to retrieve the data, given in milisecond.
    * @return number of samples in the given time frame.
    * @param startTimeInRecordingUnits begining of the time frame from which the data have been retrieved, given in recording units.
    */
    virtual long getNbSamples(long start, long end, long startInRecordingUnits);

    // Called by callback to add data to buffer.
    void processData(const cbPKT_GROUP* package);

    // Called by callback to add spike event to buffer.
    void processSpike(const cbPKT_SPK* package);

	// Called by callback to add digital and serial event to buffer.
	void processEvent(const cbPKT_DINP* package);

    // Called by callback to process configuration changes.
    void processConfig(const cbPKT_GROUPINFO* package);

	// Return last error message as string
	std::string getLastErrorMessage();

    /** Dummy function, definded to work around the bad interface design.
    * @param nb the number of channels.
    */
    virtual void setNbChannels(int nb){
        qDebug() << "Cerebus NSP used. Ignoring setNbChannels(" << nb << ")";
    }

    /** Dummy function, definded to work around the bad interface design.
    * @param res resolution.
    */
    virtual void setResolution(int res){
        qDebug() << "Cerebus NSP used.  Ignoring setResolution(" << res << ")";
    }

    /** Dummy function, definded to work around the bad interface design.
    * @param rate the sampling rate.
    */
    virtual void setSamplingRate(double rate){
        qDebug() << "Cerebus NSP used. Ignoring setSamplingRate(" << rate << ")";
    }

    /** Dummy function, definded to work around the bad interface design.
    * @param range the voltage range.
    */
    virtual void setVoltageRange(int range){
        qDebug() << "Cerebus NSP used. Ignoring setVoltageRange(" << range << ")";
    }

    /**  Dummy function, definded to work around the bad interface design.
    * @param value the amplification.
    */
    virtual void setAmplification(int value){
        qDebug() << "Cerebus NSP used. Ignoring setAmplification(" << value << ")";
    }

    /** Return the labels of each channel as read from cerebus config. */
    virtual QStringList getLabels();

    /** Called when paging is started.
     *  Recouples the buffer that is updated with the one that is
     *  viewed/returened.
     *  This essentially unpauses the signal being played and shows the
     *  live signal again.
     */
    virtual void slotPagingStarted();

    /** Called when paging is stopped.
     * Decouples the buffer that is viewed/returned from the one updated.
     * This essentialy pauses the signal that is being displayed.
     */
    virtual void slotPagingStopped();

    /** Create a cluster provider for each channel.
     */
    QList<ClustersProvider*> getClusterProviders();

    /** Get cluster data for specific channel in time between @start and @end.
     */
    Array<dataType>* getClusterData(unsigned int channel, long start, long end);

    /** Create a event provider for all event data.
     */
    EventsProvider* getEventProvider();

    /** Get event data for all events between @start and @end.
     */
    Array<dataType>* getEventData(long start, long end);

Q_SIGNALS:
    /**Signals that the data have been retrieved.
    * @param data array of data in uV (number of channels X number of samples).
    * @param initiator instance requesting the data.
    */
    void dataReady(Array<dataType>& data, QObject* initiator);

private:
    // Resolution of data packages received.
    static const int CEREBUS_RESOLUTION ;
    // Default instance id to use to talk to CB SDK
    static const unsigned int CEREBUS_INSTANCE;
    // Length of buffer in seconds (for events it is assumed there is an event for every tick in that second)
    static const unsigned int BUFFER_SIZE;
    // Sampling rate of each sampling group
    static const unsigned int SAMPLING_RATES[6];

    // True if connection, buffers and callback were initialized
    bool mInitialized;
	// True if set of channels in sampling group were changed
	bool mReconfigured;

    // Sampling group to listen to
    SamplingGroup mGroup;
    // List of NSP channel numbers we are listing to
    UINT16* mChannels;

    // List of scalings for each channel
    cbSCALING* mScales;
    // List of labels for each channel
    QStringList mLabels;

    // Return value of last CBSDK library call
    int mLastResult;

    // Data storage mutex
    QMutex mMutex;

    // Latest NSP system clock value
    UINT32* mLiveTime;
    UINT32* mViewTime;

    // Capacity of buffers
    size_t mTraceCapacity;
    size_t mEventCapacity;

    // Continous data storage
    INT16*  mLiveTraceData;
    size_t* mLiveTracePosition;
    INT16*  mViewTraceData;
    size_t* mViewTracePosition;

    // Spike event data storage
    UINT32** mLiveClusterTime;
    UINT8**  mLiveClusterID;
    size_t** mLiveClusterPosition;
    UINT32** mViewClusterTime;
    UINT8**  mViewClusterID;
    size_t** mViewClusterPosition;

    // Digital and serial event data storage
    UINT32* mLiveEventTime;
    UINT16* mLiveEventID;
    size_t* mLiveEventPosition;
    UINT32* mViewEventTime;
    UINT16* mViewEventID;
    size_t* mViewEventPosition;

    /**Retrieves the traces included in the time frame given by @p startTime and @p endTime.
    * @param startTime begining of the time frame from which to retrieve the data, given in milisecond.
    * @param endTime end of the time frame from which to retrieve the data, given in milisecond.
    * @param initiator instance requesting the data.
    * @param startTimeInRecordingUnits begining of the time interval from which to retrieve the data in recording units.
    */
    virtual void retrieveData(long startTime, long endTime, QObject* initiator, long startTimeInRecordingUnits);

    /**Computes the total length of the document in miliseconds.*/
    virtual void computeRecordingLength();

    /** Helper function that searches timestamps in buffer*/
    template <typename T>
    Array<dataType>* getTimeStampedData(UINT32* timeBuffer,
                                        T* dataBuffer,
                                        size_t* bufferPosition,
                                        long start,
                                        long end);
};

#endif
