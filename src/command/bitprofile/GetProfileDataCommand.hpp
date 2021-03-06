#pragma once

#include <QVariant>
#include <QVariantMap>

#include "ethrpc/Provider.hpp"
#include "bitprofile/Profile.hpp"
#include "bitprofile/Resolver.hpp"


#include "conf/Settings.hpp"
#include "conf/GetBitProfileNetwork.hpp"


namespace Xeth{


class GetProfileDataCommand 
{
    public:
        GetProfileDataCommand(Ethereum::Connector::Provider &provider, const Settings &settings);

        QVariant operator()(const QVariantMap &request);

    private:
        BitProfile::Resolver _resolver;
};


}
