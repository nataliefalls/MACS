// const { ipcRenderer, contextBridge } = require("electron");

// const API = {
//   window: {
//     close: () => ipcRenderer.send("app/close"),
//     minimize: () => ipcRenderer.send("app/minimize"),
//   },
//   controller: {
//     startClient: (_, data) => ipcRenderer.send("client/start", data),
//     clientStarted: (_, data) => ipcRenderer.on("client/started", data),
//     startController: (_, data) => ipcRenderer.send("controller/start", data),
//     stopController: () => ipcRenderer.send("controller/stop"),
//     errorController: (_, data) => ipcRenderer.invoke("controller/error", data),
//     disconnectedController: () => ipcRenderer.invoke("controller/disconnected"),
//   },
// };

// contextBridge.exposeInMainWorld("app", API);

function domReady(
  condition: DocumentReadyState[] = ["complete", "interactive"]
) {
  return new Promise((resolve) => {
    if (condition.includes(document.readyState)) {
      resolve(true);
    } else {
      document.addEventListener("readystatechange", () => {
        if (condition.includes(document.readyState)) {
          resolve(true);
        }
      });
    }
  });
}

const safeDOM = {
  append(parent: HTMLElement, child: HTMLElement) {
    if (!Array.from(parent.children).find((e) => e === child)) {
      return parent.appendChild(child);
    }
  },
  remove(parent: HTMLElement, child: HTMLElement) {
    if (Array.from(parent.children).find((e) => e === child)) {
      return parent.removeChild(child);
    }
  },
};

/**
 * https://tobiasahlin.com/spinkit
 * https://connoratherton.com/loaders
 * https://projects.lukehaas.me/css-loaders
 * https://matejkustec.github.io/SpinThatShit
 */
function useLoading() {
  const className = `loaders-css__square-spin`;
  const styleContent = `
@keyframes square-spin {
  25% { transform: rotateX(180deg) rotateY(0); }
  50% { transform: rotateX(180deg) rotateY(180deg); }
  75% { transform: rotateX(0) rotateY(180deg); }
  100% { transform: rotateX(0) rotateY(0); }
}
.${className} > div {
  animation-fill-mode: both;
  animation: square-spin 3s 0s cubic-bezier(0.09, 0.57, 0.49, 0.9) infinite;
}
.app-loading-wrap {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
  background: #263d52;
  z-index: 1000000000;
}
.hexagon-spinner-wrapper {
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 0;
  padding: 0;
  color: #fff;
}
.hexagon-spinner {
  display: flex;
  align-items: center;
  justify-content: center;
  font-family: "Arial", sans-serif;
  // background-color: red;
  color: #fff;
  font-size: 135px;
  line-height: 1;
  rotate: 30deg;
  -webkit-app-region: no-drag;
  -webkit-user-select: none;
  -ms-user-select: none;
  user-select: none;
}

    `;
  const oStyle = document.createElement("style");
  const oDiv = document.createElement("div");

  oStyle.id = "app-loading-style";
  oStyle.innerHTML = styleContent;
  oDiv.className = "app-loading-wrap";
  oDiv.innerHTML = `<div class="${className}"><div class="hexagon-spinner-wrapper"><p class="hexagon-spinner">&#x2B22;</p></div></div></div>`;

  return {
    appendLoading() {
      safeDOM.append(document.head, oStyle);
      safeDOM.append(document.body, oDiv);
    },
    removeLoading() {
      safeDOM.remove(document.head, oStyle);
      safeDOM.remove(document.body, oDiv);
    },
  };
}

// ----------------------------------------------------------------------

const { appendLoading, removeLoading } = useLoading();
domReady().then(appendLoading);

window.onmessage = (ev) => {
  ev.data.payload === "removeLoading" && removeLoading();
};

// setTimeout(() => {
//   console.log("removeLoading");
//   removeLoading();
// }, 10000);
