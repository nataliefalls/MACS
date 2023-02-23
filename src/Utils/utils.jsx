import React from "react";
import { ReactComponent as ButtonLogo } from "../assets/button.svg";
import { ReactComponent as SwitchLogo } from "../assets/switch.svg";
import { ReactComponent as SliderLogo } from "../assets/slider.svg";
import { ReactComponent as DialLogo } from "../assets/knob.svg";
import { ReactComponent as DpadLogo } from "../assets/dpad.svg";
import { ReactComponent as JoystickLogo } from "../assets/joystick.svg";

/* function to set the position of the target dropzone relative to the hexgrid svg */
export function positionDropzone(dropzoneIndex, moduleID, dropzoneRefs) {
  // get the target dropzone element
  // const dropzone = document.getElementById(dropzoneID);
  const dropzone = dropzoneRefs?.current[dropzoneIndex];
  // console.log(dropzone);
  // get the target module element
  const localModule = document.getElementById(moduleID);
  // console.log(hexagonRefs.current);
  // const localModule = hexagonRefs.current[moduleIndex];
  // console.log(localModule);
  // bounding box for the target module element
  if (!localModule) return;
  const dropzoneBB = localModule?.getCTM();
  // set the position of the dropzone absolute to the hexgrid svg
  // console.log(dropzoneBB);
  if (dropzone) {
    dropzone.style.left = dropzoneBB.e - 40 + "px";
    dropzone.style.top = dropzoneBB.f - 52 + "px";
  }
}

export function getModuleSVG(moduleType) {
  switch (moduleType) {
    case "button":
      return (
        <ButtonLogo
          width={65}
          height={65}
          style={{ paddingTop: 4, paddingLeft: 2.25 }}
        />
      );
    case "switch":
      return <SwitchLogo width={70} height={80} style={{ paddingTop: 6.5 }} />;
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
      return <JoystickLogo width={70} height={70} style={{ paddingTop: 5 }} />;
    default:
      return "";
  }
}

export function getElevation(moduleType) {
  switch (moduleType) {
    case "switch":
      return 8;
    case "dpad":
      return 8;
    case "dial":
      return 10;
    default:
      return 4;
  }
}

export function getModuleBackgroundColor(moduleType) {
  switch (moduleType) {
    case "button":
      return "hexagonRedAlt.main";
    case "switch":
      return "hexagonYellowAlt.main";
    case "slider":
      return "hexagonGreenAlt.main";
    case "dial":
      return "hexagonOrangeAlt.main";
    case "dpad":
      return "hexagonPurpleAlt.main";
    case "joystick":
      return "hexagonBlueAlt.main";
    default:
      return "hexagonBlackAlt.main";
  }
}

export function getHexagonBackgroundColor(moduleType) {
  switch (moduleType) {
    case "button":
      return "rgba(192, 0, 69, 0.5)";
    case "switch":
      return "rgba(255, 184, 0, 0.5)";
    case "slider":
      return "rgba(25, 177, 0, 0.5)";
    case "dial":
      return "rgba(255, 61, 0, 0.5)";
    case "dpad":
      return "rgba(143, 0, 255, 0.5)";
    case "joystick":
      return "rgba(0, 135, 177, 0.5)";
    default:
      return "rgba(0, 0, 0, 0.5)";
  }
}
