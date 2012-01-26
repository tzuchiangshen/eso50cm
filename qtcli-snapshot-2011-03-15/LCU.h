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

#ifndef __LCU_h__
#define __LCU_h__

#include <IceE/ProxyF.h>
#ifndef ICEE_PURE_CLIENT
#  include <IceE/ObjectF.h>
#endif
#include <IceE/Exception.h>
#include <IceE/ScopedArray.h>
#include <IceE/Proxy.h>
#ifndef ICEE_PURE_CLIENT
#  include <IceE/Object.h>
#  include <IceE/Incoming.h>
#endif
#include <IceE/Outgoing.h>
#include <IceE/UserExceptionFactory.h>
#include <IceE/FactoryTable.h>
#include <IceE/UndefSysMacros.h>

#ifndef ICEE_IGNORE_VERSION
#   if ICEE_INT_VERSION / 100 != 102
#       error IceE version mismatch!
#   endif
#   if ICEE_INT_VERSION % 100 < 0
#       error IceE patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace OUC
{

class LCU;

}

}

namespace OUC
{

#ifndef ICEE_PURE_CLIENT

class LCU;
bool operator==(const LCU&, const LCU&);
bool operator<(const LCU&, const LCU&);

#endif // ICEE_PURE_CLIENT

}

namespace IceInternal
{

#ifndef ICEE_PURE_CLIENT
::Ice::Object* upCast(::OUC::LCU*);
#endif // ICEE_PURE_CLIENT
::IceProxy::Ice::Object* upCast(::IceProxy::OUC::LCU*);

}

namespace OUC
{

#ifndef ICEE_PURE_CLIENT

typedef ::IceInternal::Handle< ::OUC::LCU> LCUPtr;

#endif // ICEE_PURE_CLIENT

typedef ::IceInternal::ProxyHandle< ::IceProxy::OUC::LCU> LCUPrx;

void __read(::IceInternal::BasicStream*, LCUPrx&);

}

namespace OUC
{

struct RawEncoderData
{
    ::Ice::Int lectAlphaWormE;
    ::Ice::Int lectAlphaAxisE;
    ::Ice::Int lectAlphaMotor;
    ::Ice::Int posAlphaWormE;
    ::Ice::Int posAlphaAxisE;
    ::Ice::Int posAlphaMotor;
    ::Ice::Int lectDeltaWormE;
    ::Ice::Int lectDeltaAxisE;
    ::Ice::Int lectDeltaMotor;
    ::Ice::Int posDeltaWormE;
    ::Ice::Int posDeltaAxisE;
    ::Ice::Int posDeltaMotor;
    ::Ice::Double deltaT;
    ::Ice::Long lcuTime;

    bool operator==(const RawEncoderData&) const;
    bool operator<(const RawEncoderData&) const;
    bool operator!=(const RawEncoderData& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const RawEncoderData& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const RawEncoderData& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const RawEncoderData& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct EncoderData
{
    ::Ice::Double alphaWormE;
    ::Ice::Double alphaAxisE;
    ::Ice::Double deltaWormE;
    ::Ice::Double deltaAxisE;
    ::Ice::Long localTime;
    ::Ice::Long lcuTime;

    bool operator==(const EncoderData&) const;
    bool operator<(const EncoderData&) const;
    bool operator!=(const EncoderData& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const EncoderData& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const EncoderData& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const EncoderData& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct TrackingInfo
{
    bool trackState;
    ::Ice::Double degVel;
    ::Ice::Int ticVel;

    bool operator==(const TrackingInfo&) const;
    bool operator<(const TrackingInfo&) const;
    bool operator!=(const TrackingInfo& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const TrackingInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const TrackingInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const TrackingInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct TelescopeConfigData
{
    ::Ice::Long localTime;
    ::Ice::Long lcuTime;
    ::Ice::Double latitude;
    ::Ice::Double longitude;
    ::Ice::Double altitude;
    ::Ice::Double AMT;
    ::Ice::Double AMH;
    ::Ice::Double AMR;
    ::Ice::Double AWT;
    ::Ice::Double AWH;
    ::Ice::Double AWR;
    ::Ice::Double AAT;
    ::Ice::Double AAH;
    ::Ice::Double AAR;
    ::Ice::Double DMT;
    ::Ice::Double DMH;
    ::Ice::Double DMR;
    ::Ice::Double DWT;
    ::Ice::Double DWH;
    ::Ice::Double DWR;
    ::Ice::Double DAT;
    ::Ice::Double DAH;
    ::Ice::Double DAR;

    bool operator==(const TelescopeConfigData&) const;
    bool operator<(const TelescopeConfigData&) const;
    bool operator!=(const TelescopeConfigData& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const TelescopeConfigData& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const TelescopeConfigData& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const TelescopeConfigData& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct TelescopePosition
{
    ::Ice::Double localSideralTime;
    ::Ice::Double RA;
    ::Ice::Double HA;
    ::Ice::Double Dec;
    ::Ice::Double Alt;
    ::Ice::Double Az;

    bool operator==(const TelescopePosition&) const;
    bool operator<(const TelescopePosition&) const;
    bool operator!=(const TelescopePosition& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const TelescopePosition& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const TelescopePosition& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const TelescopePosition& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct TelescopeData
{
    ::Ice::Long localTime;
    ::Ice::Long lcuTime;
    ::Ice::Double deltaT;
    ::Ice::Double localSideralTime;
    ::Ice::Double julianDate;
    ::Ice::Double latitude;
    ::Ice::Double longitude;
    ::Ice::Double altitude;
    ::Ice::Double lowElevation;
    ::Ice::Double highElevation;
    ::std::string targetName;
    ::OUC::TelescopePosition currentPos;
    ::OUC::TelescopePosition targetPos;
    ::OUC::TelescopePosition differencePos;

    bool operator==(const TelescopeData&) const;
    bool operator<(const TelescopeData&) const;
    bool operator!=(const TelescopeData& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const TelescopeData& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const TelescopeData& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const TelescopeData& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

class ErrorEx : public ::Ice::UserException
{
public:

    ErrorEx() {}
    explicit ErrorEx(const ::std::string&);
    virtual ~ErrorEx() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string reason;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

static ErrorEx __ErrorEx_init;

class TelescopeNotConfiguredEx : public ::OUC::ErrorEx
{
public:

    TelescopeNotConfiguredEx() {}
    explicit TelescopeNotConfiguredEx(const ::std::string&);
    virtual ~TelescopeNotConfiguredEx() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

class TargetOutOfLimitsEx : public ::OUC::ErrorEx
{
public:

    TargetOutOfLimitsEx() {}
    explicit TargetOutOfLimitsEx(const ::std::string&);
    virtual ~TargetOutOfLimitsEx() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
};

}

namespace OUC
{

#ifndef ICEE_PURE_CLIENT

class LCU : virtual public ::Ice::Object
{
public:

    typedef LCUPrx ProxyType;
    typedef LCUPtr PointerType;
    

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void sayHello(::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___sayHello(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::EncoderData getEncodersPosition(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getEncodersPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getRawEncodersPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getConfiguration(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::TelescopeData getPosition(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool isConfigured(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___isConfigured(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool isTracking(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___isTracking(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setConfiguration(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setConfiguration(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setTarget(::OUC::TelescopePosition&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setTarget(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setOffset(::OUC::TelescopePosition&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setOffset(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setTracking(::OUC::TrackingInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setTracking(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void parkTelescope(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___parkTelescope(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);
};

#endif // ICEE_PURE_CLIENT

}

namespace IceProxy
{

namespace OUC
{

class LCU : virtual public ::IceProxy::Ice::Object
{
public:

    void sayHello(::Ice::Int delay)
    {
        sayHello(delay, 0);
    }
    void sayHello(::Ice::Int delay, const ::Ice::Context& __ctx)
    {
        sayHello(delay, &__ctx);
    }
    
private:

    void sayHello(::Ice::Int, const ::Ice::Context*);
    
public:

    void shutdown()
    {
        shutdown(0);
    }
    void shutdown(const ::Ice::Context& __ctx)
    {
        shutdown(&__ctx);
    }
    
private:

    void shutdown(const ::Ice::Context*);
    
public:

    ::OUC::EncoderData getEncodersPosition()
    {
        return getEncodersPosition(0);
    }
    ::OUC::EncoderData getEncodersPosition(const ::Ice::Context& __ctx)
    {
        return getEncodersPosition(&__ctx);
    }
    
private:

    ::OUC::EncoderData getEncodersPosition(const ::Ice::Context*);
    
public:

    ::OUC::RawEncoderData getRawEncodersPosition()
    {
        return getRawEncodersPosition(0);
    }
    ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Context& __ctx)
    {
        return getRawEncodersPosition(&__ctx);
    }
    
private:

    ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Context*);
    
public:

    ::OUC::TelescopeConfigData getConfiguration()
    {
        return getConfiguration(0);
    }
    ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Context& __ctx)
    {
        return getConfiguration(&__ctx);
    }
    
private:

    ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Context*);
    
public:

    ::OUC::TelescopeData getPosition()
    {
        return getPosition(0);
    }
    ::OUC::TelescopeData getPosition(const ::Ice::Context& __ctx)
    {
        return getPosition(&__ctx);
    }
    
private:

    ::OUC::TelescopeData getPosition(const ::Ice::Context*);
    
public:

    bool isConfigured()
    {
        return isConfigured(0);
    }
    bool isConfigured(const ::Ice::Context& __ctx)
    {
        return isConfigured(&__ctx);
    }
    
private:

    bool isConfigured(const ::Ice::Context*);
    
public:

    bool isTracking()
    {
        return isTracking(0);
    }
    bool isTracking(const ::Ice::Context& __ctx)
    {
        return isTracking(&__ctx);
    }
    
private:

    bool isTracking(const ::Ice::Context*);
    
public:

    void setConfiguration(const ::std::string& fileName)
    {
        setConfiguration(fileName, 0);
    }
    void setConfiguration(const ::std::string& fileName, const ::Ice::Context& __ctx)
    {
        setConfiguration(fileName, &__ctx);
    }
    
private:

    void setConfiguration(const ::std::string&, const ::Ice::Context*);
    
public:

    void setTarget(::OUC::TelescopePosition& targetPos)
    {
        setTarget(targetPos, 0);
    }
    void setTarget(::OUC::TelescopePosition& targetPos, const ::Ice::Context& __ctx)
    {
        setTarget(targetPos, &__ctx);
    }
    
private:

    void setTarget(::OUC::TelescopePosition&, const ::Ice::Context*);
    
public:

    void setOffset(::OUC::TelescopePosition& offsetPos)
    {
        setOffset(offsetPos, 0);
    }
    void setOffset(::OUC::TelescopePosition& offsetPos, const ::Ice::Context& __ctx)
    {
        setOffset(offsetPos, &__ctx);
    }
    
private:

    void setOffset(::OUC::TelescopePosition&, const ::Ice::Context*);
    
public:

    void setTracking(::OUC::TrackingInfo& trkInfo)
    {
        setTracking(trkInfo, 0);
    }
    void setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context& __ctx)
    {
        setTracking(trkInfo, &__ctx);
    }
    
private:

    void setTracking(::OUC::TrackingInfo&, const ::Ice::Context*);
    
public:

    void parkTelescope()
    {
        parkTelescope(0);
    }
    void parkTelescope(const ::Ice::Context& __ctx)
    {
        parkTelescope(&__ctx);
    }
    
private:

    void parkTelescope(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();
};

}

}

#endif
