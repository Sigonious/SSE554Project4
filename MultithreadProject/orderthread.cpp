#include "orderthread.h"
#include "databasehandler.h"

void OrderThread::run()
{
    {
        emit done(DatabaseHandler::GetOrders());
    }
}
