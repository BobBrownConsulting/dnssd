#pragma once

#include "../common/Error.h"
#include "../common/CommonAdvertiserInterface.h"
#include "ScopedDnsServiceRef.h"

#include <map>

namespace dnssd {

    class BonjourAdvertiser : public CommonAdvertiserInterface
    {
    public:
        explicit BonjourAdvertiser() = default;

        Error registerService(const std::string& serviceName, uint16_t port, const char* name) noexcept override;
        Error registerService(const std::string& serviceName, uint16_t port, const TxtRecord& txtRecord, const char* name) noexcept override;

        void unregisterService() noexcept override;

    private:
        ScopedDnsServiceRef mServiceRef;
    };

} // namespace dnssd
