// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `LCU.ice'

#include <LCU.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __OUC__LCU__sayHello_name = "sayHello";

static const ::std::string __OUC__LCU__shutdown_name = "shutdown";

static const ::std::string __OUC__LCU__getEncodersPosition_name = "getEncodersPosition";

static const ::std::string __OUC__LCU__getRawEncodersPosition_name = "getRawEncodersPosition";

static const ::std::string __OUC__LCU__getConfiguration_name = "getConfiguration";

static const ::std::string __OUC__LCU__getPosition_name = "getPosition";

static const ::std::string __OUC__LCU__isConfigured_name = "isConfigured";

static const ::std::string __OUC__LCU__isTracking_name = "isTracking";

static const ::std::string __OUC__LCU__setConfiguration_name = "setConfiguration";

static const ::std::string __OUC__LCU__setTarget_name = "setTarget";

static const ::std::string __OUC__LCU__setOffset_name = "setOffset";

static const ::std::string __OUC__LCU__setTracking_name = "setTracking";

static const ::std::string __OUC__LCU__parkTelescope_name = "parkTelescope";

static const ::std::string __OUC__LCU__stopTelescope_name = "stopTelescope";

static const ::std::string __OUC__LCU__moveToTarget_name = "moveToTarget";

::Ice::Object* IceInternal::upCast(::OUC::LCU* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::OUC::LCU* p) { return p; }

void
OUC::__read(::IceInternal::BasicStream* __is, ::OUC::LCUPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::OUC::LCU;
        v->__copyFrom(proxy);
    }
}

void
OUC::__write(::IceInternal::BasicStream* __os, ::OUC::TelescopeDirection v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 4);
}

void
OUC::__read(::IceInternal::BasicStream* __is, ::OUC::TelescopeDirection& v)
{
    ::Ice::Byte val;
    __is->read(val, 4);
    v = static_cast< ::OUC::TelescopeDirection>(val);
}

bool
OUC::RawEncoderData::operator==(const RawEncoderData& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(lectAlphaWormE != __rhs.lectAlphaWormE)
    {
        return false;
    }
    if(lectAlphaAxisE != __rhs.lectAlphaAxisE)
    {
        return false;
    }
    if(lectAlphaMotor != __rhs.lectAlphaMotor)
    {
        return false;
    }
    if(posAlphaWormE != __rhs.posAlphaWormE)
    {
        return false;
    }
    if(posAlphaAxisE != __rhs.posAlphaAxisE)
    {
        return false;
    }
    if(posAlphaMotor != __rhs.posAlphaMotor)
    {
        return false;
    }
    if(lectDeltaWormE != __rhs.lectDeltaWormE)
    {
        return false;
    }
    if(lectDeltaAxisE != __rhs.lectDeltaAxisE)
    {
        return false;
    }
    if(lectDeltaMotor != __rhs.lectDeltaMotor)
    {
        return false;
    }
    if(posDeltaWormE != __rhs.posDeltaWormE)
    {
        return false;
    }
    if(posDeltaAxisE != __rhs.posDeltaAxisE)
    {
        return false;
    }
    if(posDeltaMotor != __rhs.posDeltaMotor)
    {
        return false;
    }
    if(deltaT != __rhs.deltaT)
    {
        return false;
    }
    if(lcuTime != __rhs.lcuTime)
    {
        return false;
    }
    return true;
}

bool
OUC::RawEncoderData::operator<(const RawEncoderData& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(lectAlphaWormE < __rhs.lectAlphaWormE)
    {
        return true;
    }
    else if(__rhs.lectAlphaWormE < lectAlphaWormE)
    {
        return false;
    }
    if(lectAlphaAxisE < __rhs.lectAlphaAxisE)
    {
        return true;
    }
    else if(__rhs.lectAlphaAxisE < lectAlphaAxisE)
    {
        return false;
    }
    if(lectAlphaMotor < __rhs.lectAlphaMotor)
    {
        return true;
    }
    else if(__rhs.lectAlphaMotor < lectAlphaMotor)
    {
        return false;
    }
    if(posAlphaWormE < __rhs.posAlphaWormE)
    {
        return true;
    }
    else if(__rhs.posAlphaWormE < posAlphaWormE)
    {
        return false;
    }
    if(posAlphaAxisE < __rhs.posAlphaAxisE)
    {
        return true;
    }
    else if(__rhs.posAlphaAxisE < posAlphaAxisE)
    {
        return false;
    }
    if(posAlphaMotor < __rhs.posAlphaMotor)
    {
        return true;
    }
    else if(__rhs.posAlphaMotor < posAlphaMotor)
    {
        return false;
    }
    if(lectDeltaWormE < __rhs.lectDeltaWormE)
    {
        return true;
    }
    else if(__rhs.lectDeltaWormE < lectDeltaWormE)
    {
        return false;
    }
    if(lectDeltaAxisE < __rhs.lectDeltaAxisE)
    {
        return true;
    }
    else if(__rhs.lectDeltaAxisE < lectDeltaAxisE)
    {
        return false;
    }
    if(lectDeltaMotor < __rhs.lectDeltaMotor)
    {
        return true;
    }
    else if(__rhs.lectDeltaMotor < lectDeltaMotor)
    {
        return false;
    }
    if(posDeltaWormE < __rhs.posDeltaWormE)
    {
        return true;
    }
    else if(__rhs.posDeltaWormE < posDeltaWormE)
    {
        return false;
    }
    if(posDeltaAxisE < __rhs.posDeltaAxisE)
    {
        return true;
    }
    else if(__rhs.posDeltaAxisE < posDeltaAxisE)
    {
        return false;
    }
    if(posDeltaMotor < __rhs.posDeltaMotor)
    {
        return true;
    }
    else if(__rhs.posDeltaMotor < posDeltaMotor)
    {
        return false;
    }
    if(deltaT < __rhs.deltaT)
    {
        return true;
    }
    else if(__rhs.deltaT < deltaT)
    {
        return false;
    }
    if(lcuTime < __rhs.lcuTime)
    {
        return true;
    }
    else if(__rhs.lcuTime < lcuTime)
    {
        return false;
    }
    return false;
}

void
OUC::RawEncoderData::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(lectAlphaWormE);
    __os->write(lectAlphaAxisE);
    __os->write(lectAlphaMotor);
    __os->write(posAlphaWormE);
    __os->write(posAlphaAxisE);
    __os->write(posAlphaMotor);
    __os->write(lectDeltaWormE);
    __os->write(lectDeltaAxisE);
    __os->write(lectDeltaMotor);
    __os->write(posDeltaWormE);
    __os->write(posDeltaAxisE);
    __os->write(posDeltaMotor);
    __os->write(deltaT);
    __os->write(lcuTime);
}

void
OUC::RawEncoderData::__read(::IceInternal::BasicStream* __is)
{
    __is->read(lectAlphaWormE);
    __is->read(lectAlphaAxisE);
    __is->read(lectAlphaMotor);
    __is->read(posAlphaWormE);
    __is->read(posAlphaAxisE);
    __is->read(posAlphaMotor);
    __is->read(lectDeltaWormE);
    __is->read(lectDeltaAxisE);
    __is->read(lectDeltaMotor);
    __is->read(posDeltaWormE);
    __is->read(posDeltaAxisE);
    __is->read(posDeltaMotor);
    __is->read(deltaT);
    __is->read(lcuTime);
}

bool
OUC::EncoderData::operator==(const EncoderData& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(alphaWormE != __rhs.alphaWormE)
    {
        return false;
    }
    if(alphaAxisE != __rhs.alphaAxisE)
    {
        return false;
    }
    if(deltaWormE != __rhs.deltaWormE)
    {
        return false;
    }
    if(deltaAxisE != __rhs.deltaAxisE)
    {
        return false;
    }
    if(localTime != __rhs.localTime)
    {
        return false;
    }
    if(lcuTime != __rhs.lcuTime)
    {
        return false;
    }
    return true;
}

bool
OUC::EncoderData::operator<(const EncoderData& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(alphaWormE < __rhs.alphaWormE)
    {
        return true;
    }
    else if(__rhs.alphaWormE < alphaWormE)
    {
        return false;
    }
    if(alphaAxisE < __rhs.alphaAxisE)
    {
        return true;
    }
    else if(__rhs.alphaAxisE < alphaAxisE)
    {
        return false;
    }
    if(deltaWormE < __rhs.deltaWormE)
    {
        return true;
    }
    else if(__rhs.deltaWormE < deltaWormE)
    {
        return false;
    }
    if(deltaAxisE < __rhs.deltaAxisE)
    {
        return true;
    }
    else if(__rhs.deltaAxisE < deltaAxisE)
    {
        return false;
    }
    if(localTime < __rhs.localTime)
    {
        return true;
    }
    else if(__rhs.localTime < localTime)
    {
        return false;
    }
    if(lcuTime < __rhs.lcuTime)
    {
        return true;
    }
    else if(__rhs.lcuTime < lcuTime)
    {
        return false;
    }
    return false;
}

void
OUC::EncoderData::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(alphaWormE);
    __os->write(alphaAxisE);
    __os->write(deltaWormE);
    __os->write(deltaAxisE);
    __os->write(localTime);
    __os->write(lcuTime);
}

void
OUC::EncoderData::__read(::IceInternal::BasicStream* __is)
{
    __is->read(alphaWormE);
    __is->read(alphaAxisE);
    __is->read(deltaWormE);
    __is->read(deltaAxisE);
    __is->read(localTime);
    __is->read(lcuTime);
}

bool
OUC::TrackingInfo::operator==(const TrackingInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(trackState != __rhs.trackState)
    {
        return false;
    }
    if(degVel != __rhs.degVel)
    {
        return false;
    }
    if(ticVel != __rhs.ticVel)
    {
        return false;
    }
    return true;
}

bool
OUC::TrackingInfo::operator<(const TrackingInfo& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(trackState < __rhs.trackState)
    {
        return true;
    }
    else if(__rhs.trackState < trackState)
    {
        return false;
    }
    if(degVel < __rhs.degVel)
    {
        return true;
    }
    else if(__rhs.degVel < degVel)
    {
        return false;
    }
    if(ticVel < __rhs.ticVel)
    {
        return true;
    }
    else if(__rhs.ticVel < ticVel)
    {
        return false;
    }
    return false;
}

void
OUC::TrackingInfo::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(trackState);
    __os->write(degVel);
    __os->write(ticVel);
}

void
OUC::TrackingInfo::__read(::IceInternal::BasicStream* __is)
{
    __is->read(trackState);
    __is->read(degVel);
    __is->read(ticVel);
}

bool
OUC::TelescopeConfigData::operator==(const TelescopeConfigData& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(localTime != __rhs.localTime)
    {
        return false;
    }
    if(lcuTime != __rhs.lcuTime)
    {
        return false;
    }
    if(latitude != __rhs.latitude)
    {
        return false;
    }
    if(longitude != __rhs.longitude)
    {
        return false;
    }
    if(altitude != __rhs.altitude)
    {
        return false;
    }
    if(AMT != __rhs.AMT)
    {
        return false;
    }
    if(AMH != __rhs.AMH)
    {
        return false;
    }
    if(AMR != __rhs.AMR)
    {
        return false;
    }
    if(AWT != __rhs.AWT)
    {
        return false;
    }
    if(AWH != __rhs.AWH)
    {
        return false;
    }
    if(AWR != __rhs.AWR)
    {
        return false;
    }
    if(AAT != __rhs.AAT)
    {
        return false;
    }
    if(AAH != __rhs.AAH)
    {
        return false;
    }
    if(AAR != __rhs.AAR)
    {
        return false;
    }
    if(DMT != __rhs.DMT)
    {
        return false;
    }
    if(DMH != __rhs.DMH)
    {
        return false;
    }
    if(DMR != __rhs.DMR)
    {
        return false;
    }
    if(DWT != __rhs.DWT)
    {
        return false;
    }
    if(DWH != __rhs.DWH)
    {
        return false;
    }
    if(DWR != __rhs.DWR)
    {
        return false;
    }
    if(DAT != __rhs.DAT)
    {
        return false;
    }
    if(DAH != __rhs.DAH)
    {
        return false;
    }
    if(DAR != __rhs.DAR)
    {
        return false;
    }
    return true;
}

bool
OUC::TelescopeConfigData::operator<(const TelescopeConfigData& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(localTime < __rhs.localTime)
    {
        return true;
    }
    else if(__rhs.localTime < localTime)
    {
        return false;
    }
    if(lcuTime < __rhs.lcuTime)
    {
        return true;
    }
    else if(__rhs.lcuTime < lcuTime)
    {
        return false;
    }
    if(latitude < __rhs.latitude)
    {
        return true;
    }
    else if(__rhs.latitude < latitude)
    {
        return false;
    }
    if(longitude < __rhs.longitude)
    {
        return true;
    }
    else if(__rhs.longitude < longitude)
    {
        return false;
    }
    if(altitude < __rhs.altitude)
    {
        return true;
    }
    else if(__rhs.altitude < altitude)
    {
        return false;
    }
    if(AMT < __rhs.AMT)
    {
        return true;
    }
    else if(__rhs.AMT < AMT)
    {
        return false;
    }
    if(AMH < __rhs.AMH)
    {
        return true;
    }
    else if(__rhs.AMH < AMH)
    {
        return false;
    }
    if(AMR < __rhs.AMR)
    {
        return true;
    }
    else if(__rhs.AMR < AMR)
    {
        return false;
    }
    if(AWT < __rhs.AWT)
    {
        return true;
    }
    else if(__rhs.AWT < AWT)
    {
        return false;
    }
    if(AWH < __rhs.AWH)
    {
        return true;
    }
    else if(__rhs.AWH < AWH)
    {
        return false;
    }
    if(AWR < __rhs.AWR)
    {
        return true;
    }
    else if(__rhs.AWR < AWR)
    {
        return false;
    }
    if(AAT < __rhs.AAT)
    {
        return true;
    }
    else if(__rhs.AAT < AAT)
    {
        return false;
    }
    if(AAH < __rhs.AAH)
    {
        return true;
    }
    else if(__rhs.AAH < AAH)
    {
        return false;
    }
    if(AAR < __rhs.AAR)
    {
        return true;
    }
    else if(__rhs.AAR < AAR)
    {
        return false;
    }
    if(DMT < __rhs.DMT)
    {
        return true;
    }
    else if(__rhs.DMT < DMT)
    {
        return false;
    }
    if(DMH < __rhs.DMH)
    {
        return true;
    }
    else if(__rhs.DMH < DMH)
    {
        return false;
    }
    if(DMR < __rhs.DMR)
    {
        return true;
    }
    else if(__rhs.DMR < DMR)
    {
        return false;
    }
    if(DWT < __rhs.DWT)
    {
        return true;
    }
    else if(__rhs.DWT < DWT)
    {
        return false;
    }
    if(DWH < __rhs.DWH)
    {
        return true;
    }
    else if(__rhs.DWH < DWH)
    {
        return false;
    }
    if(DWR < __rhs.DWR)
    {
        return true;
    }
    else if(__rhs.DWR < DWR)
    {
        return false;
    }
    if(DAT < __rhs.DAT)
    {
        return true;
    }
    else if(__rhs.DAT < DAT)
    {
        return false;
    }
    if(DAH < __rhs.DAH)
    {
        return true;
    }
    else if(__rhs.DAH < DAH)
    {
        return false;
    }
    if(DAR < __rhs.DAR)
    {
        return true;
    }
    else if(__rhs.DAR < DAR)
    {
        return false;
    }
    return false;
}

void
OUC::TelescopeConfigData::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(localTime);
    __os->write(lcuTime);
    __os->write(latitude);
    __os->write(longitude);
    __os->write(altitude);
    __os->write(AMT);
    __os->write(AMH);
    __os->write(AMR);
    __os->write(AWT);
    __os->write(AWH);
    __os->write(AWR);
    __os->write(AAT);
    __os->write(AAH);
    __os->write(AAR);
    __os->write(DMT);
    __os->write(DMH);
    __os->write(DMR);
    __os->write(DWT);
    __os->write(DWH);
    __os->write(DWR);
    __os->write(DAT);
    __os->write(DAH);
    __os->write(DAR);
}

void
OUC::TelescopeConfigData::__read(::IceInternal::BasicStream* __is)
{
    __is->read(localTime);
    __is->read(lcuTime);
    __is->read(latitude);
    __is->read(longitude);
    __is->read(altitude);
    __is->read(AMT);
    __is->read(AMH);
    __is->read(AMR);
    __is->read(AWT);
    __is->read(AWH);
    __is->read(AWR);
    __is->read(AAT);
    __is->read(AAH);
    __is->read(AAR);
    __is->read(DMT);
    __is->read(DMH);
    __is->read(DMR);
    __is->read(DWT);
    __is->read(DWH);
    __is->read(DWR);
    __is->read(DAT);
    __is->read(DAH);
    __is->read(DAR);
}

bool
OUC::TelescopePosition::operator==(const TelescopePosition& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(localSideralTime != __rhs.localSideralTime)
    {
        return false;
    }
    if(RA != __rhs.RA)
    {
        return false;
    }
    if(HA != __rhs.HA)
    {
        return false;
    }
    if(Dec != __rhs.Dec)
    {
        return false;
    }
    if(Alt != __rhs.Alt)
    {
        return false;
    }
    if(Az != __rhs.Az)
    {
        return false;
    }
    return true;
}

bool
OUC::TelescopePosition::operator<(const TelescopePosition& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(localSideralTime < __rhs.localSideralTime)
    {
        return true;
    }
    else if(__rhs.localSideralTime < localSideralTime)
    {
        return false;
    }
    if(RA < __rhs.RA)
    {
        return true;
    }
    else if(__rhs.RA < RA)
    {
        return false;
    }
    if(HA < __rhs.HA)
    {
        return true;
    }
    else if(__rhs.HA < HA)
    {
        return false;
    }
    if(Dec < __rhs.Dec)
    {
        return true;
    }
    else if(__rhs.Dec < Dec)
    {
        return false;
    }
    if(Alt < __rhs.Alt)
    {
        return true;
    }
    else if(__rhs.Alt < Alt)
    {
        return false;
    }
    if(Az < __rhs.Az)
    {
        return true;
    }
    else if(__rhs.Az < Az)
    {
        return false;
    }
    return false;
}

void
OUC::TelescopePosition::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(localSideralTime);
    __os->write(RA);
    __os->write(HA);
    __os->write(Dec);
    __os->write(Alt);
    __os->write(Az);
}

void
OUC::TelescopePosition::__read(::IceInternal::BasicStream* __is)
{
    __is->read(localSideralTime);
    __is->read(RA);
    __is->read(HA);
    __is->read(Dec);
    __is->read(Alt);
    __is->read(Az);
}

bool
OUC::TelescopeData::operator==(const TelescopeData& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(localTime != __rhs.localTime)
    {
        return false;
    }
    if(lcuTime != __rhs.lcuTime)
    {
        return false;
    }
    if(deltaT != __rhs.deltaT)
    {
        return false;
    }
    if(localSideralTime != __rhs.localSideralTime)
    {
        return false;
    }
    if(julianDate != __rhs.julianDate)
    {
        return false;
    }
    if(latitude != __rhs.latitude)
    {
        return false;
    }
    if(longitude != __rhs.longitude)
    {
        return false;
    }
    if(altitude != __rhs.altitude)
    {
        return false;
    }
    if(lowElevation != __rhs.lowElevation)
    {
        return false;
    }
    if(highElevation != __rhs.highElevation)
    {
        return false;
    }
    if(targetName != __rhs.targetName)
    {
        return false;
    }
    if(currentPos != __rhs.currentPos)
    {
        return false;
    }
    if(targetPos != __rhs.targetPos)
    {
        return false;
    }
    if(differencePos != __rhs.differencePos)
    {
        return false;
    }
    return true;
}

bool
OUC::TelescopeData::operator<(const TelescopeData& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(localTime < __rhs.localTime)
    {
        return true;
    }
    else if(__rhs.localTime < localTime)
    {
        return false;
    }
    if(lcuTime < __rhs.lcuTime)
    {
        return true;
    }
    else if(__rhs.lcuTime < lcuTime)
    {
        return false;
    }
    if(deltaT < __rhs.deltaT)
    {
        return true;
    }
    else if(__rhs.deltaT < deltaT)
    {
        return false;
    }
    if(localSideralTime < __rhs.localSideralTime)
    {
        return true;
    }
    else if(__rhs.localSideralTime < localSideralTime)
    {
        return false;
    }
    if(julianDate < __rhs.julianDate)
    {
        return true;
    }
    else if(__rhs.julianDate < julianDate)
    {
        return false;
    }
    if(latitude < __rhs.latitude)
    {
        return true;
    }
    else if(__rhs.latitude < latitude)
    {
        return false;
    }
    if(longitude < __rhs.longitude)
    {
        return true;
    }
    else if(__rhs.longitude < longitude)
    {
        return false;
    }
    if(altitude < __rhs.altitude)
    {
        return true;
    }
    else if(__rhs.altitude < altitude)
    {
        return false;
    }
    if(lowElevation < __rhs.lowElevation)
    {
        return true;
    }
    else if(__rhs.lowElevation < lowElevation)
    {
        return false;
    }
    if(highElevation < __rhs.highElevation)
    {
        return true;
    }
    else if(__rhs.highElevation < highElevation)
    {
        return false;
    }
    if(targetName < __rhs.targetName)
    {
        return true;
    }
    else if(__rhs.targetName < targetName)
    {
        return false;
    }
    if(currentPos < __rhs.currentPos)
    {
        return true;
    }
    else if(__rhs.currentPos < currentPos)
    {
        return false;
    }
    if(targetPos < __rhs.targetPos)
    {
        return true;
    }
    else if(__rhs.targetPos < targetPos)
    {
        return false;
    }
    if(differencePos < __rhs.differencePos)
    {
        return true;
    }
    else if(__rhs.differencePos < differencePos)
    {
        return false;
    }
    return false;
}

void
OUC::TelescopeData::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(localTime);
    __os->write(lcuTime);
    __os->write(deltaT);
    __os->write(localSideralTime);
    __os->write(julianDate);
    __os->write(latitude);
    __os->write(longitude);
    __os->write(altitude);
    __os->write(lowElevation);
    __os->write(highElevation);
    __os->write(targetName);
    currentPos.__write(__os);
    targetPos.__write(__os);
    differencePos.__write(__os);
}

void
OUC::TelescopeData::__read(::IceInternal::BasicStream* __is)
{
    __is->read(localTime);
    __is->read(lcuTime);
    __is->read(deltaT);
    __is->read(localSideralTime);
    __is->read(julianDate);
    __is->read(latitude);
    __is->read(longitude);
    __is->read(altitude);
    __is->read(lowElevation);
    __is->read(highElevation);
    __is->read(targetName);
    currentPos.__read(__is);
    targetPos.__read(__is);
    differencePos.__read(__is);
}

OUC::ErrorEx::ErrorEx(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    reason(__ice_reason)
{
}

OUC::ErrorEx::~ErrorEx() throw()
{
}

static const char* __OUC__ErrorEx_name = "OUC::ErrorEx";

::std::string
OUC::ErrorEx::ice_name() const
{
    return __OUC__ErrorEx_name;
}

::Ice::Exception*
OUC::ErrorEx::ice_clone() const
{
    return new ErrorEx(*this);
}

void
OUC::ErrorEx::ice_throw() const
{
    throw *this;
}

void
OUC::ErrorEx::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::OUC::ErrorEx"), false);
    __os->startWriteSlice();
    __os->write(reason);
    __os->endWriteSlice();
}

void
OUC::ErrorEx::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(reason);
    __is->endReadSlice();
}

void
OUC::ErrorEx::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::ErrorEx was not generated with stream support";
    throw ex;
}

void
OUC::ErrorEx::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::ErrorEx was not generated with stream support";
    throw ex;
}

struct __F__OUC__ErrorEx : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::OUC::ErrorEx();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__OUC__ErrorEx__Ptr = new __F__OUC__ErrorEx;

const ::IceInternal::UserExceptionFactoryPtr&
OUC::ErrorEx::ice_factory()
{
    return __F__OUC__ErrorEx__Ptr;
}

class __F__OUC__ErrorEx__Init
{
public:

    __F__OUC__ErrorEx__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::OUC::ErrorEx", ::OUC::ErrorEx::ice_factory());
    }

    ~__F__OUC__ErrorEx__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::OUC::ErrorEx");
    }
};

static __F__OUC__ErrorEx__Init __F__OUC__ErrorEx__i;

#ifdef __APPLE__
extern "C" { void __F__OUC__ErrorEx__initializer() {} }
#endif

OUC::TelescopeNotConfiguredEx::TelescopeNotConfiguredEx(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx(__ice_reason)
#else
    ::OUC::ErrorEx(__ice_reason)
#endif
{
}

OUC::TelescopeNotConfiguredEx::~TelescopeNotConfiguredEx() throw()
{
}

static const char* __OUC__TelescopeNotConfiguredEx_name = "OUC::TelescopeNotConfiguredEx";

::std::string
OUC::TelescopeNotConfiguredEx::ice_name() const
{
    return __OUC__TelescopeNotConfiguredEx_name;
}

::Ice::Exception*
OUC::TelescopeNotConfiguredEx::ice_clone() const
{
    return new TelescopeNotConfiguredEx(*this);
}

void
OUC::TelescopeNotConfiguredEx::ice_throw() const
{
    throw *this;
}

void
OUC::TelescopeNotConfiguredEx::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::OUC::TelescopeNotConfiguredEx"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx::__write(__os);
#else
    ::OUC::ErrorEx::__write(__os);
#endif
}

void
OUC::TelescopeNotConfiguredEx::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx::__read(__is, true);
#else
    ::OUC::ErrorEx::__read(__is, true);
#endif
}

void
OUC::TelescopeNotConfiguredEx::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::TelescopeNotConfiguredEx was not generated with stream support";
    throw ex;
}

void
OUC::TelescopeNotConfiguredEx::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::TelescopeNotConfiguredEx was not generated with stream support";
    throw ex;
}

struct __F__OUC__TelescopeNotConfiguredEx : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::OUC::TelescopeNotConfiguredEx();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__OUC__TelescopeNotConfiguredEx__Ptr = new __F__OUC__TelescopeNotConfiguredEx;

const ::IceInternal::UserExceptionFactoryPtr&
OUC::TelescopeNotConfiguredEx::ice_factory()
{
    return __F__OUC__TelescopeNotConfiguredEx__Ptr;
}

class __F__OUC__TelescopeNotConfiguredEx__Init
{
public:

    __F__OUC__TelescopeNotConfiguredEx__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::OUC::TelescopeNotConfiguredEx", ::OUC::TelescopeNotConfiguredEx::ice_factory());
    }

    ~__F__OUC__TelescopeNotConfiguredEx__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::OUC::TelescopeNotConfiguredEx");
    }
};

static __F__OUC__TelescopeNotConfiguredEx__Init __F__OUC__TelescopeNotConfiguredEx__i;

#ifdef __APPLE__
extern "C" { void __F__OUC__TelescopeNotConfiguredEx__initializer() {} }
#endif

OUC::TargetOutOfLimitsEx::TargetOutOfLimitsEx(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx(__ice_reason)
#else
    ::OUC::ErrorEx(__ice_reason)
#endif
{
}

OUC::TargetOutOfLimitsEx::~TargetOutOfLimitsEx() throw()
{
}

static const char* __OUC__TargetOutOfLimitsEx_name = "OUC::TargetOutOfLimitsEx";

::std::string
OUC::TargetOutOfLimitsEx::ice_name() const
{
    return __OUC__TargetOutOfLimitsEx_name;
}

::Ice::Exception*
OUC::TargetOutOfLimitsEx::ice_clone() const
{
    return new TargetOutOfLimitsEx(*this);
}

void
OUC::TargetOutOfLimitsEx::ice_throw() const
{
    throw *this;
}

void
OUC::TargetOutOfLimitsEx::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::OUC::TargetOutOfLimitsEx"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx::__write(__os);
#else
    ::OUC::ErrorEx::__write(__os);
#endif
}

void
OUC::TargetOutOfLimitsEx::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx::__read(__is, true);
#else
    ::OUC::ErrorEx::__read(__is, true);
#endif
}

void
OUC::TargetOutOfLimitsEx::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::TargetOutOfLimitsEx was not generated with stream support";
    throw ex;
}

void
OUC::TargetOutOfLimitsEx::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::TargetOutOfLimitsEx was not generated with stream support";
    throw ex;
}

struct __F__OUC__TargetOutOfLimitsEx : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::OUC::TargetOutOfLimitsEx();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__OUC__TargetOutOfLimitsEx__Ptr = new __F__OUC__TargetOutOfLimitsEx;

const ::IceInternal::UserExceptionFactoryPtr&
OUC::TargetOutOfLimitsEx::ice_factory()
{
    return __F__OUC__TargetOutOfLimitsEx__Ptr;
}

class __F__OUC__TargetOutOfLimitsEx__Init
{
public:

    __F__OUC__TargetOutOfLimitsEx__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::OUC::TargetOutOfLimitsEx", ::OUC::TargetOutOfLimitsEx::ice_factory());
    }

    ~__F__OUC__TargetOutOfLimitsEx__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::OUC::TargetOutOfLimitsEx");
    }
};

static __F__OUC__TargetOutOfLimitsEx__Init __F__OUC__TargetOutOfLimitsEx__i;

#ifdef __APPLE__
extern "C" { void __F__OUC__TargetOutOfLimitsEx__initializer() {} }
#endif

void
IceProxy::OUC::LCU::sayHello(::Ice::Int delay, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->sayHello(delay, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::shutdown(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->shutdown(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::OUC::EncoderData
IceProxy::OUC::LCU::getEncodersPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__getEncodersPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            return __del->getEncodersPosition(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::OUC::RawEncoderData
IceProxy::OUC::LCU::getRawEncodersPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__getRawEncodersPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            return __del->getRawEncodersPosition(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::OUC::TelescopeConfigData
IceProxy::OUC::LCU::getConfiguration(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__getConfiguration_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            return __del->getConfiguration(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::OUC::TelescopeData
IceProxy::OUC::LCU::getPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__getPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            return __del->getPosition(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::OUC::LCU::isConfigured(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__isConfigured_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            return __del->isConfigured(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::OUC::LCU::isTracking(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__isTracking_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            return __del->isTracking(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->setConfiguration(fileName, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__setTarget_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->setTarget(targetPos, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__setOffset_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->setOffset(offsetPos, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__setTracking_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->setTracking(trkInfo, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::parkTelescope(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__parkTelescope_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->parkTelescope(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__stopTelescope_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->stopTelescope(dir, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::OUC::LCU::moveToTarget(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__LCU__moveToTarget_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::LCU* __del = dynamic_cast< ::IceDelegate::OUC::LCU*>(__delBase.get());
            __del->moveToTarget(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::OUC::LCU::ice_staticId()
{
    return ::OUC::LCU::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::OUC::LCU::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::OUC::LCU);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::OUC::LCU::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::OUC::LCU);
}

::IceProxy::Ice::Object*
IceProxy::OUC::LCU::__newInstance() const
{
    return new LCU;
}

void
IceDelegateM::OUC::LCU::sayHello(::Ice::Int delay, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__sayHello_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(delay);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::OUC::LCU::shutdown(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__shutdown_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::OUC::EncoderData
IceDelegateM::OUC::LCU::getEncodersPosition(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__getEncodersPosition_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::OUC::EncoderData __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::OUC::RawEncoderData
IceDelegateM::OUC::LCU::getRawEncodersPosition(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__getRawEncodersPosition_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::OUC::RawEncoderData __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::OUC::TelescopeConfigData
IceDelegateM::OUC::LCU::getConfiguration(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__getConfiguration_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::OUC::TelescopeConfigData __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::OUC::TelescopeData
IceDelegateM::OUC::LCU::getPosition(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__getPosition_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::OUC::TelescopeData __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::OUC::LCU::isConfigured(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__isConfigured_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::OUC::LCU::isTracking(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__isTracking_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::LCU::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__setConfiguration_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(fileName);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::OUC::LCU::setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__setTarget_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        targetPos.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TargetOutOfLimitsEx&)
            {
                throw;
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::LCU::setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__setOffset_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        offsetPos.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TargetOutOfLimitsEx&)
            {
                throw;
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::LCU::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__setTracking_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        trkInfo.__read(__is);
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::LCU::parkTelescope(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__parkTelescope_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::LCU::stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__stopTelescope_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::OUC::__write(__os, dir);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::LCU::moveToTarget(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__LCU__moveToTarget_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::TelescopeNotConfiguredEx&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::OUC::LCU::sayHello(::Ice::Int delay, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int delay, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_delay(delay)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->sayHello(_m_delay, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int _m_delay;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__sayHello_name, ::Ice::Idempotent, __context);
    try
    {
        _DirectI __direct(delay, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::shutdown(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->shutdown(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__shutdown_name, ::Ice::Idempotent, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::OUC::EncoderData
IceDelegateD::OUC::LCU::getEncodersPosition(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::EncoderData& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getEncodersPosition(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::OUC::EncoderData& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__getEncodersPosition_name, ::Ice::Normal, __context);
    ::OUC::EncoderData __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::OUC::RawEncoderData
IceDelegateD::OUC::LCU::getRawEncodersPosition(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::RawEncoderData& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getRawEncodersPosition(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::OUC::RawEncoderData& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__getRawEncodersPosition_name, ::Ice::Normal, __context);
    ::OUC::RawEncoderData __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::OUC::TelescopeConfigData
IceDelegateD::OUC::LCU::getConfiguration(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::TelescopeConfigData& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getConfiguration(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::OUC::TelescopeConfigData& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__getConfiguration_name, ::Ice::Normal, __context);
    ::OUC::TelescopeConfigData __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::OUC::TelescopeData
IceDelegateD::OUC::LCU::getPosition(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::TelescopeData& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getPosition(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::OUC::TelescopeData& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__getPosition_name, ::Ice::Normal, __context);
    ::OUC::TelescopeData __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::OUC::LCU::isConfigured(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->isConfigured(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__isConfigured_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::OUC::LCU::isTracking(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->isTracking(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__isTracking_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::OUC::LCU::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& fileName, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_fileName(fileName)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setConfiguration(_m_fileName, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_fileName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__setConfiguration_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(fileName, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::OUC::TelescopePosition& targetPos, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_targetPos(targetPos)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setTarget(_m_targetPos, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::OUC::TelescopePosition& _m_targetPos;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__setTarget_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(targetPos, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TargetOutOfLimitsEx&)
    {
        throw;
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_offsetPos(offsetPos)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setOffset(_m_offsetPos, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::OUC::TelescopePosition& _m_offsetPos;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__setOffset_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(offsetPos, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TargetOutOfLimitsEx&)
    {
        throw;
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::TrackingInfo& trkInfo, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_trkInfo(trkInfo)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setTracking(_m_trkInfo, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::OUC::TrackingInfo& _m_trkInfo;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__setTracking_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(trkInfo, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::parkTelescope(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->parkTelescope(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__parkTelescope_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::TelescopeDirection dir, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_dir(dir)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->stopTelescope(_m_dir, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::OUC::TelescopeDirection _m_dir;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__stopTelescope_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(dir, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::OUC::LCU::moveToTarget(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::LCU* servant = dynamic_cast< ::OUC::LCU*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->moveToTarget(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__LCU__moveToTarget_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::OUC::TelescopeNotConfiguredEx&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::ObjectPtr
OUC::LCU::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __OUC__LCU_ids[2] =
{
    "::Ice::Object",
    "::OUC::LCU"
};

bool
OUC::LCU::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__OUC__LCU_ids, __OUC__LCU_ids + 2, _s);
}

::std::vector< ::std::string>
OUC::LCU::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__OUC__LCU_ids[0], &__OUC__LCU_ids[2]);
}

const ::std::string&
OUC::LCU::ice_id(const ::Ice::Current&) const
{
    return __OUC__LCU_ids[1];
}

const ::std::string&
OUC::LCU::ice_staticId()
{
    return __OUC__LCU_ids[1];
}

::Ice::DispatchStatus
OUC::LCU::___sayHello(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Int delay;
    __is->read(delay);
    __is->endReadEncaps();
    sayHello(delay, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___shutdown(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    shutdown(__current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___getEncodersPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::EncoderData __ret = getEncodersPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___getRawEncodersPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::RawEncoderData __ret = getRawEncodersPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___getConfiguration(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::TelescopeConfigData __ret = getConfiguration(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___getPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::TelescopeData __ret = getPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___isConfigured(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = isConfigured(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___isTracking(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = isTracking(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___setConfiguration(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string fileName;
    __is->read(fileName);
    __is->endReadEncaps();
    setConfiguration(fileName, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___setTarget(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::OUC::TelescopePosition targetPos;
    targetPos.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        setTarget(targetPos, __current);
    }
    catch(const ::OUC::TargetOutOfLimitsEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___setOffset(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::OUC::TelescopePosition offsetPos;
    offsetPos.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        setOffset(offsetPos, __current);
    }
    catch(const ::OUC::TargetOutOfLimitsEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___setTracking(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::OUC::TrackingInfo trkInfo;
    try
    {
        setTracking(trkInfo, __current);
        trkInfo.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___parkTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        parkTelescope(__current);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___stopTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::OUC::TelescopeDirection dir;
    ::OUC::__read(__is, dir);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        stopTelescope(dir, __current);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::LCU::___moveToTarget(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        moveToTarget(__current);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

static ::std::string __OUC__LCU_all[] =
{
    "getConfiguration",
    "getEncodersPosition",
    "getPosition",
    "getRawEncodersPosition",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "isConfigured",
    "isTracking",
    "moveToTarget",
    "parkTelescope",
    "sayHello",
    "setConfiguration",
    "setOffset",
    "setTarget",
    "setTracking",
    "shutdown",
    "stopTelescope"
};

::Ice::DispatchStatus
OUC::LCU::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__OUC__LCU_all, __OUC__LCU_all + 19, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __OUC__LCU_all)
    {
        case 0:
        {
            return ___getConfiguration(in, current);
        }
        case 1:
        {
            return ___getEncodersPosition(in, current);
        }
        case 2:
        {
            return ___getPosition(in, current);
        }
        case 3:
        {
            return ___getRawEncodersPosition(in, current);
        }
        case 4:
        {
            return ___ice_id(in, current);
        }
        case 5:
        {
            return ___ice_ids(in, current);
        }
        case 6:
        {
            return ___ice_isA(in, current);
        }
        case 7:
        {
            return ___ice_ping(in, current);
        }
        case 8:
        {
            return ___isConfigured(in, current);
        }
        case 9:
        {
            return ___isTracking(in, current);
        }
        case 10:
        {
            return ___moveToTarget(in, current);
        }
        case 11:
        {
            return ___parkTelescope(in, current);
        }
        case 12:
        {
            return ___sayHello(in, current);
        }
        case 13:
        {
            return ___setConfiguration(in, current);
        }
        case 14:
        {
            return ___setOffset(in, current);
        }
        case 15:
        {
            return ___setTarget(in, current);
        }
        case 16:
        {
            return ___setTracking(in, current);
        }
        case 17:
        {
            return ___shutdown(in, current);
        }
        case 18:
        {
            return ___stopTelescope(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
OUC::LCU::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
OUC::LCU::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
OUC::LCU::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type OUC::LCU was not generated with stream support";
    throw ex;
}

void
OUC::LCU::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type OUC::LCU was not generated with stream support";
    throw ex;
}

void 
OUC::__patch__LCUPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::OUC::LCUPtr* p = static_cast< ::OUC::LCUPtr*>(__addr);
    assert(p);
    *p = ::OUC::LCUPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::OUC::LCU::ice_staticId(), v->ice_id());
    }
}

bool
OUC::operator==(const ::OUC::LCU& l, const ::OUC::LCU& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
OUC::operator<(const ::OUC::LCU& l, const ::OUC::LCU& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
