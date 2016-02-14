#include "FacadeLinker.hpp"


namespace Xeth{


FacadeLinker::FacadeLinker(QWebFrame *frame, Facade *facade) :
    _frame(frame),
    _facade(facade)
{

}

void FacadeLinker::linkEvents()
{
    _frame->addToJavaScriptWindowObject("events", &_facade->getNotifier());
}

void FacadeLinker::linkAll()
{
    linkEvents();
    linkObjects();
}

void FacadeLinker::linkObjects()
{
    _frame->addToJavaScriptWindowObject("wallet", &_facade->getWallet());
    _frame->addToJavaScriptWindowObject("addressbook", &_facade->getAddressBook());
    _frame->addToJavaScriptWindowObject("progress", &_facade->getProgress());
    _frame->addToJavaScriptWindowObject("convert", &_facade->getConverter());
    _frame->addToJavaScriptWindowObject("config", &_facade->getConfig());
}


}
