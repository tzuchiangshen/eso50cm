// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Observing.ice'

#include <Observing.h>
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

static const ::std::string __OUC__Telescope__sayHelloTelescope_name = "sayHelloTelescope";

static const ::std::string __OUC__Telescope__getEncodersPosition_name = "getEncodersPosition";

static const ::std::string __OUC__Telescope__getRawEncodersPosition_name = "getRawEncodersPosition";

static const ::std::string __OUC__Telescope__getConfiguration_name = "getConfiguration";

static const ::std::string __OUC__Telescope__getPosition_name = "getPosition";

static const ::std::string __OUC__Telescope__isConfigured_name = "isConfigured";

static const ::std::string __OUC__Telescope__isTracking_name = "isTracking";

static const ::std::string __OUC__Telescope__setConfiguration_name = "setConfiguration";

static const ::std::string __OUC__Telescope__setTarget_name = "setTarget";

static const ::std::string __OUC__Telescope__setOffset_name = "setOffset";

static const ::std::string __OUC__Telescope__setTracking_name = "setTracking";

static const ::std::string __OUC__Telescope__parkTelescope_name = "parkTelescope";

static const ::std::string __OUC__Telescope__stopTelescope_name = "stopTelescope";

static const ::std::string __OUC__Telescope__moveToTarget_name = "moveToTarget";

static const ::std::string __OUC__Observing__sayHello_name = "sayHello";

static const ::std::string __OUC__Observing__shutdown_name = "shutdown";

static const ::std::string __OUC__Observing__getTelescope_name = "getTelescope";

::Ice::Object* IceInternal::upCast(::OUC::Telescope* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::OUC::Telescope* p) { return p; }

::Ice::Object* IceInternal::upCast(::OUC::Observing* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::OUC::Observing* p) { return p; }

void
OUC::__read(::IceInternal::BasicStream* __is, ::OUC::TelescopePrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::OUC::Telescope;
        v->__copyFrom(proxy);
    }
}

void
OUC::__read(::IceInternal::BasicStream* __is, ::OUC::ObservingPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::OUC::Observing;
        v->__copyFrom(proxy);
    }
}

OUC::NotLCUReferenceAvailableEx::NotLCUReferenceAvailableEx(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx(__ice_reason)
#else
    ::OUC::ErrorEx(__ice_reason)
#endif
{
}

OUC::NotLCUReferenceAvailableEx::~NotLCUReferenceAvailableEx() throw()
{
}

static const char* __OUC__NotLCUReferenceAvailableEx_name = "OUC::NotLCUReferenceAvailableEx";

::std::string
OUC::NotLCUReferenceAvailableEx::ice_name() const
{
    return __OUC__NotLCUReferenceAvailableEx_name;
}

::Ice::Exception*
OUC::NotLCUReferenceAvailableEx::ice_clone() const
{
    return new NotLCUReferenceAvailableEx(*this);
}

void
OUC::NotLCUReferenceAvailableEx::ice_throw() const
{
    throw *this;
}

void
OUC::NotLCUReferenceAvailableEx::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::OUC::NotLCUReferenceAvailableEx"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    ErrorEx::__write(__os);
#else
    ::OUC::ErrorEx::__write(__os);
#endif
}

void
OUC::NotLCUReferenceAvailableEx::__read(::IceInternal::BasicStream* __is, bool __rid)
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
OUC::NotLCUReferenceAvailableEx::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::NotLCUReferenceAvailableEx was not generated with stream support";
    throw ex;
}

void
OUC::NotLCUReferenceAvailableEx::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception OUC::NotLCUReferenceAvailableEx was not generated with stream support";
    throw ex;
}

struct __F__OUC__NotLCUReferenceAvailableEx : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::OUC::NotLCUReferenceAvailableEx();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__OUC__NotLCUReferenceAvailableEx__Ptr = new __F__OUC__NotLCUReferenceAvailableEx;

const ::IceInternal::UserExceptionFactoryPtr&
OUC::NotLCUReferenceAvailableEx::ice_factory()
{
    return __F__OUC__NotLCUReferenceAvailableEx__Ptr;
}

class __F__OUC__NotLCUReferenceAvailableEx__Init
{
public:

    __F__OUC__NotLCUReferenceAvailableEx__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::OUC::NotLCUReferenceAvailableEx", ::OUC::NotLCUReferenceAvailableEx::ice_factory());
    }

    ~__F__OUC__NotLCUReferenceAvailableEx__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::OUC::NotLCUReferenceAvailableEx");
    }
};

static __F__OUC__NotLCUReferenceAvailableEx__Init __F__OUC__NotLCUReferenceAvailableEx__i;

#ifdef __APPLE__
extern "C" { void __F__OUC__NotLCUReferenceAvailableEx__initializer() {} }
#endif

void
IceProxy::OUC::Telescope::sayHelloTelescope(::Ice::Int delay, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
            __del->sayHelloTelescope(delay, __ctx);
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

::OUC::EncoderData
IceProxy::OUC::Telescope::getEncodersPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__getEncodersPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::getRawEncodersPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__getRawEncodersPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::getConfiguration(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__getConfiguration_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::getPosition(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__getPosition_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::isConfigured(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__isConfigured_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::isTracking(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__isTracking_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__setConfiguration_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__setTarget_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__setOffset_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__setTracking_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::parkTelescope(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__parkTelescope_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__stopTelescope_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::moveToTarget(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Telescope__moveToTarget_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Telescope* __del = dynamic_cast< ::IceDelegate::OUC::Telescope*>(__delBase.get());
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
IceProxy::OUC::Telescope::ice_staticId()
{
    return ::OUC::Telescope::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::OUC::Telescope::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::OUC::Telescope);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::OUC::Telescope::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::OUC::Telescope);
}

::IceProxy::Ice::Object*
IceProxy::OUC::Telescope::__newInstance() const
{
    return new Telescope;
}

void
IceProxy::OUC::Observing::sayHello(::Ice::Int delay, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Observing* __del = dynamic_cast< ::IceDelegate::OUC::Observing*>(__delBase.get());
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
IceProxy::OUC::Observing::shutdown(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Observing* __del = dynamic_cast< ::IceDelegate::OUC::Observing*>(__delBase.get());
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

::OUC::TelescopePrx
IceProxy::OUC::Observing::getTelescope(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__OUC__Observing__getTelescope_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::OUC::Observing* __del = dynamic_cast< ::IceDelegate::OUC::Observing*>(__delBase.get());
            return __del->getTelescope(__ctx);
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
IceProxy::OUC::Observing::ice_staticId()
{
    return ::OUC::Observing::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::OUC::Observing::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::OUC::Observing);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::OUC::Observing::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::OUC::Observing);
}

::IceProxy::Ice::Object*
IceProxy::OUC::Observing::__newInstance() const
{
    return new Observing;
}

void
IceDelegateM::OUC::Telescope::sayHelloTelescope(::Ice::Int delay, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__sayHelloTelescope_name, ::Ice::Normal, __context);
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

::OUC::EncoderData
IceDelegateM::OUC::Telescope::getEncodersPosition(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__getEncodersPosition_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::getRawEncodersPosition(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__getRawEncodersPosition_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::getConfiguration(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__getConfiguration_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::getPosition(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__getPosition_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::isConfigured(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__isConfigured_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
            {
                throw;
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
IceDelegateM::OUC::Telescope::isTracking(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__isTracking_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
            {
                throw;
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
IceDelegateM::OUC::Telescope::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__setConfiguration_name, ::Ice::Normal, __context);
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
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__setTarget_name, ::Ice::Normal, __context);
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
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
            {
                throw;
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
IceDelegateM::OUC::Telescope::setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__setOffset_name, ::Ice::Normal, __context);
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
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
            {
                throw;
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
IceDelegateM::OUC::Telescope::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__setTracking_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
        trkInfo.__read(__is);
        __is->endReadEncaps();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::OUC::Telescope::parkTelescope(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__parkTelescope_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__stopTelescope_name, ::Ice::Normal, __context);
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
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Telescope::moveToTarget(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Telescope__moveToTarget_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateM::OUC::Observing::sayHello(::Ice::Int delay, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Observing__sayHello_name, ::Ice::Idempotent, __context);
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
IceDelegateM::OUC::Observing::shutdown(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Observing__shutdown_name, ::Ice::Idempotent, __context);
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

::OUC::TelescopePrx
IceDelegateM::OUC::Observing::getTelescope(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __OUC__Observing__getTelescope_name, ::Ice::Normal, __context);
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
        ::OUC::TelescopePrx __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::OUC::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::OUC::Telescope::sayHelloTelescope(::Ice::Int delay, const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->sayHelloTelescope(_m_delay, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int _m_delay;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__Telescope__sayHelloTelescope_name, ::Ice::Normal, __context);
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

::OUC::EncoderData
IceDelegateD::OUC::Telescope::getEncodersPosition(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__getEncodersPosition_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
    return __result;
}

::OUC::RawEncoderData
IceDelegateD::OUC::Telescope::getRawEncodersPosition(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__getRawEncodersPosition_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
    return __result;
}

::OUC::TelescopeConfigData
IceDelegateD::OUC::Telescope::getConfiguration(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__getConfiguration_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
    return __result;
}

::OUC::TelescopeData
IceDelegateD::OUC::Telescope::getPosition(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__getPosition_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
    return __result;
}

bool
IceDelegateD::OUC::Telescope::isConfigured(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->isConfigured(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        bool& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__Telescope__isConfigured_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateD::OUC::Telescope::isTracking(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->isTracking(_current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        bool& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__Telescope__isTracking_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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

void
IceDelegateD::OUC::Telescope::setConfiguration(const ::std::string& fileName, const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                servant->setConfiguration(_m_fileName, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        const ::std::string& _m_fileName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__Telescope__setConfiguration_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateD::OUC::Telescope::setTarget(const ::OUC::TelescopePosition& targetPos, const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__setTarget_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
    {
        throw;
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
IceDelegateD::OUC::Telescope::setOffset(const ::OUC::TelescopePosition& offsetPos, const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__setOffset_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
    {
        throw;
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
IceDelegateD::OUC::Telescope::setTracking(::OUC::TrackingInfo& trkInfo, const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__setTracking_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateD::OUC::Telescope::parkTelescope(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__parkTelescope_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateD::OUC::Telescope::stopTelescope(::OUC::TelescopeDirection dir, const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__stopTelescope_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateD::OUC::Telescope::moveToTarget(const ::Ice::Context* __context)
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
            ::OUC::Telescope* servant = dynamic_cast< ::OUC::Telescope*>(object);
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
    __initCurrent(__current, __OUC__Telescope__moveToTarget_name, ::Ice::Normal, __context);
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
    catch(const ::OUC::NotLCUReferenceAvailableEx&)
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
IceDelegateD::OUC::Observing::sayHello(::Ice::Int delay, const ::Ice::Context* __context)
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
            ::OUC::Observing* servant = dynamic_cast< ::OUC::Observing*>(object);
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
    __initCurrent(__current, __OUC__Observing__sayHello_name, ::Ice::Idempotent, __context);
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
IceDelegateD::OUC::Observing::shutdown(const ::Ice::Context* __context)
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
            ::OUC::Observing* servant = dynamic_cast< ::OUC::Observing*>(object);
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
    __initCurrent(__current, __OUC__Observing__shutdown_name, ::Ice::Idempotent, __context);
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

::OUC::TelescopePrx
IceDelegateD::OUC::Observing::getTelescope(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::OUC::TelescopePrx& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::OUC::Observing* servant = dynamic_cast< ::OUC::Observing*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getTelescope(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::OUC::TelescopePrx& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __OUC__Observing__getTelescope_name, ::Ice::Normal, __context);
    ::OUC::TelescopePrx __result;
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

::Ice::ObjectPtr
OUC::Telescope::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __OUC__Telescope_ids[2] =
{
    "::Ice::Object",
    "::OUC::Telescope"
};

bool
OUC::Telescope::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__OUC__Telescope_ids, __OUC__Telescope_ids + 2, _s);
}

::std::vector< ::std::string>
OUC::Telescope::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__OUC__Telescope_ids[0], &__OUC__Telescope_ids[2]);
}

const ::std::string&
OUC::Telescope::ice_id(const ::Ice::Current&) const
{
    return __OUC__Telescope_ids[1];
}

const ::std::string&
OUC::Telescope::ice_staticId()
{
    return __OUC__Telescope_ids[1];
}

::Ice::DispatchStatus
OUC::Telescope::___sayHelloTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Int delay;
    __is->read(delay);
    __is->endReadEncaps();
    sayHelloTelescope(delay, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::Telescope::___getEncodersPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::EncoderData __ret = getEncodersPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___getRawEncodersPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::RawEncoderData __ret = getRawEncodersPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___getConfiguration(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::TelescopeConfigData __ret = getConfiguration(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___getPosition(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::OUC::TelescopeData __ret = getPosition(__current);
        __ret.__write(__os);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___isConfigured(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        bool __ret = isConfigured(__current);
        __os->write(__ret);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::Telescope::___isTracking(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        bool __ret = isTracking(__current);
        __os->write(__ret);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::Telescope::___setConfiguration(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string fileName;
    __is->read(fileName);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        setConfiguration(fileName, __current);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::Telescope::___setTarget(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
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
OUC::Telescope::___setOffset(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
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
OUC::Telescope::___setTracking(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___parkTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        parkTelescope(__current);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___stopTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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
OUC::Telescope::___moveToTarget(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        moveToTarget(__current);
    }
    catch(const ::OUC::NotLCUReferenceAvailableEx& __ex)
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

static ::std::string __OUC__Telescope_all[] =
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
    "sayHelloTelescope",
    "setConfiguration",
    "setOffset",
    "setTarget",
    "setTracking",
    "stopTelescope"
};

::Ice::DispatchStatus
OUC::Telescope::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__OUC__Telescope_all, __OUC__Telescope_all + 18, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __OUC__Telescope_all)
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
            return ___sayHelloTelescope(in, current);
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
            return ___stopTelescope(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
OUC::Telescope::__write(::IceInternal::BasicStream* __os) const
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
OUC::Telescope::__read(::IceInternal::BasicStream* __is, bool __rid)
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
OUC::Telescope::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type OUC::Telescope was not generated with stream support";
    throw ex;
}

void
OUC::Telescope::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type OUC::Telescope was not generated with stream support";
    throw ex;
}

void 
OUC::__patch__TelescopePtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::OUC::TelescopePtr* p = static_cast< ::OUC::TelescopePtr*>(__addr);
    assert(p);
    *p = ::OUC::TelescopePtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::OUC::Telescope::ice_staticId(), v->ice_id());
    }
}

bool
OUC::operator==(const ::OUC::Telescope& l, const ::OUC::Telescope& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
OUC::operator<(const ::OUC::Telescope& l, const ::OUC::Telescope& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
OUC::Observing::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __OUC__Observing_ids[2] =
{
    "::Ice::Object",
    "::OUC::Observing"
};

bool
OUC::Observing::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__OUC__Observing_ids, __OUC__Observing_ids + 2, _s);
}

::std::vector< ::std::string>
OUC::Observing::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__OUC__Observing_ids[0], &__OUC__Observing_ids[2]);
}

const ::std::string&
OUC::Observing::ice_id(const ::Ice::Current&) const
{
    return __OUC__Observing_ids[1];
}

const ::std::string&
OUC::Observing::ice_staticId()
{
    return __OUC__Observing_ids[1];
}

::Ice::DispatchStatus
OUC::Observing::___sayHello(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
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
OUC::Observing::___shutdown(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    shutdown(__current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
OUC::Observing::___getTelescope(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::OUC::TelescopePrx __ret = getTelescope(__current);
    __os->write(::Ice::ObjectPrx(::IceInternal::upCast(__ret.get())));
    return ::Ice::DispatchOK;
}

static ::std::string __OUC__Observing_all[] =
{
    "getTelescope",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "sayHello",
    "shutdown"
};

::Ice::DispatchStatus
OUC::Observing::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__OUC__Observing_all, __OUC__Observing_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __OUC__Observing_all)
    {
        case 0:
        {
            return ___getTelescope(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
        case 5:
        {
            return ___sayHello(in, current);
        }
        case 6:
        {
            return ___shutdown(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
OUC::Observing::__write(::IceInternal::BasicStream* __os) const
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
OUC::Observing::__read(::IceInternal::BasicStream* __is, bool __rid)
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
OUC::Observing::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type OUC::Observing was not generated with stream support";
    throw ex;
}

void
OUC::Observing::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type OUC::Observing was not generated with stream support";
    throw ex;
}

void 
OUC::__patch__ObservingPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::OUC::ObservingPtr* p = static_cast< ::OUC::ObservingPtr*>(__addr);
    assert(p);
    *p = ::OUC::ObservingPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::OUC::Observing::ice_staticId(), v->ice_id());
    }
}

bool
OUC::operator==(const ::OUC::Observing& l, const ::OUC::Observing& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
OUC::operator<(const ::OUC::Observing& l, const ::OUC::Observing& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
