#include "inventorythread.h"
#include "databasehandler.h"

void InventoryThread::run()
{
    {
        emit done(DatabaseHandler::GetItems());
    }
}
