import { app, BrowserWindow, shell, ipcMain } from "electron";
import { release } from "node:os";
import { join } from "node:path";
const ViGEmClient = require("vigemclient");
const HID = require("node-hid");

let client = new ViGEmClient();
let controller;
let controllerListen;
let controllerListenRestart;
let controllerFound = false;
let MACS_CONTROLLER;
let testDevice;
let testDevice2;
let testDevice3;
let buttons;
let axes;
let configuration = [];

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
      devTools: false,
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

  // win.webContents.session.on(
  //   "select-hid-device",
  //   (event, details, callback) => {
  //     if ("hid" in navigator) {
  //       // The WebHID API is supported.
  //       console.log("please");
  //     }
  //     //Add events to handle devices being added or removed before the callback on
  //     //`select-hid-device` is called.
  //     // win.webContents.session.on("hid-device-added", (event, device) => {
  //     //   console.log("hid-device-added FIRED WITH", device);
  //     //   //Optionally update details.deviceList
  //     // });

  //     // win.webContents.session.on("hid-device-removed", (event, device) => {
  //     //   console.log("hid-device-removed FIRED WITH", device);
  //     //   //Optionally update details.deviceList
  //     // });

  //     event.preventDefault();
  //     if (details.deviceList && details.deviceList.length > 0) {
  //       callback(details.deviceList[0].deviceId);
  //     }
  //   }
  // );

  // win.webContents.session.setPermissionCheckHandler(
  //   (webContents, permission, requestingOrigin, details) => {
  //     if (permission === "hid" && details.securityOrigin === "file:///") {
  //       return true;
  //     }
  //   }
  // );

  // win.webContents.session.setDevicePermissionHandler((details) => {
  //   console.log("ran from permission");
  //   if (details.deviceType === "hid" && details.origin === "file://") {
  //     return true;
  //   }
  // });

  const listenerLoop = () => {
    // read the controller data
    try {
      MACS_CONTROLLER = new HID.devices(0xcafe, 0x0000);
      if (MACS_CONTROLLER.length > 0 && !controllerFound) {
        testDevice = new HID.HID(MACS_CONTROLLER[2].path);
        testDevice2 = new HID.HID(MACS_CONTROLLER[0].path);
        win?.webContents.send("controller_found", true);
        testDevice.on("data", function (data) {
          // this is where we update the virtual controller if it is on
          /*
              state = {
                a: data.readUInt8(Buttons.A),
                b: data.readUInt8(Buttons.B),
                x: data.readUInt8(Buttons.X),
                y: data.readUInt8(Buttons.Y),
                leftStick: {
                  xRaw: data.readInt8(Buttons.LeftStickX),
                  yRaw: data.readInt8(Buttons.LeftStickY),
                  x: 0,
                  y: 0,
                  module: 0,
                  angle: 0,
                },
                rightStick: {
                  xRaw: data.readInt8(Buttons.RightStickX),
                  yRaw: data.readInt8(Buttons.RightStickY),
                  x: 0,
                  y: 0,
                  module: 0,
                  angle: 0,
                },
                r2: data.readUInt8(Buttons.R2),
                l2: data.readUInt8(Buttons.L2),
              };
              */

          // need to parse the data
          console.log(data.readInt8(2));
          // console.log("before loop");
          try {
            if (controller?.type) {
              // console.log("entered loop");
              configuration?.forEach((module, index) => {
                if (module?.index === 7) {
                  // console.log(module);
                  // console.log(module?.configuration?.input);
                  const buttonState = data.readInt8(2);
                  // console.log(buttons);
                  controller?.button[
                    buttons[getInputIndex(module?.configuration?.input)]
                  ].setValue(buttonState);
                  controller?.update();
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

          // count++;
          // if (controller && controller?.productID && controller?.vendorID) {
          //   const buttonState = data.readUInt8(5);
          //   console.log(buttonState);
          //   controller.button.A.setValue(buttonState);
          //   controller.update();
          // }
          // controller.axis.leftX.setValue();
          // controller.axis.leftY.setValue();
          // controller.axis.rightX.setValue();
          // controller.axis.rightY.setValue();
          // controller.axis.dpadHorz.setValue();
          // controller.axis.dpadVert.setValue();
          // buttons.forEach((btn) => {
          //   if (btn != "GUIDE") {
          //     // otherwise Win 10 spams the GameBar
          //     // the set value comes from the parsed data
          //     controller.button[buttons[btn]].setValue(data); // invert button value
          //   }
          // });
          // controller.update(); // update manually for better performance
          // }
        });
        // } catch (error) {
        //   console.log("controller disconnected");
        // }
        testDevice.on("error", function (error) {
          console.log(error);
          controllerFound = false;
          win?.webContents.send("controller_found", false);
        });
        testDevice2.on("data", function (data) {
          // this is where we update the virtual controller if it is on
          /*
              state = {
                a: data.readUInt8(Buttons.A),
                b: data.readUInt8(Buttons.B),
                x: data.readUInt8(Buttons.X),
                y: data.readUInt8(Buttons.Y),
                leftStick: {
                  xRaw: data.readInt8(Buttons.LeftStickX),
                  yRaw: data.readInt8(Buttons.LeftStickY),
                  x: 0,
                  y: 0,
                  module: 0,
                  angle: 0,
                },
                rightStick: {
                  xRaw: data.readInt8(Buttons.RightStickX),
                  yRaw: data.readInt8(Buttons.RightStickY),
                  x: 0,
                  y: 0,
                  module: 0,
                  angle: 0,
                },
                r2: data.readUInt8(Buttons.R2),
                l2: data.readUInt8(Buttons.L2),
              };
              */
          // need to parse the data
          // console.log(data);
          // count++;
          // if (controller && controller?.productID && controller?.vendorID) {
          //   const buttonState = data.readUInt8(5);
          //   console.log(buttonState);
          //   controller.button.A.setValue(buttonState);
          //   controller.update();
          // }
          // controller.axis.leftX.setValue();
          // controller.axis.leftY.setValue();
          // controller.axis.rightX.setValue();
          // controller.axis.rightY.setValue();
          // controller.axis.dpadHorz.setValue();
          // controller.axis.dpadVert.setValue();
          // buttons.forEach((btn) => {
          //   if (btn != "GUIDE") {
          //     // otherwise Win 10 spams the GameBar
          //     // the set value comes from the parsed data
          //     controller.button[buttons[btn]].setValue(data); // invert button value
          //   }
          // });
          // controller.update(); // update manually for better performance
          // }
        });
        // } catch (error) {
        //   console.log("controller disconnected");
        // }
        testDevice2.on("error", function (error) {
          console.log(error);
          controllerFound = false;
          win?.webContents.send("controller_found", false);
        });
        controllerFound = true;
        win?.webContents.send("controller_found", true);
        clearInterval(controllerListen);
        clearInterval(controllerListenRestart);
      } else if (controllerFound && MACS_CONTROLLER.length > 0) {
        clearInterval(controllerListen);
        clearInterval(controllerListenRestart);
      } else {
        win?.webContents.send("controller_found", false);
      }
    } catch (e) {
      console.log("controller not found");
    }
  };

  // Test actively push message to the Electron-Renderer
  win.webContents.on("did-finish-load", () => {
    const devices = HID.devices();
    console.log(devices);
    let count = 0;
    controllerListen = setInterval(() => {
      console.log("loop");
      // read the controller data
      try {
        MACS_CONTROLLER = new HID.devices(0xcafe, 0x0000);
        if (MACS_CONTROLLER.length > 0 && !controllerFound) {
          testDevice = new HID.HID(MACS_CONTROLLER[2].path);
          testDevice2 = new HID.HID(MACS_CONTROLLER[0].path);
          testDevice3 = new HID.HID(MACS_CONTROLLER[4].path);
          win?.webContents.send("controller_found", true);
          testDevice.on("data", function (data) {
            // this is where we update the virtual controller if it is on
            /*
                state = {
                  a: data.readUInt8(Buttons.A),
                  b: data.readUInt8(Buttons.B),
                  x: data.readUInt8(Buttons.X),
                  y: data.readUInt8(Buttons.Y),
                  leftStick: {
                    xRaw: data.readInt8(Buttons.LeftStickX),
                    yRaw: data.readInt8(Buttons.LeftStickY),
                    x: 0,
                    y: 0,
                    module: 0,
                    angle: 0,
                  },
                  rightStick: {
                    xRaw: data.readInt8(Buttons.RightStickX),
                    yRaw: data.readInt8(Buttons.RightStickY),
                    x: 0,
                    y: 0,
                    module: 0,
                    angle: 0,
                  },
                  r2: data.readUInt8(Buttons.R2),
                  l2: data.readUInt8(Buttons.L2),
                };
                */

            // need to parse the data
            // console.log(count);
            // count++;
            // console.log("before loop");
            try {
              if (controller?.type) {
                // console.log("entered loop");
                configuration?.forEach((module, index) => {
                  if (module?.index === 7) {
                    // console.log(module);
                    // console.log(module?.configuration?.input);
                    const buttonState = data.readInt8(2);
                    // console.log(buttons);
                    controller?.button[
                      buttons[getInputIndex(module?.configuration?.input)]
                    ].setValue(buttonState);
                    controller?.update();
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

            // count++;
            // if (controller && controller?.productID && controller?.vendorID) {
            //   const buttonState = data.readUInt8(5);
            //   console.log(buttonState);
            //   controller.button.A.setValue(buttonState);
            //   controller.update();
            // }
            // controller.axis.leftX.setValue();
            // controller.axis.leftY.setValue();
            // controller.axis.rightX.setValue();
            // controller.axis.rightY.setValue();
            // controller.axis.dpadHorz.setValue();
            // controller.axis.dpadVert.setValue();
            // buttons.forEach((btn) => {
            //   if (btn != "GUIDE") {
            //     // otherwise Win 10 spams the GameBar
            //     // the set value comes from the parsed data
            //     controller.button[buttons[btn]].setValue(data); // invert button value
            //   }
            // });
            // controller.update(); // update manually for better performance
            // }
          });
          testDevice.on("error", function (error) {
            console.log(error);
            win?.webContents.send("controller_found", false);
            controllerFound = false;
            controllerListenRestart = setInterval(() => listenerLoop(), 1000);
          });
          testDevice2.on("data", function (data) {
            // this is where we update the virtual controller if it is on
            /*
                state = {
                  a: data.readUInt8(Buttons.A),
                  b: data.readUInt8(Buttons.B),
                  x: data.readUInt8(Buttons.X),
                  y: data.readUInt8(Buttons.Y),
                  leftStick: {
                    xRaw: data.readInt8(Buttons.LeftStickX),
                    yRaw: data.readInt8(Buttons.LeftStickY),
                    x: 0,
                    y: 0,
                    module: 0,
                    angle: 0,
                  },
                  rightStick: {
                    xRaw: data.readInt8(Buttons.RightStickX),
                    yRaw: data.readInt8(Buttons.RightStickY),
                    x: 0,
                    y: 0,
                    module: 0,
                    angle: 0,
                  },
                  r2: data.readUInt8(Buttons.R2),
                  l2: data.readUInt8(Buttons.L2),
                };
                */
            // need to parse the data
            // console.log(data);
            // count++;
            // if (controller && controller?.productID && controller?.vendorID) {
            //   const buttonState = data.readUInt8(5);
            //   console.log(buttonState);
            //   controller.button.A.setValue(buttonState);
            //   controller.update();
            // }
            // controller.axis.leftX.setValue();
            // controller.axis.leftY.setValue();
            // controller.axis.rightX.setValue();
            // controller.axis.rightY.setValue();
            // controller.axis.dpadHorz.setValue();
            // controller.axis.dpadVert.setValue();
            // buttons.forEach((btn) => {
            //   if (btn != "GUIDE") {
            //     // otherwise Win 10 spams the GameBar
            //     // the set value comes from the parsed data
            //     controller.button[buttons[btn]].setValue(data); // invert button value
            //   }
            // });
            // controller.update(); // update manually for better performance
            // }
          });
          testDevice2.on("error", function (error) {
            console.log(error);
            win?.webContents.send("controller_found", false);
            controllerFound = false;
            controllerListenRestart = setInterval(() => listenerLoop(), 1000);
          });
          testDevice3.on("data", function (data) {
            // console.log(data);
            const joystickHorizontal = data.readUInt16LE(2);
            const joystickVertical = data.readUInt16LE(4);
            console.log(
              "x " +
                ((parseInt(joystickHorizontal, 16) / 16600) * 2 - 1).toString()
            );
            console.log(
              "y " +
                ((parseInt(joystickVertical, 16) / 16600) * 2 - 1).toString()
            );
            // console.log("before loop");
            try {
              if (controller?.type) {
                // console.log("entered loop");
                configuration?.forEach((module, index) => {
                  if (module?.index === 5) {
                    console.log(module);
                    // console.log(module?.configuration?.input);
                    if (module?.configuration?.behavior === "default") {
                      // controller.axis.leftX.setValue(0.5); // move left stick 50% to the left
                      // controller.axis.leftY.setValue(-0.5); // move left stick 50% down
                      controller.axis.leftX.setValue(
                        (parseInt(joystickHorizontal, 16) / 16600) * 2 - 1
                      );
                      controller.axis.leftY.setValue(
                        (parseInt(joystickHorizontal, 16) / 16000) * 2 - 1
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
          });
          testDevice3.on("error", function (error) {
            console.log(error);
            win?.webContents.send("controller_found", false);
            controllerFound = false;
            controllerListenRestart = setInterval(() => listenerLoop(), 1000);
          });
          controllerFound = true;
          clearInterval(controllerListen);
          clearInterval(controllerListenRestart);
        } else {
          controllerFound = false;
          win?.webContents.send("controller_found", false);
        }
      } catch (e) {
        controllerFound = false;
        console.log("controller not found");
        win?.webContents.send("controller_found", false);
      }
    }, 1000);
    // MACS_CONTROLLER = new HID.HID(0x54c, 0xce6); // ps5 version
    // MACS_CONTROLLER.on("data", function (data) {
    //   console.log(data);
    // });
    win?.webContents.send("main-process-message", new Date().toLocaleString());
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
