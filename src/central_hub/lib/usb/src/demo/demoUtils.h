#pragma once
#include "ModuleUpdateHandler.h"
#include "ButtonPayload.h"
#include "JoystickPayload.h"
#include "IForceableReportQueueController.h"

bool toggleJoystickConnection(ModuleUpdateHandler<JoystickPayload> **joystickModule, IForceableReportQueueController *controller, bool &connected);

bool toggleButtonConnection(ModuleUpdateHandler<ButtonPayload> **buttonModule, IForceableReportQueueController *controller, bool &connected);

bool updateButtonModule(ModuleUpdateHandler<ButtonPayload> *buttonModule);

bool updateJoystickModule(ModuleUpdateHandler<JoystickPayload> *joystickModule);