import { app, BrowserWindow, shell, ipcMain } from "electron";
import { release } from "node:os";
import { join } from "node:path";
const ViGEmClient = require("vigemclient");

let client = new ViGEmClient();
let controller;
let controllerListen;

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

  // Test actively push message to the Electron-Renderer
  win.webContents.on("did-finish-load", () => {
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

// ipcMain.on("client/start", (event, arg) => {
//   console.log(event);
//   const { start } = arg;
//   console.log(arg);
//   if (start) {
//     try {
//       client.connect(); // establish connection to the ViGEmBus driver
//     } catch (e) {
//       ipcMain.handle("controller/error", e)
//     }
//     console.log("client started");

//     // try {
//     let controller = client.createX360Controller();
//     controller.connect(); // plug in the virtual controller
//     // } catch (e) {
//     //   event.sender.send("controller/error", e);
//     // }
//     console.log("controller connected");
//     console.log(controller.vendorID);
//     console.log(controller.productID);
//     ipcMain.handle("client/started", true);
//   }
// });

ipcMain.on("handle_controller", (event, arg) => {
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

    let buttons = Object.keys(controller.button);
    let btn = 0;
    // controllerListen = setInterval(() => {
    //   controller.axis.leftX.setValue(Math.sin(t));
    //   controller.axis.leftY.setValue(Math.cos(t));
    //   controller.axis.rightX.setValue(-Math.sin(t));
    //   controller.axis.rightY.setValue(Math.cos(t));
    //   controller.axis.dpadHorz.setValue(Math.sin(t));
    //   controller.axis.dpadVert.setValue(Math.cos(t));
    //   if (buttons[btn] != "GUIDE") {
    //     // otherwise Win 10 spams the GameBar
    //     controller.button[buttons[btn]].setValue(
    //       !controller.button[buttons[btn]].value
    //     ); // invert button value
    //   }
    //   controller.update(); // update manually for better performance
    //   t += 0.1;
    //   btn = (btn + 1) % buttons.length;
    // }, 500);

    controllerListen = setInterval(() => {
      console.log(controller.button.A.value);
      controller.button.A.setValue(!controller.button.A.value);
      controller.update();
    }, 500);
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
  app.quit();
});

ipcMain.on("app/minimize", () => {
  win.minimize();
});
