const { ipcRenderer } = require("electron");

const MINUS = document.getElementById("minimize");
const CLOSE_APP = document.getElementById("close-app");

MINUS.addEventListener("click", minimize);
CLOSE_APP.addEventListener("click", close_app);

function close_app() {
  ipcRenderer.send("app/close");
}

function minimize() {
  ipcRenderer.send("app/minimize");
}

console.log("window.js loaded");
console.log(window.app);
