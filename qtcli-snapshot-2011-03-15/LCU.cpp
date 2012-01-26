// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice-E version 1.2.0
// Generated from file `LCU.ice'

#include <LCU.h>
#include <IceE/Connection.h>
#include <IceE/LocalException.h>
#include <IceE/BasicStream.h>
#include <IceE/LocalException.h>
#include <IceE/Iterator.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 102
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

#ifndef ICEE_PURE_CLIENT
::Ice::Object* IceInternal::upCast(::OUC::LCU* p) { return p; }
#endif // ICEE_PURE_CLIENT
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

static const ::std::string __OUC__LCU_ids[2] =
{
    "::Ice::Object",
    "::OUC::LCU"
};

#ifndef ICEE_PURE_CLIENT

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

::IceInternal::DispatchStatus
OUC::LCU::___sayHello(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::Ice::Int delay;
    __is->read(delay);
    sayHello(delay, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___shutdown(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    shutdown(__current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___getEncodersPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::EncoderData __ret = getEncodersPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___getRawEncodersPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::RawEncoderData __ret = getRawEncodersPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___getConfiguration(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::TelescopeConfigData __ret = getConfiguration(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___getPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::TelescopeData __ret = getPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___isConfigured(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = isConfigured(__current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___isTracking(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = isTracking(__current);
    __os->write(__ret);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___setConfiguration(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::std::string fileName;
    __is->read(fileName);
    setConfiguration(fileName, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___setTarget(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::OUC::TelescopePosition targetPos;
    try
    {
        setTarget(targetPos, __current);
        targetPos.__write(__os);
    }
    catch(const ::OUC::TargetOutOfLimitsEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___setOffset(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    ::OUC::TelescopePosition offsetPos;
    try
    {
        setOffset(offsetPos, __current);
        offsetPos.__write(__os);
    }
    catch(const ::OUC::TargetOutOfLimitsEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___setTracking(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
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
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
OUC::LCU::___parkTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        parkTelescope(__current);
    }
    catch(const ::OUC::TelescopeNotConfiguredEx& __ex)
    {
        __os->write(__ex);
        return ::IceInternal::DispatchUserException;
    }
    return ::IceInternal::DispatchOK;
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
    "parkTelescope",
    "sayHello",
    "setConfiguration",
    "setOffset",
    "setTarget",
    "setTracking",
    "shutdown"
};

::IceInternal::DispatchStatus
OUC::LCU::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__OUC__LCU_all, __OUC__LCU_all + 17, current.operation);
    if(r.first == r.second)
    {
        return ::IceInternal::DispatchOperationNotExist;
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
            return ___parkTelescope(in, current);
        }
        case 11:
        {
            return ___sayHello(in, current);
        }
        case 12:
        {
            return ___setConfiguration(in, current);
        }
        case 13:
        {
            return ___setOffset(in, current);
        }
        case 14:
        {
            return ___setTarget(in, current);
        }
        case 15:
        {
            return ___setTracking(in, current);
        }
        case 16:
        {
            return ___shutdown(in, current);
        }
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
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

#endif // ICEE_PURE_CLIENT

void
IceProxy::OUC::LCU::sayHello(::Ice::Int delay, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            static const ::std::string __operation("sayHello");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Idempotent, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.stream();
                __os->write(delay);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__connection, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::OUC::LCU::shutdown(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            static const ::std::string __operation("shutdown");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Idempotent, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__connection, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::OUC::EncoderData
IceProxy::OUC::LCU::getEncodersPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("getEncodersPosition");
            static const ::std::string __operation("getEncodersPosition");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::OUC::TelescopeNotConfiguredEx&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::OUC::EncoderData __ret;
                __ret.__read(__is);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::OUC::RawEncoderData
IceProxy::OUC::LCU::getRawEncodersPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("getRawEncodersPosition");
            static const ::std::string __operation("getRawEncodersPosition");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::OUC::TelescopeNotConfiguredEx&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::OUC::RawEncoderData __ret;
                __ret.__read(__is);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::OUC::TelescopeConfigData
IceProxy::OUC::LCU::getConfiguration(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("getConfiguration");
            static const ::std::string __operation("getConfiguration");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::OUC::TelescopeNotConfiguredEx&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::OUC::TelescopeConfigData __ret;
                __ret.__read(__is);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

::OUC::TelescopeData
IceProxy::OUC::LCU::getPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("getPosition");
            static const ::std::string __operation("getPosition");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::OUC::TelescopeNotConfiguredEx&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                ::OUC::TelescopeData __ret;
                __ret.__read(__is);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

bool
IceProxy::OUC::LCU::isConfigured(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("isConfigured");
            static const ::std::string __operation("isConfigured");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                bool __ret;
                __is->read(__ret);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

bool
IceProxy::OUC::LCU::isTracking(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("isTracking");
            static const ::std::string __operation("isTracking");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                bool __ret;
                __is->read(__ret);
                return __ret;
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::OUC::LCU::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            static const ::std::string __operation("setConfiguration");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            try
            {
                ::IceInternal::BasicStream* __os = __outS.stream();
                __os->write(fileName);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                __outS.abort(__ex);
            }
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::OUC::LCU::setTarget(::OUC::TelescopePosition& targetPos, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("setTarget");
            static const ::std::string __operation("setTarget");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
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
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                targetPos.__read(__is);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::OUC::LCU::setOffset(::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("setOffset");
            static const ::std::string __operation("setOffset");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
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
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                offsetPos.__read(__is);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::OUC::LCU::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("setTracking");
            static const ::std::string __operation("setTracking");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::OUC::TelescopeNotConfiguredEx&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
                trkInfo.__read(__is);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

void
IceProxy::OUC::LCU::parkTelescope(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::Ice::ConnectionPtr __connection;
        try
        {
            __checkTwowayOnly("parkTelescope");
            static const ::std::string __operation("parkTelescope");
            __connection = ice_getConnection();
            ::IceInternal::Outgoing __outS(__connection.get(), _reference.get(), __operation, ::Ice::Normal, __ctx);
            bool __ok = __outS.invoke();
            try
            {
                ::IceInternal::BasicStream* __is = __outS.stream();
                if(!__ok)
                {
                    try
                    {
                        __is->throwException();
                    }
                    catch(const ::OUC::TelescopeNotConfiguredEx&)
                    {
                        throw;
                    }
                    catch(const ::Ice::UserException& __ex)
                    {
                        ::Ice::UnknownUserException __uex(__FILE__, __LINE__);
                        __uex.unknown = __ex.ice_name();
                        throw __uex;
                    }
                }
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
            catch(...)
            {
                throw;
            }
#endif
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__connection, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__connection, __ex, __cnt);
        }
#if defined(_MSC_VER) && defined(_M_ARM) // ARM bug.
        catch(...)
        {
            throw;
        }
#endif
    }
}

const ::std::string&
IceProxy::OUC::LCU::ice_staticId()
{
    return __OUC__LCU_ids[1];
}
