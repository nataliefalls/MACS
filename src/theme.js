import { createTheme } from "@mui/material";

const theme = createTheme({
  palette: {
    primary: {
      main: "#ffffffb3",
    },
    panButtons: {
      main: "#70828f",
    },
    backgroundBlack: {
      main: "#323232",
    },
    drawerBackground: {
      main: "#485863",
    },
    moduleBackground: {
      main: "rgba(112, 130, 143, 0.83)",
    },
    hexagonBlack: {
      main: "rgba(0, 0, 0, 0.5)",
    },
    hexagonBlackAlt: {
      main: "rgba(50, 50, 50, 0.5)",
    },
    hexagonRed: {
      main: "rgba(192, 0, 69, 0.5)",
    },
    hexagonRedAlt: {
      main: "rgba(192, 0, 69, 0.2)",
    },
    hexagonBlue: {
      main: "rgba(0, 135, 177, 0.5)",
    },
    hexagonBlueAlt: {
      main: "rgba(0, 135, 177, 0.2)",
    },
    hexagonGreen: {
      main: "rgba(25, 177, 0, 0.5)",
    },
    hexagonGreenAlt: {
      main: "rgba(25, 177, 0, 0.2)",
    },
    hexagonPurple: {
      main: "rgba(143, 0, 255, 0.5)",
    },
    hexagonPurpleAlt: {
      main: "rgba(143, 0, 255, 0.2)",
    },
    hexagonOrange: {
      main: "rgba(255, 61, 0, 0.5)",
    },
    hexagonOrangeAlt: {
      main: "rgba(255, 61, 0, 0.2)",
    },
    hexagonYellow: {
      main: "rgba(255, 184, 0, 0.5)",
    },
    hexagonYellowAlt: {
      main: "rgba(255, 184, 0, 0.2)",
    },
    startButtonGreen: {
      main: "#8ac926",
    },
    stopButtonRed: {
      main: "#c00045",
    },
    selectWhite: {
      main: "#ffffff",
    },
    topBarBlue: {
      main: "#263d52",
    },
  },
  typography: {
    fontFamily: ["K2D", "Roboto", "Helvetica", "Arial", "sans-serif"].join(","),
  },
});

export default theme;
