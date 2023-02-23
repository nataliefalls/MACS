import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import "@fontsource/roboto/300.css";
import "@fontsource/roboto/400.css";
import "@fontsource/roboto/500.css";
import "@fontsource/roboto/700.css";
import "@fontsource/k2d/100.css";
import "@fontsource/k2d/200.css";
import "@fontsource/k2d/300.css";
import "@fontsource/k2d/400.css";
import "@fontsource/k2d/500.css";
import "@fontsource/k2d/600.css";
import "@fontsource/k2d/700.css";
import "@fontsource/k2d/800.css";
import "@fontsource/k2d/100-italic.css";
import "@fontsource/k2d/200-italic.css";
import "@fontsource/k2d/300-italic.css";
import "@fontsource/k2d/400-italic.css";
import "@fontsource/k2d/500-italic.css";
import "@fontsource/k2d/600-italic.css";
import "@fontsource/k2d/700-italic.css";
import "@fontsource/k2d/800-italic.css";
// import "./samples/node-api";
import "./index.scss";
import { ThemeProvider } from "@mui/material";
import theme from "./theme";

ReactDOM.createRoot(document.getElementById("root") as HTMLElement).render(
  <React.StrictMode>
    <ThemeProvider theme={theme}>
      <App />
    </ThemeProvider>
  </React.StrictMode>
);

setTimeout(() => postMessage({ payload: "removeLoading" }, "*"), 5000);
