#include "ForceableReportQueue.h"
#include "PicoQueueReportQueue.h"

ForceableReportQueue::ForceableReportQueue(IReportQueue *_reportQueue, IReportQueue *_forcedReports):
    reportQueue(_reportQueue), forcedReports(_forcedReports) {}

ForceableReportQueue::~ForceableReportQueue() {
    delete this->reportQueue;
    delete this->forcedReports;
}

bool ForceableReportQueue::forcePush(const report_t &report) const {
    return this->forcedReports->push(report);
}

bool ForceableReportQueue::push(const report_t &report) const {
    report_t forcedReport;
    // priority goes to reports that have been forced onto the queue
    if (this->forcedReports->pop(&forcedReport)) {
        this->reportQueue->push(forcedReport);
        // we only want to push 1 report at a time
        return false;
    } else {
        // no reports have been forced, push as normal
        return this->reportQueue->push(report);
    }
}

bool ForceableReportQueue::pop(report_t *report) const {
    return this->reportQueue->pop(report);
}