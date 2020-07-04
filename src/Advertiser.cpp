#include <dnssd/Advertiser.h>

#include <iostream>
#include <thread>

#include <dns_sd.h>

using namespace dnssd;

static void DNSSD_API registerServiceCallBack(DNSServiceRef serviceRef, DNSServiceFlags flags,
                                              DNSServiceErrorType errorCode, const char* serviceName,
                                              const char* regType, const char* replyDomain, void* context)
{
    (void) serviceRef;
    (void) flags;
    (void) serviceName;
    (void) regType;
    (void) replyDomain;

    Error error = { errorCode };

    if (error)
    {
        Advertiser* owner = static_cast<Advertiser*>(context);
        owner->callObserver([error](const Advertiser::Observer& observer){ observer.onAdvertiserError(error); });
        owner->unregisterService();
        return;
    }
}

Advertiser::~Advertiser()
{
    unregisterService();
}

Error Advertiser::registerService(const std::string& serviceName, uint16_t port) noexcept
{
    const Error error = DNSServiceRegister(&mServiceRef, 0, 0, nullptr, serviceName.c_str(), nullptr, nullptr,
                                         htons(port), 0, nullptr, registerServiceCallBack, this);

    if (error) { return error; }

    return DNSServiceProcessResult(mServiceRef);
}

Error Advertiser::registerService(
    const std::string &serviceName, uint16_t port,
    const TXTRecord &txtRecord) noexcept
{

    std::cout << "Register service (thread id: " << std::this_thread::get_id() << ")" << std::endl;

    const Error error = DNSServiceRegister(&mServiceRef, 0, 0, nullptr, serviceName.c_str(), nullptr, nullptr,
                                         htons(port), txtRecord.length(), txtRecord.bytesPtr(), registerServiceCallBack,
                                         this);
    if (error) { return error; }

    return DNSServiceProcessResult(mServiceRef);
}

Error Advertiser::registerService(
    const std::string &serviceName, uint16_t port,
    const std::map<std::string, std::string>& keysValues) noexcept
{
    TXTRecord txtRecord = TXTRecord();

    for (auto& keyValue : keysValues)
    {
        txtRecord.setValue(keyValue.first, keyValue.second);
    }

    const Error error = DNSServiceRegister(&mServiceRef, 0, 0, nullptr, serviceName.c_str(), nullptr, nullptr,
                                         htons(port), txtRecord.length(), txtRecord.bytesPtr(), registerServiceCallBack,
                                         this);
    if (error) { return error; }

    return DNSServiceProcessResult(mServiceRef);
}


void Advertiser::unregisterService() noexcept
{
    if (mServiceRef) {
        DNSServiceRefDeallocate(mServiceRef);
        mServiceRef = nullptr;
    }
}

void Advertiser::callObserver(std::function<void(Observer&)> callback) noexcept
{
    if (mObserver != nullptr) { callback(*mObserver); }
}
