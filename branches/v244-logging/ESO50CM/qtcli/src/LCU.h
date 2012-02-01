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

#ifndef __LCU_h__
#define __LCU_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

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

namespace IceProxy
{

namespace OUC
{

class LCU;

}

}

namespace OUC
{

class LCU;
bool operator==(const LCU&, const LCU&);
bool operator<(const LCU&, const LCU&);

}

namespace IceInternal
{

::Ice::Object* upCast(::OUC::LCU*);
::IceProxy::Ice::Object* upCast(::IceProxy::OUC::LCU*);

}

namespace OUC
{

typedef ::IceInternal::Handle< ::OUC::LCU> LCUPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::OUC::LCU> LCUPrx;

void __read(::IceInternal::BasicStream*, LCUPrx&);
void __patch__LCUPtr(void*, ::Ice::ObjectPtr&);

}

namespace OUC
{

enum TelescopeDirection
{
    North,
    South,
    East,
    West
};

void __write(::IceInternal::BasicStream*, TelescopeDirection);
void __read(::IceInternal::BasicStream*, TelescopeDirection&);

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
    ::Ice::Double lcuTime;

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
    ::Ice::Double localTime;
    ::Ice::Double lcuTime;

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
    ::Ice::Double localTime;
    ::Ice::Double lcuTime;
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
    ::Ice::Double localTime;
    ::Ice::Double lcuTime;
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
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
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
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
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
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

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

    void setTarget(const ::OUC::TelescopePosition& targetPos)
    {
        setTarget(targetPos, 0);
    }
    void setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context& __ctx)
    {
        setTarget(targetPos, &__ctx);
    }
    
private:

    void setTarget(const ::OUC::TelescopePosition&, const ::Ice::Context*);
    
public:

    void setOffset(const ::OUC::TelescopePosition& offsetPos)
    {
        setOffset(offsetPos, 0);
    }
    void setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context& __ctx)
    {
        setOffset(offsetPos, &__ctx);
    }
    
private:

    void setOffset(const ::OUC::TelescopePosition&, const ::Ice::Context*);
    
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

    void stopTelescope(::OUC::TelescopeDirection dir)
    {
        stopTelescope(dir, 0);
    }
    void stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context& __ctx)
    {
        stopTelescope(dir, &__ctx);
    }
    
private:

    void stopTelescope(::OUC::TelescopeDirection, const ::Ice::Context*);
    
public:

    void moveToTarget()
    {
        moveToTarget(0);
    }
    void moveToTarget(const ::Ice::Context& __ctx)
    {
        moveToTarget(&__ctx);
    }
    
private:

    void moveToTarget(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<LCU> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<LCU> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<LCU*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<LCU*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace OUC
{

class LCU : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void sayHello(::Ice::Int, const ::Ice::Context*) = 0;

    virtual void shutdown(const ::Ice::Context*) = 0;

    virtual ::OUC::EncoderData getEncodersPosition(const ::Ice::Context*) = 0;

    virtual ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Context*) = 0;

    virtual ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Context*) = 0;

    virtual ::OUC::TelescopeData getPosition(const ::Ice::Context*) = 0;

    virtual bool isConfigured(const ::Ice::Context*) = 0;

    virtual bool isTracking(const ::Ice::Context*) = 0;

    virtual void setConfiguration(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setTarget(const ::OUC::TelescopePosition&, const ::Ice::Context*) = 0;

    virtual void setOffset(const ::OUC::TelescopePosition&, const ::Ice::Context*) = 0;

    virtual void setTracking(::OUC::TrackingInfo&, const ::Ice::Context*) = 0;

    virtual void parkTelescope(const ::Ice::Context*) = 0;

    virtual void stopTelescope(::OUC::TelescopeDirection, const ::Ice::Context*) = 0;

    virtual void moveToTarget(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace OUC
{

class LCU : virtual public ::IceDelegate::OUC::LCU,
            virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void sayHello(::Ice::Int, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);

    virtual ::OUC::EncoderData getEncodersPosition(const ::Ice::Context*);

    virtual ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Context*);

    virtual ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Context*);

    virtual ::OUC::TelescopeData getPosition(const ::Ice::Context*);

    virtual bool isConfigured(const ::Ice::Context*);

    virtual bool isTracking(const ::Ice::Context*);

    virtual void setConfiguration(const ::std::string&, const ::Ice::Context*);

    virtual void setTarget(const ::OUC::TelescopePosition&, const ::Ice::Context*);

    virtual void setOffset(const ::OUC::TelescopePosition&, const ::Ice::Context*);

    virtual void setTracking(::OUC::TrackingInfo&, const ::Ice::Context*);

    virtual void parkTelescope(const ::Ice::Context*);

    virtual void stopTelescope(::OUC::TelescopeDirection, const ::Ice::Context*);

    virtual void moveToTarget(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace OUC
{

class LCU : virtual public ::IceDelegate::OUC::LCU,
            virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void sayHello(::Ice::Int, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);

    virtual ::OUC::EncoderData getEncodersPosition(const ::Ice::Context*);

    virtual ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Context*);

    virtual ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Context*);

    virtual ::OUC::TelescopeData getPosition(const ::Ice::Context*);

    virtual bool isConfigured(const ::Ice::Context*);

    virtual bool isTracking(const ::Ice::Context*);

    virtual void setConfiguration(const ::std::string&, const ::Ice::Context*);

    virtual void setTarget(const ::OUC::TelescopePosition&, const ::Ice::Context*);

    virtual void setOffset(const ::OUC::TelescopePosition&, const ::Ice::Context*);

    virtual void setTracking(::OUC::TrackingInfo&, const ::Ice::Context*);

    virtual void parkTelescope(const ::Ice::Context*);

    virtual void stopTelescope(::OUC::TelescopeDirection, const ::Ice::Context*);

    virtual void moveToTarget(const ::Ice::Context*);
};

}

}

namespace OUC
{

class LCU : virtual public ::Ice::Object
{
public:

    typedef LCUPrx ProxyType;
    typedef LCUPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void sayHello(::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___sayHello(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::EncoderData getEncodersPosition(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getEncodersPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::RawEncoderData getRawEncodersPosition(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getRawEncodersPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::TelescopeConfigData getConfiguration(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getConfiguration(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::OUC::TelescopeData getPosition(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getPosition(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool isConfigured(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___isConfigured(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool isTracking(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___isTracking(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setConfiguration(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setConfiguration(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setTarget(const ::OUC::TelescopePosition&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setTarget(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setOffset(const ::OUC::TelescopePosition&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setOffset(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setTracking(::OUC::TrackingInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setTracking(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void parkTelescope(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___parkTelescope(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopTelescope(::OUC::TelescopeDirection, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___stopTelescope(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void moveToTarget(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___moveToTarget(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
