import nodeLogo from "./assets/node.svg";
import { useState } from "react";
import "./App.scss";

const { ipcRenderer } = require("electron");

// console.log(
//   "[App.tsx]",
//   `Hello world from Electron ${process.versions.electron}!`
// );

const { app } = window;

console.log(app);

function App() {
  const [controllerStatus, setControllerStatus] = useState(false);
  const handleController = () => {
    console.log(controllerStatus);
    if (!controllerStatus) {
      ipcRenderer.send("handle_controller", { start: true });
      ipcRenderer.on("controller_error", (event, arg) => {
        console.error(arg);
      });
      ipcRenderer.on("controller_started", (event, arg) => {
        setControllerStatus(true);
      });
    } else {
      ipcRenderer.send("handle_controller", { disconnect: true });
      ipcRenderer.on("controller_error", (event, arg) => {
        console.error(arg);
      });
      ipcRenderer.on("controller_disconnected", (event, arg) => {
        setControllerStatus(false);
      });
    }
    // if (!controllerStatus) {
    //   app.controller.startClient("client/start", { start: true });
    //   app.controller.errorController("controller/error", (event, arg) => {
    //     console.error(arg);
    //   });
    //   app.controller.clientStarted("client/started", (event, arg) => {
    //     console.log(arg);
    //     setControllerStatus(true);
    //   });
    // } else {
    //   app.controller.stopController("controller/stop", { disconnect: true });
    //   app.controller.errorController("controller/error", (event, arg) => {
    //     console.error(arg);
    //   });
    //   app.controller.disconnectedController(
    //     "controller/disconnected",
    //     (event, arg) => {
    //       setControllerStatus(false);
    //     }
    //   );
    // }
  };
  return (
    <div className="App">
      <h1 className="app-title">MACS</h1>
      <div className="card">
        <button onClick={() => handleController()}>
          {!controllerStatus ? "Start Controller" : "Stop Controller"}
        </button>
      </div>
    </div>
  );
}

export default App;
