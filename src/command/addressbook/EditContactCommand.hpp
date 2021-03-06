#pragma once

#include <QVariant>
#include <QVariantMap>

#include "database/DataBase.hpp"


namespace Xeth{



class EditContactCommand
{
    public:
        EditContactCommand(DataBase &);

        QVariant operator()(const QVariantMap &);

    private:
        DataBase &_database;

};



}
