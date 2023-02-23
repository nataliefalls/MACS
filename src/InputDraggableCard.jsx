import React, { useState, useEffect, useMemo } from "react";
import Box from "@mui/material/Box";
import { Card, CardActionArea, CardContent, Typography } from "@mui/material";
import { Draggable } from "react-smooth-dnd";
import { ReactComponent as ButtonLogo } from "./assets/button.svg";
import { ReactComponent as SwitchLogo } from "./assets/switch.svg";
import { ReactComponent as SliderLogo } from "./assets/slider.svg";
import { ReactComponent as DialLogo } from "./assets/knob.svg";
import { ReactComponent as DpadLogo } from "./assets/dpad.svg";
import { ReactComponent as JoystickLogo } from "./assets/joystick.svg";

const InputDraggableCard = ({ inputType }) => {
  const getModuleSVG = (inputType) => {
    switch (inputType) {
      case "button":
        return (
          <ButtonLogo
            width={65}
            height={65}
            style={{ paddingTop: 4, paddingLeft: 2.25 }}
          />
        );
      case "switch":
        return (
          <SwitchLogo width={70} height={80} style={{ paddingTop: 6.5 }} />
        );
      case "slider":
        return <SliderLogo width={70} height={70} style={{ paddingTop: 6 }} />;
      case "dial":
        return (
          <DialLogo
            width={107}
            height={107}
            style={{ marginTop: -1.25, marginLeft: -18.5 }}
          />
        );
      case "dpad":
        return (
          <DpadLogo
            width={65}
            height={65}
            style={{ paddingTop: 3.75, paddingLeft: 2.25 }}
          />
        );
      case "joystick":
        return (
          <JoystickLogo width={70} height={70} style={{ paddingTop: 5 }} />
        );
      default:
        return "";
    }
  };

  const getElevation = (inputType) => {
    switch (inputType) {
      case "switch":
        return 8;
      case "dpad":
        return 8;
      case "dial":
        return 10;
      default:
        return 4;
    }
  };
  return (
    <Draggable key={inputType.id} style={{ transform: "none" }}>
      <Box
        className={"inputDraggableCard"}
        sx={{
          // backgroundColor: "red",
          transform: "none",
          position: "static",
          minWidth: "80px",
          minHeight: "80px",
          display: "flex",
          flexDirection: "column",
          justifyContent: "center",
          alignItems: "center",
          zIndex: 100,
        }}
      >
        <Card
          elevation={4}
          sx={{
            width: "70px",
            height: "70px",
            backgroundColor: "panButtons.main",
          }}
        >
          <CardContent
            sx={{
              width: "70px",
              height: "70px",
              padding: 0,
              margin: 0,
            }}
          >
            {getModuleSVG(inputType.type)}
          </CardContent>
        </Card>
        {/* <Typography
          color={"white"}
          sx={{
            fontSize: 12,
            margin: "auto",
            textAlign: "center",
          }}
        >
          {`${
            inputType.type.charAt(0).toUpperCase() + inputType.type.slice(1)
          }`}
        </Typography> */}
      </Box>
    </Draggable>
  );
};

export default InputDraggableCard;
