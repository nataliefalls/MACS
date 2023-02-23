import React, { useContext, memo } from "react";
import {
  Box,
  Card,
  CardActionArea,
  CardContent,
  Typography,
} from "@mui/material";
import { Container } from "react-smooth-dnd";
import { getModuleBackgroundColor, getElevation } from "../Utils/utils";

import ModuleContext from "../Utils/moduleContext";

const Dropzone = ({ moduleID, moduleType, index }) => {
  const { dropzoneRefs, panRef, activeHexagon, hexagons } =
    useContext(ModuleContext);
  const [stateActiveHexagon, setStateActiveHexagon] = activeHexagon;
  const [stateHexagons, setStateHexagons] = hexagons;
  const foundHexagon = stateHexagons.find((hexagon) => hexagon.id === moduleID);

  const ModuleSVG = memo(
    ({ moduleType }) => {
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
          return (
            <SwitchLogo width={70} height={80} style={{ paddingTop: 6.5 }} />
          );
        case "slider":
          return (
            <SliderLogo width={70} height={70} style={{ paddingTop: 6 }} />
          );
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
    },
    (prevProps, nextProps) => {
      return prevProps.moduleType === nextProps.moduleType;
    }
  );
  return (
    <Box
      id={`dropzone-${moduleID}`}
      ref={(el) => (dropzoneRefs.current[index] = el)}
      sx={{
        position: "absolute",
        // backgroundColor: "red",
        width: "80px",
        height: "104px",
        display: "flex",
        flexDirection: "column",
        justifyContent: "center",
        alignItems: "center",
      }}
    >
      <Container
        groupName="inputTypes"
        behaviour="move"
        dragClass="inputTypeDrag"
        dropClass="inputTypeDrop"
        getChildPayload={(i) => {
          console.log(i);
          hexagons[index];
        }}
        onDragStart={(e) => {
          setDragging(true);
        }}
        onDrop={(e) => {
          console.log("drop", e);
          console.log(index);
          addInputHandler(foundHexagon, e.payload.type);
        }}
        onDragEnd={(e) => {
          setDragging(false);
        }}
        onDragEnter={(e) => {
          console.log("drag enter");
        }}
        onDragLeave={(e) => {
          console.log("drag leave");
        }}
        dropPlaceholder={{
          animationDuration: 150,
          showOnTop: true,
          className: "drop-preview",
        }}
        shouldAcceptDrop={(sourceContainerOptions, payload) => {
          if (foundHexagon.moduleType === undefined) return true;
          else return false;
        }}
        dropPlaceholderAnimation={200}
        style={{
          position: "absolute",
          display: "flex",
          width: "70px",
          height: "70px",
          borderRadius: "5px",
          boxShadow:
            foundHexagon.moduleType === undefined
              ? "inset 0 0 7px #70828f"
              : "",
          backgroundColor:
            foundHexagon.moduleType === undefined
              ? "rgba(50, 50, 50, 0.5)"
              : "transparent",
          transition: "all 0.2s ease-in",
        }}
      >
        {foundHexagon.moduleType !== undefined ? (
          <Card
            elevation={getElevation(moduleType)}
            sx={{
              width: "70px",
              height: "70px",
              backgroundColor: getModuleBackgroundColor(moduleType),
              // zIndex: 1000,
            }}
          >
            <CardActionArea
              onClick={(e) => {
                panRef.current.zoomToElement(
                  `dropzone-${moduleID}`,
                  3,
                  300,
                  "easeOut"
                );
                setStateActiveHexagon({ ...foundHexagon, index });
                // console.log(foundHexagon, index);
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
                <ModuleSVG moduleType={moduleType} />
              </CardContent>
            </CardActionArea>
          </Card>
        ) : (
          ""
        )}
      </Container>
      <Typography
        color={"white"}
        sx={{
          fontSize: 12,
          position: "absolute",
          bottom: 0,
          left: 0,
          right: 0,
          margin: "auto",
          textAlign: "center",
        }}
      >
        {`Module ${index + 1}`}
      </Typography>
      <Typography
        sx={{
          position: "absolute",
          color: "white",
          fontSize: 12,
          display: foundHexagon.moduleType !== undefined ? "none" : "inherit",
          zIndex: -1,
        }}
      >
        Unassigned
      </Typography>
    </Box>
  );
};

export default memo(Dropzone);
