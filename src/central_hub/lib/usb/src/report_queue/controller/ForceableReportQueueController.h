#include "IForceableReportQueueController.h"
#include "IForceableReportQueue.h"

class ForceableReportQueueController: public IForceableReportQueueController {
    private:
    bool controllerOwner;

    protected:
    const IForceableReportQueue *reportQueue;
    const IReportQueueController *controller;

    private:
    ForceableReportQueueController(IForceableReportQueue *_reportQueue, IReportQueueController *_controller, bool _controllerOwner);

    public:
    ForceableReportQueueController() = default;

    ForceableReportQueueController(IForceableReportQueue *_reportQueue);

    ForceableReportQueueController(IForceableReportQueue *_reportQueue, IReportQueueController *_controller);

    ~ForceableReportQueueController();
    
    virtual bool forceReport(report_t &report) const;
    
    virtual bool report(report_t &report) const;

    virtual bool inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const;

    virtual bool moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const;

    virtual bool moduleDisconnectedReport(uint8_t moduleID) const;
};