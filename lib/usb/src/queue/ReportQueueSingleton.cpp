#include "ReportQueueSingleton.h"
#include "ReportQueueHandler.h"
#include "ReportQueueController.h"
#include "PicoQueueReportQueue.h"
#include "pico/util/queue.h"

IReportQueue* getInputQueue() {
    static queue_t _inputQueue;
    static IReportQueue *inputQueue;
    static bool inputQueueInitialized = false;

    if (!inputQueueInitialized) {
        inputQueue = new PicoQueueReportQueue(&_inputQueue);
        inputQueueInitialized = true;
    }
    return inputQueue;
}

IReportQueue* getConnectionQueue() {
    static queue_t _connectionQueue;
    static IReportQueue *connectionQueue;
    static bool connectionQueueInitialized = false;

    if (!connectionQueueInitialized) {
        connectionQueue = new PicoQueueReportQueue(&_connectionQueue);
        connectionQueueInitialized = true;
    }
    return connectionQueue;
}

IReportQueueController* ReportQueueSingleton::getController() {
    static IReportQueueController *controller;
    static bool controllerInitialized = false;

    if (!controllerInitialized) {
        controller = new ReportQueueController(
            getInputQueue(),
            getConnectionQueue());
        controllerInitialized = true;
    }
    return controller;
}

IReportQueueHandler* ReportQueueSingleton::getHandler() {
    static IReportQueueHandler *handler;
    static bool handlerInitialized = false;

    if (!handlerInitialized) {
        handler = new ReportQueueHandler(
            getInputQueue(),
            getConnectionQueue());
        handlerInitialized = true;
    }
    return handler;
}