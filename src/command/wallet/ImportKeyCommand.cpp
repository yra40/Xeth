#include "ImportKeyCommand.hpp"


namespace Xeth{


ImportKeyCommand::ImportKeyCommand(const Settings &settings, DataBase &database, Synchronizer &synchronizer, Notifier &notifier) : 
    _settings(settings),
    _database(database),
    _synchronizer(synchronizer),
    _notifier(notifier)
{}


QVariant ImportKeyCommand::operator()(const QVariantMap &request)
{

    if(!request.contains("file")||!request.contains("password"))
    {
        return QVariant::fromValue(false);
    }

    QString file = request["file"].toString();
    QString password = request["password"].toString();

    if(!file.length()||!password.length())
    {
        return QVariant::fromValue(false);
    }

    QString address;

    if(!importEthereumKey(file, password, address))
    {
        if(!importStealthKey(file, password, address))
        {
            if(!importPresaleKey(file, password, address))
            {
                return QVariant::fromValue(false);
            }
        }
    }

    return QVariant::fromValue(address);
}


bool ImportKeyCommand::importEthereumKey(const QString &file, const QString &password, QString &address)
{
    ImportEthereumKeyCommand command(_database, _synchronizer);
    return command.import(file, password, address);
}


bool ImportKeyCommand::importStealthKey(const QString &file, const QString &password, QString &address)
{
    ImportStealthKeyCommand command(_database, _synchronizer);
    return command.import(file, password, address);
}


bool ImportKeyCommand::importPresaleKey(const QString &file, const QString &password, QString &address)
{
    ImportPresaleKeyCommand command(_settings, _synchronizer, _database);
    return command.import(file, password, address);
}


ImportEthereumKeyCommand::ImportEthereumKeyCommand(DataBase &database, Synchronizer &synchronizer) :
    Base(database.getEthereumKeys(), synchronizer)
{}


ImportStealthKeyCommand::ImportStealthKeyCommand(DataBase &database, Synchronizer &synchronizer) :
    Base(database.getStealthKeys(), synchronizer)
{}



ImportPresaleKeyCommand::ImportPresaleKeyCommand(const Settings &settings, Synchronizer &synchronizer, DataBase &database) :
    _settings(settings),
    _synchronizer(synchronizer),
    _database(database)
{}


bool ImportPresaleKeyCommand::import(const QString &path, const QString &password, QString &address)
{

    qDebug()<<"import "<<path<<" password="<<password;

    QStringList args;
    Json::Value json;

    args.push_back("wallet");
    args.push_back("import");
    args.push_back(path);

    QProcess process;
    EthProcessInitializer::Initialize(process, _settings, args);

    process.start();
    process.waitForStarted();

    process.write(password.toLocal8Bit());
    process.write("\n");
    process.waitForFinished();

    if(process.exitCode()!=0)
    {
        qDebug()<<process.readAllStandardError();
        return false;
    }
    else
    {
        JsonReader reader;

        if(!reader.read(path.toStdString().c_str(), json))
        {
            return false;
        }
        else
        {
            std::string addr = json["ethaddr"].asString();
            _synchronizer.watchAddress(addr);
            address = addr.c_str();
            _database.getEthereumKeys().touch(addr.c_str());
        }
    }

    return true;
}


bool ImportPresaleKeyCommand::import(const QVariantMap &request, QString &address)
{
    if(!request.contains("file")||!request.contains("password"))
    {
        return false;
    }

    return import(request["file"].toString(),  request["password"].toString(), address);
}


QVariant ImportPresaleKeyCommand::operator ()(const QVariantMap &request)
{
    QString address;
    if(!import(request, address))
    {
        return QVariant::fromValue(false);
    }
    return QVariant::fromValue(address);
}


}
