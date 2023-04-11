import { app, BrowserWindow, shell, ipcMain } from "electron";
import { release } from "node:os";
import { join } from "node:path";
const ViGEmClient = require("vigemclient");
const HID = require("node-hid");

const Store = require("electron-store");

const store = new Store();

let client = new ViGEmClient();
let controller;
let controllerListen;
let controllerListenRestart;
let controllerFound = false;
let MACS_CONTROLLER;
let macsReports;
let buttons;
let axes;
let configuration = [];
let count = 0;
let configs;

let id = 1;

function typeFromAddress(address: number): string | undefined {
  switch (address & 7) {
    case 0:
      return "button";
    case 1:
      return "switch";
    case 2:
      return "slider";
    case 3:
      return "dial";
    case 4:
      return "joystick";
    default:
      return undefined;
  }
}

function configFromMemory(id: number) {
  const foundConfig = configs.find((config: any) => config.id === id);
  if (foundConfig) {
    console.log("found config");
    console.log(foundConfig.configuration);
    return foundConfig.configuration;
  } else {
    console.log("no config found");
    switch (id & 7) {
      case 2:
      case 3:
        return {
          input: {
            start: 0,
            end: 100,
          },
        };
      case 4:
        return {
          behavior: "default",
        };
      default:
        return {};
    }
  }
}

// The built directory structure
//
// ├─┬ dist-electron
// │ ├─┬ main
// │ │ └── index.js    > Electron-Main
// │ └─┬ preload
// │   └── index.js    > Preload-Scripts
// ├─┬ dist
// │ └── index.html    > Electron-Renderer
//
process.env.DIST_ELECTRON = join(__dirname, "../");
process.env.DIST = join(process.env.DIST_ELECTRON, "../dist");
process.env.PUBLIC = process.env.VITE_DEV_SERVER_URL
  ? join(process.env.DIST_ELECTRON, "../public")
  : process.env.DIST;

// Disable GPU Acceleration for Windows 7
if (release().startsWith("6.1")) app.disableHardwareAcceleration();

// Set application name for Windows 10+ notifications
if (process.platform === "win32") app.setAppUserModelId(app.getName());

if (!app.requestSingleInstanceLock()) {
  app.quit();
  process.exit(0);
}

// Remove electron security warnings
// This warning only shows in development mode
// Read more on https://www.electronjs.org/docs/latest/tutorial/security
// process.env['ELECTRON_DISABLE_SECURITY_WARNINGS'] = 'true'

let win: BrowserWindow | null = null;
// Here, you can also use other preload
const preload = join(__dirname, "../preload/index.js");
const url = process.env.VITE_DEV_SERVER_URL;
const indexHtml = join(process.env.DIST, "index.html");

async function createWindow() {
  win = new BrowserWindow({
    title: "MACS Controller",
    icon: join(process.env.PUBLIC, "app-logo.png"),
    webPreferences: {
      devTools: true,
      preload,
      // Warning: Enable nodeIntegration and disable contextIsolation is not secure in production
      // Consider using contextBridge.exposeInMainWorld
      // Read more on https://www.electronjs.org/docs/latest/tutorial/context-isolation
      nodeIntegration: true,
      contextIsolation: false,
    },
    frame: false,
    autoHideMenuBar: true,
    minWidth: 1000,
    minHeight: 600,
    width: 1000,
    height: 600,
  });

  if (process.env.VITE_DEV_SERVER_URL) {
    // electron-vite-vue#298
    win.loadURL(url);
    // Open devTool if the app is not packaged
    win.webContents.openDevTools();
  } else {
    win.loadFile(indexHtml);
  }

  const listenerLoop = () => {
    // read the controller data;
    console.log("searching for controller");
    processMACS();
  };

  const isAButton = (inputRequested) => {
    switch (inputRequested) {
      case "START":
      case "BACK":
      case "LEFT_THUMB":
      case "RIGHT_THUMB":
      case "LEFT_SHOULDER":
      case "RIGHT_SHOULDER":
      case "GUIDE":
      case "A":
      case "B":
      case "X":
      case "Y":
        return true;
      default:
        return false;
    }
  };

  const processMACS = () => {
    try {
      MACS_CONTROLLER = HID.devices(0xcafe, 0x0000);
      console.log(MACS_CONTROLLER);
      if (MACS_CONTROLLER.length > 0 && !controllerFound) {
        macsReports = new HID.HID(MACS_CONTROLLER[0].path); // connected
        macsReports.on("data", function (data) {
          switch (data.readInt8(0)) {
            case 1:
              console.log("connected");
              console.log({
                id: data.readInt8(1),
                q: 0 - (0 - data.readInt8(2) + (0 - data.readInt8(3))),
                r: 0 - data.readInt8(3),
                s: 0 - data.readInt8(2),
                moduleType: typeFromAddress(data.readInt8(1)),
                configuration: configFromMemory(data.readInt8(1)),
              });
              win?.webContents.send("module_connected", {
                id: data.readInt8(1),
                q: 0 - (0 - data.readInt8(2) + (0 - data.readInt8(3))),
                r: 0 - data.readInt8(3),
                s: 0 - data.readInt8(2),
                moduleType: typeFromAddress(data.readInt8(1)),
                configuration: configFromMemory(data.readInt8(1)),
              });
              break;
            case 2:
              console.log("removed");
              win?.webContents.send("module_removed", {
                id: data.readInt8(1),
                q: 0 - (0 - data.readInt8(2) + (0 - data.readInt8(3))),
                r: 0 - data.readInt8(3),
                s: 0 - data.readInt8(2),
              });
              break;
            case 3:
              console.log("buttonState: " + data.readInt8(2));
              try {
                if (controller?.type) {
                  configuration?.forEach((module, index) => {
                    if (module?.id === data.readInt8(1)) {
                      console.log("found");
                      const buttonState = data.readInt8(2);
                      if (isAButton(module?.configuration?.input)) {
                        controller?.button[
                          buttons[getInputIndex(module?.configuration?.input)]
                        ].setValue(buttonState);
                        controller?.update();
                      } else {
                        controller?.axis[
                          axes[getAxisIndex(module?.configuration?.input)]
                        ].setValue(buttonState);
                        controller?.update();
                      }
                    } else {
                      // console.log("not found");
                    }
                  });
                } else {
                  // console.log("controller is null");
                }
              } catch (e) {
                // controller is not on
              }
              break;
            case 4:
              const moduleID = data.readInt8(1);
              const analogValue = data.readUInt8(2);
              console.log("analog: " + analogValue);
              try {
                if (controller?.type) {
                  configuration?.forEach((module, index) => {
                    if (module?.id === data.readInt8(1)) {
                      const buttonState = data.readInt8(2);
                      if (isAButton(module?.configuration?.input?.input)) {
                        controller?.button[
                          buttons[
                            getInputIndex(module?.configuration?.input?.input)
                          ]
                        ].setValue(buttonState / 255);
                        controller?.update();
                      } else {
                        if (
                          getAxisDirection(
                            module?.configuration?.input?.input
                          ) !== 0
                        ) {
                          // console.log("in switch");
                          switch (module?.configuration?.input?.input) {
                            case "RIGHT_JOYSTICK_UP":
                              controller?.axis?.rightY?.(analogValue / 255);
                              controller?.update();
                              break;
                            case "RIGHT_JOYSTICK_DOWN":
                              controller?.axis?.rightY?.setValue(
                                0 - analogValue / 255
                              );
                              controller?.update();
                              break;
                            case "RIGHT_JOYSTICK_LEFT":
                              controller?.axis?.rightX?.setValue(
                                0 - analogValue / 255
                              );
                              controller?.update();
                              break;
                            case "RIGHT_JOYSTICK_RIGHT":
                              console.log("in right joystick right");
                              console.log(analogValue / 255);
                              controller?.axis?.rightX?.setValue(
                                analogValue / 255
                              );
                              controller?.update();
                              break;
                            case "LEFT_JOYSTICK_UP":
                              controller?.axis?.leftY?.setValue(
                                analogValue / 255
                              );
                              controller?.update();
                              break;
                            case "LEFT_JOYSTICK_DOWN":
                              controller?.axis?.leftY.setValue(
                                0 - analogValue / 255
                              );
                              controller?.update();
                              break;
                            case "LEFT_JOYSTICK_LEFT":
                              controller?.axis?.leftX.setValue(
                                0 - analogValue / 255
                              );
                              controller?.update();
                              break;
                            case "LEFT_JOYSTICK_RIGHT":
                              controller?.axis?.leftY?.setValue(
                                analogValue / 255
                              );
                              controller?.update();
                              break;
                            default:
                              break;
                          }
                        } else {
                          controller?.axis[
                            axes[
                              getAxisIndex(module?.configuration?.input?.input)
                            ]
                          ].setValue((analogValue - 128) / 128);
                          controller?.update();
                        }
                      }
                    } else {
                      // console.log("not found");
                    }
                  });
                } else {
                  // console.log("controller is null");
                }
              } catch (e) {
                // controller is not on
              }
              break;
            case 5:
              // console.log(data);
              const joystickHorizontal = data.readUInt8(2);
              const joystickVertical = data.readUInt8(3);
              const joystickButtonState = data.readUInt8(4);

              const x = (joystickHorizontal - 128) / 128;
              const y = (joystickVertical - 128) / 128;
              console.log(joystickButtonState);
              // console.log(joystickHorizontal);
              // console.log(joystickVertical);
              console.log(
                0 - Math.cos((Math.PI / 6) * x - y * Math.sin(Math.PI / 6))
              );
              console.log(
                y * Math.cos(Math.PI / 6) + x * Math.sin(Math.PI / 6)
              );
              // console.log("before loop");
              try {
                if (controller?.type) {
                  // console.log("entered loop");
                  configuration?.forEach((module, index) => {
                    if (module?.id === data.readInt8(1)) {
                      // console.log(module);
                      // console.log(module?.configuration?.input);
                      if (module?.configuration?.behavior === "default") {
                        // controller.axis.leftX.setValue(0.5); // move left stick 50% to the left
                        // controller.axis.leftY.setValue(-0.5); // move left stick 50% down
                        controller.axis.leftX.setValue(
                          0 -
                            Math.cos(Math.PI / 6) * x -
                            y * Math.sin(Math.PI / 6)
                        );
                        controller.axis.leftY.setValue(
                          y * Math.cos(Math.PI / 6) + x * Math.sin(Math.PI / 6)
                        );
                        controller?.update();
                      }
                    } else {
                      // console.log("not found");
                    }
                  });
                } else {
                  // console.log("controller is null");
                }
              } catch (e) {
                // controller is not on
              }
              break;
            default:
              console.log("unknown");
              break;
          }
        });
        macsReports.on("error", function (error) {
          console.log(error);
          win?.webContents.send("controller_found", false);
          controllerFound = false;
          controllerListenRestart = setInterval(() => listenerLoop(), 1000);
        });
        win?.webContents.send("controller_found", true);
        controllerFound = true;
        console.log("controller found");
        clearInterval(controllerListen);
        clearInterval(controllerListenRestart);
      } else {
        controllerFound = false;
        win?.webContents.send("controller_found", false);
        console.log("controller not found");
      }
    } catch (e) {
      controllerFound = false;
      console.log("controller not found");
      win?.webContents.send("controller_found", false);
    }
  };

  // Test actively push message to the Electron-Renderer
  win.webContents.on("did-finish-load", () => {
    configs = store.get("configuration");
    controllerListen = setInterval(() => {
      console.log("searching for controller");
      // read the controller data
      processMACS();
    }, 1000);
    win?.webContents.send("main-process-message", new Date().toLocaleString());
    console.log("did-finish-load");
  });
  // Make all links open with the browser, not with the application
  win.webContents.setWindowOpenHandler(({ url }) => {
    if (url.startsWith("https:")) shell.openExternal(url);
    return { action: "deny" };
  });
}

app.whenReady().then(createWindow);

app.on("window-all-closed", () => {
  win = null;
  if (process.platform !== "darwin") app.quit();
});

app.on("second-instance", () => {
  if (win) {
    // Focus on the main window if the user tried to open another
    if (win.isMinimized()) win.restore();
    win.focus();
  }
});

app.on("activate", () => {
  const allWindows = BrowserWindow.getAllWindows();
  if (allWindows.length) {
    allWindows[0].focus();
  } else {
    createWindow();
  }
});

// New window example arg: new windows url
ipcMain.handle("open-win", (_, arg) => {
  const childWindow = new BrowserWindow({
    webPreferences: {
      preload,
      nodeIntegration: true,
      contextIsolation: false,
    },
  });

  if (process.env.VITE_DEV_SERVER_URL) {
    childWindow.loadURL(`${url}#${arg}`);
  } else {
    childWindow.loadFile(indexHtml, { hash: arg });
  }
});

ipcMain.on("initialize", (event, arg) => {
  // console.log(arg);
  configuration = arg;
  event.sender.send("initialized", true);
});

ipcMain.on("save_config", (event, arg) => {
  // console.log(arg);

  configuration = arg;
  store.set("configuration", arg);
  console.log(configuration);
  try {
    controller.disconnect();
    clearInterval(controllerListen);
  } catch (e) {
    event.sender.send("controller_error", e);
  }
  event.sender.send("controller_disconnected", true);
  console.log("controller disconnected");
  event.sender.send("config_saved", true);
});

const getInputIndex = (input) => {
  switch (input) {
    case "START":
      return 0;
    case "BACK":
      return 1;
    case "LEFT_THUMB":
      return 2;
    case "RIGHT_THUMB":
      return 3;
    case "LEFT_SHOULDER":
      return 4;
    case "RIGHT_SHOULDER":
      return 5;
    case "GUIDE":
      return 6;
    case "A":
      return 7;
    case "B":
      return 8;
    case "X":
      return 9;
    case "Y":
      return 10;
    default:
      return;
  }
};

const getAxisIndex = (axis) => {
  switch (axis) {
    case "LEFT_TRIGGER":
      return 4;
    case "RIGHT_TRIGGER":
      return 5;
    case "RIGHT_JOYSTICK_UP":
      return 3;
    case "RIGHT_JOYSTICK_DOWN":
      return 3;
    case "RIGHT_JOYSTICK_LEFT":
      return 2;
    case "RIGHT_JOYSTICK_RIGHT":
      return 2;
    case "LEFT_JOYSTICK_UP":
      return 1;
    case "LEFT_JOYSTICK_DOWN":
      return 1;
    case "LEFT_JOYSTICK_LEFT":
      return 0;
    case "LEFT_JOYSTICK_RIGHT":
      return 0;
    default:
      return;
  }
};

const getAxisDirection = (axis) => {
  switch (axis) {
    case "RIGHT_JOYSTICK_UP":
      return 1;
    case "RIGHT_JOYSTICK_DOWN":
      return -1;
    case "RIGHT_JOYSTICK_LEFT":
      return -1;
    case "RIGHT_JOYSTICK_RIGHT":
      return 1;
    case "LEFT_JOYSTICK_UP":
      return 1;
    case "LEFT_JOYSTICK_DOWN":
      return -1;
    case "LEFT_JOYSTICK_LEFT":
      return -1;
    case "LEFT_JOYSTICK_RIGHT":
      return 1;
    default:
      return 0;
  }
};

ipcMain.on("handle_controller", (event, arg) => {
  console.log(configuration);
  const { start, disconnect } = arg;

  if (start) {
    try {
      if (!controller) {
        client?.connect(); // establish connection to the ViGEmBus driver
      }
    } catch (e) {
      event.sender.send("controller_error", e);
    }
    console.log("client started");

    try {
      console.log(client);
      controller = client?.createX360Controller();
      controller?.connect(); // plug in the virtual controller
    } catch (e) {
      event.sender.send("controller_error", e);
    }
    console.log("controller connected");
    event.sender.send("controller_started", true);
    let t = 0;

    buttons = Object.keys(controller.button);
    axes = Object.keys(controller.axis);
    console.log(buttons);
    console.log(axes);
    // controllerListen = setInterval(() => {
    //   // read the controller data
    //   console.log(controller.button.A.value);
    //   controller.button.A.setValue(!controller.button.A.value);
    //   controller.update();
    // }, 500);
    // try {
  }
  if (disconnect) {
    try {
      controller.disconnect();
      // controller = null;
      console.log(controller);
      console.log("controller disconnected");
      clearInterval(controllerListen);
    } catch (e) {
      console.log(e);
      event.sender.send("controller_error", e);
    }
    event.sender.send("controller_disconnected", true);
  }
});

ipcMain.on("app/close", () => {
  // if (MACS_CONTROLLER)
  //   MACS_CONTROLLER.forEach((localDevice) => localDevice.close());
  app.quit();
});

ipcMain.on("app/minimize", () => {
  win.minimize();
});
