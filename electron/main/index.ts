import { app, BrowserWindow, shell, ipcMain } from "electron";
import { release } from "node:os";
import { join } from "node:path";
const ViGEmClient = require("vigemclient");
const HID = require("node-hid");

let client = new ViGEmClient();
let controller;
let controllerListen;
let MACS_CONTROLLER;
let buttons;
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

  // Test actively push message to the Electron-Renderer
  win.webContents.on("did-finish-load", () => {
    const devices = HID.devices();
    MACS_CONTROLLER = new HID.devices(0xcafe, 0x0000);
    // device = new HID.HID(0x054c, 0x0ce6); // ps5 controller
    // device = new HID.HID(0x045e, 0x02ff); // xbox controller
    // console.log("test");
    console.log(devices);
    console.log(MACS_CONTROLLER);
    let count = 0;
    for (const device of MACS_CONTROLLER) {
      const testDevice = new HID.HID(device.path);
      if (testDevice) {
        console.log("device found");
        // try {
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
          console.log(data);
          // count++;
          if (controller && controller?.productID && controller?.vendorID) {
            const buttonState = data.readUInt8(5);
            console.log(buttonState);
            controller.button.A.setValue(buttonState);
            controller.update();
          }
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
        });
      }
    }

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

ipcMain.on("handle_controller", (event, arg) => {
  console.log(configuration);
  const { start, disconnect } = arg;
  if (start) {
    try {
      client.connect(); // establish connection to the ViGEmBus driver
    } catch (e) {
      event.sender.send("controller_error", e);
    }
    console.log("client started");

    try {
      controller = client.createX360Controller();
      controller.connect(); // plug in the virtual controller
    } catch (e) {
      event.sender.send("controller_error", e);
    }
    console.log("controller connected");
    console.log(controller.vendorID);
    console.log(controller.productID);
    event.sender.send("controller_started", true);
    let t = 0;

    buttons = Object.keys(controller.button);
    // controllerListen = setInterval(() => {
    //   // read the controller data
    //   console.log(controller.button.A.value);
    //   controller.button.A.setValue(!controller.button.A.value);
    //   controller.update();
    // }, 500);
  }
  if (disconnect) {
    try {
      controller.disconnect();
      clearInterval(controllerListen);
    } catch (e) {
      event.sender.send("controller_error", e);
    }
    event.sender.send("controller_disconnected", true);
    console.log("controller disconnected");
  }
});

ipcMain.on("app/close", () => {
  if (device.length > 0) device.close();
  app.quit();
});

ipcMain.on("app/minimize", () => {
  win.minimize();
});
