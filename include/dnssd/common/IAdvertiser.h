#pragma once

#include "../common/TxtRecord.h"
#include "Result.h"
#include "Util.h"

#include <functional>

namespace dnssd
{

/**
 * Interface class which represents a dnssd advertiser object, which is able to present itself onto the network.
 */
class IAdvertiser
{
public:
    explicit IAdvertiser() = default;
    virtual ~IAdvertiser() = default;

    /**
     * Registers a service with given arguments.
     *
     * @param regType The service type followed by the protocol, separated by a dot (e.g. "_ftp._tcp"). The service type
     * must be an underscore, followed by 1-15 characters, which may be letters, digits, or hyphens. The transport
     * protocol must be "_tcp" or "_udp".
     * @param name If non-NULL, specifies the service name to be registered. Most applications will not specify a name,
     * in which case the computer name is used (this name is communicated to the client via the callback).
     * @param domain If non-NULL, specifies the domain on which to advertise the service. Most applications will not
     * specify a domain, instead automatically registering in the default domain(s).
     * @param port The port of the service.
     * on the computer name which in most cases makes sense to do.
     * @param txtRecord A TXT record to add to the service, consisting of a couple of keys and values.
     * @return An Result indicating success or failure.
     */
    virtual Result registerService (
        const std::string& regType,
        const char* name,
        const char* domain,
        uint32_t interfaceIndex,
        uint16_t port,
        const TxtRecord& txtRecord) noexcept = 0;

    /**
     * Updates the TXT record of this service. The given TXT record will replace the previous one.
     * @param txtRecord The new TXT record.
     * @return An Result indicating success or failure.
     */
    virtual Result updateTxtRecord (const TxtRecord& txtRecord) = 0;

    /**
     * Unregisters this service from the mDnsResponder, after which the service will no longer be found on the network.
     */
    virtual void unregisterService() noexcept = 0;

    /**
     * Called when there was an error.
     * Note: this call will be made from a background thread and wil not be synchronised.
     * @param error The error which occured.
     */
    virtual void onAdvertiserErrorAsync (const Result& error) { ignore (error); }
};

} // namespace dnssd
