import nodeLogo from "./assets/node.svg";
import {
  useState,
  Fragment,
  useRef,
  useEffect,
  useMemo,
  memo,
  useCallback,
} from "react";
import CssBaseline from "@mui/material/CssBaseline";
import Box from "@mui/material/Box";
import "./App.scss";
import {
  Button,
  Card,
  CardActionArea,
  CardContent,
  Grid,
  Paper,
  Typography,
  IconButton,
  Select,
  FormControl,
  MenuItem,
  InputLabel,
  RadioGroup,
  Radio,
  FormControlLabel,
  Dialog,
  DialogTitle,
  DialogActions,
  DialogContent,
  TextField,
  LinearProgress,
} from "@mui/material";
import { styled, useTheme } from "@mui/material/styles";
import { ReactComponent as ButtonLogo } from "./assets/button.svg";
import { ReactComponent as SwitchLogo } from "./assets/switch.svg";
import { ReactComponent as SliderLogo } from "./assets/slider.svg";
import { ReactComponent as DialLogo } from "./assets/knob.svg";
import { ReactComponent as DpadLogo } from "./assets/dpad.svg";
import { ReactComponent as JoystickLogo } from "./assets/joystick.svg";
import EditIcon from "@mui/icons-material/Edit";
import { Container, Draggable } from "react-smooth-dnd";
import { TransformWrapper, TransformComponent } from "react-zoom-pan-pinch";
import { HexGrid, Layout, Hexagon } from "react-hexgrid";
import {
  ZoomIn,
  ZoomOut,
  RestartAlt,
  Calculate,
  ChevronLeft,
  ArrowDropDown,
  RotateLeft,
  RotateRight,
} from "@mui/icons-material";
import InputDraggableCard from "./InputDraggableCard";
import { nanoid } from "nanoid";
import ModuleContext from "./Utils/moduleContext";
import { applyDrag } from "./Utils/applyDrag";
import GlobalStyles from "@mui/material/GlobalStyles";
const { ipcRenderer } = require("electron");

const Hexagons = [
  // {
  //   id: nanoid(),
  //   q: 0,
  //   r: -3,
  //   s: 3,
  //   moduleType: undefined,
  //   mainModule: true,
  // },
  {
    id: nanoid(),
    q: 0,
    r: -2,
    s: 2,
    moduleType: undefined,
    mainModule: true,
    configuration: {},
  },
  {
    id: nanoid(),
    q: 1,
    r: -2,
    s: 1,
    moduleType: undefined,
    configuration: {},
  },
  {
    id: nanoid(),
    q: 0,
    r: 0,
    s: 0,
    moduleType: "dial",
    configuration: {},
  },
  {
    id: nanoid(),
    q: 0,
    r: -1,
    s: 1,
    moduleType: "dpad",
    configuration: {},
  },
  {
    id: nanoid(),
    q: 0,
    r: 1,
    s: -1,
    moduleType: "button",
    configuration: {},
  },
  {
    id: nanoid(),
    q: 1,
    r: -1,
    s: 0,
    moduleType: "slider",
    configuration: {},
  },
  {
    id: nanoid(),
    q: 1,
    r: 0,
    s: -1,
    moduleType: "joystick",
    configuration: {
      behavior: "default",
    },
  },
  {
    id: nanoid(),
    q: -1,
    r: 1,
    s: 0,
    moduleType: "switch",
    configuration: {},
  },
  {
    id: nanoid(),
    q: -1,
    r: 0,
    s: 1,
    moduleType: "button",
    configuration: {},
  },
  {
    id: nanoid(),
    q: -2,
    r: 0,
    s: 1,
    moduleType: "button",
    configuration: {},
  },
];

// const Hexagons = [
//   {
//     id: nanoid(),
//     q: 0,
//     r: -1,
//     s: 1,
//     moduleType: undefined,
//     mainModule: true,
//   },
//   {
//     id: nanoid(),
//     q: 0,
//     r: 0,
//     s: 0,
//     moduleType: undefined,
//     configuration: {},
//   },
//   {
//     id: nanoid(),
//     q: 0,
//     r: 1,
//     s: -1,
//     moduleType: undefined,
//     configuration: {},
//   },
//   {
//     id: nanoid(),
//     q: 1,
//     r: 0,
//     s: -1,
//     moduleType: undefined,
//   },
//   {
//     id: nanoid(),
//     q: 2,
//     r: -1,
//     s: -1,
//     moduleType: undefined,
//     configuration: {},
//   },
//   {
//     id: nanoid(),
//     q: -1,
//     r: 1,
//     s: 0,
//     moduleType: undefined,
//     configuration: {},
//   },
//   {
//     id: nanoid(),
//     q: -2,
//     r: 1,
//     s: 1,
//     moduleType: undefined,
//     configuration: {},
//   },
//   {
//     id: nanoid(),
//     q: -3,
//     r: 1,
//     s: 2,
//     moduleType: undefined,
//     configuration: {},
//   },
//   {
//     id: nanoid(),
//     q: 3,
//     r: -2,
//     s: -1,
//     moduleType: undefined,
//     configuration: {},
//   },
// ];

const InputTypes = [
  {
    id: nanoid(),
    type: "button",
  },
  {
    id: nanoid(),
    type: "joystick",
  },
  {
    id: nanoid(),
    type: "switch",
  },
  {
    id: nanoid(),
    type: "slider",
  },
  {
    id: nanoid(),
    type: "dpad",
  },
  {
    id: nanoid(),
    type: "dial",
  },
];

const ControllerInputs = [
  {
    label: "A",
    value: "A",
  },
  {
    label: "B",
    value: "B",
  },
  {
    label: "X",
    value: "X",
  },
  {
    label: "Y",
    value: "Y",
  },
  {
    label: "Left Thumbstick",
    value: "LEFT_THUMB",
  },
  {
    label: "Right Thumbstick",
    value: "RIGHT_THUMB",
  },
  {
    label: "Left Shoulder",
    value: "LEFT_SHOULDER",
  },
  {
    label: "Right Shoulder",
    value: "RIGHT_SHOULDER",
  },
  {
    label: "Left Trigger",
    value: "LEFT_TRIGGER",
  },
  {
    label: "Right Trigger",
    value: "RIGHT_TRIGGER",
  },
  {
    label: "Right Joystick Up",
    value: "RIGHT_JOYSTICK_UP",
  },
  {
    label: "Right Joystick Right",
    value: "RIGHT_JOYSTICK_RIGHT",
  },
  {
    label: "Right Joystick Down",
    value: "RIGHT_JOYSTICK_DOWN",
  },
  {
    label: "Right Joystick Left",
    value: "RIGHT_JOYSTICK_LEFT",
  },
  {
    label: "Left Joystick Up",
    value: "LEFT_JOYSTICK_UP",
  },
  {
    label: "Left Joystick Right",
    value: "LEFT_JOYSTICK_RIGHT",
  },
  {
    label: "Left Joystick Down",
    value: "LEFT_JOYSTICK_DOWN",
  },
  {
    label: "Left Joystick Left",
    value: "LEFT_JOYSTICK_LEFT",
  },
  {
    label: "Start",
    value: "START",
  },
  {
    label: "Back",
    value: "BACK",
  },
  {
    label: "Guide",
    value: "GUIDE",
  },
];

function App() {
  const [inputTypes, setInputTypes] = useState(InputTypes);
  const [hexagons, setHexagons] = useState(Hexagons);
  const [controllerFound, setControllerFound] = useState(true);
  const [controllerStatus, setControllerStatus] = useState(false);
  const [activeHexagon, setActiveHexagon] = useState(null);
  const [gridRotate, setGridRotate] = useState("0deg");
  const [dropzones, setDropzones] = useState([]);
  const [distinctInputTypes, setDistinctInputTypes] = useState([]);
  const [viewBox, setViewBox] = useState("-50 -50 100 100");
  const [open, setOpen] = useState(false);
  const [controllerName, setControllerName] = useState("MACS");
  const [tempControllerName, setTempControllerName] = useState(controllerName);
  const theme = useTheme();
  const filteredHexagons = useMemo(() =>
    hexagons.filter(
      (hexagon) => {
        return !(hexagon?.mainModule ?? false);
      },
      [hexagons]
    )
  );
  const inputGlobalStyles = (
    <GlobalStyles
      styles={{
        "&::-webkit-scrollbar": {
          width: "7px",
        },

        "&::-webkit-scrollbar-track": {
          boxShadow: "inset 0 0 6px rgba(0, 0, 0, 0.3)",
          borderTopRightRadius: "10px",
          borderBottomRightRadius: "10px",
        },

        "&::-webkit-scrollbar-thumb": {
          backgroundColor: "rgba(19, 19, 19, 0.8)",
          borderRadius: "100px",
        },
        "& .controller-name-input > .MuiFormLabel-root": {
          color: "black !important",
          width: "300px",
          userSelect: "none",
          WebkitUserSelect: "none",
          MozUserSelect: "none",
        },
        "& .MuiFormLabel-root, MuiInputLabel-root": {
          color: "white !important",
          width: "300px",
          userSelect: "none",
          WebkitUserSelect: "none",
          MozUserSelect: "none",
        },
        "& .MuiInputBase-root, MuiInput-root, MuiSelect-root": {
          "&:before": {
            borderBottom: "1px solid rgb(255 255 255) !important",
          },
        },
        "& .controller-name-input > .MuiInputBase-root": {
          "&:before": {
            borderBottom: "1px solid rgb(0 0 0) !important",
          },
        },
        "& .MuiSelect-select, MuiInputBase-input, MuiInput-input": {
          color: "white !important",
          "&:before": {
            borderBottom: "1px solid rgb(255 255 255) !important",
            left: 0,
            bottom: 0,
            position: "absolute",
            right: 0,
            transition:
              "border-bottom-color 200ms cubic-bezier(1, 1, 1, 1) 0ms",
            pointerEvents: "none",
          },
          "&:after": {
            borderBottom: "2px solid #ffffff !important",
            left: 0,
            bottom: 0,
            position: "absolute",
            right: 0,
            webkitTransform: "scaleX(0)",
            transform: "scaleX(0)",
            transition: "transform 200ms cubic-bezier(0.0, 0, 0.2, 1) 0ms",
            pointerEvents: "none",
          },
        },
        "& .MuiSvgIcon-root.MuiSelect-icon": {
          webkitUserSelect: "none",
          userSelect: "none",
          width: "1em",
          height: "1em",
          display: "inline-block",
          fill: "white !important",
          webkitFlexShrink: 0,
          flexShrink: 0,
          webkitTransition: "fill 200ms cubic-bezier(0.4, 0, 0.2, 1) 0ms",
          transition: "fill 200ms cubic-bezier(0.4, 0, 0.2, 1) 0ms",
          fontSize: "1.5rem",
          position: "absolute",
          right: 0,
          top: "calc(50% - 0.5em)",
          pointerEvents: "none",
          color: "white !important",
        },
      }}
    />
  );
  const panRef = useRef(null);
  const textFieldRef = useRef(null);
  const dropzoneRefs = useRef([]);
  const hexagonRefs = useRef([]);

  const handleController = () => {
    console.log(controllerStatus);
    if (!controllerStatus) {
      ipcRenderer.send("handle_controller", { start: true });
      ipcRenderer.on("controller_error", (event, arg) => {
        console.error(event);
      });
      ipcRenderer.on("controller_started", (event, arg) => {
        setControllerStatus(true);
      });
    } else {
      ipcRenderer.send("handle_controller", { disconnect: true });
      ipcRenderer.on("controller_error", (event, arg) => {
        console.error(event);
      });
      ipcRenderer.on("controller_disconnected", (event, arg) => {
        setControllerStatus(false);
      });
    }
  };

  const handleSaveConfig = (newHexagons) => {
    // const configs = newHexagons.map((hexagon) => hexagon.configuration);
    ipcRenderer.send("save_config", newHexagons);
    ipcRenderer.on("config_saved", (event, arg) => {
      console.log(arg);
      setControllerStatus(false);
    });
  };

  const hexagonSize = { x: 10, y: 10 };

  /* function to set the position of the target dropzone relative to the hexgrid svg */
  const positionDropzone = (dropzoneIndex, moduleID) => {
    // get the target dropzone element
    // const dropzone = document.getElementById(dropzoneID);
    const dropzone = dropzoneRefs.current[dropzoneIndex];
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
  };

  const positionSVG = () => {
    if (controllerFound) {
      const svgGrid = document.getElementById("module-grid");
      const { xMin, xMax, yMin, yMax } = [...svgGrid.children].reduce(
        (acc, el) => {
          const { x, y, width, height } = el.getBBox();
          if (!acc.xMin || x < acc.xMin) acc.xMin = x;
          if (!acc.xMax || x + width > acc.xMax) acc.xMax = x + width;
          if (!acc.yMin || y < acc.yMin) acc.yMin = y;
          if (!acc.yMax || y + height > acc.yMax) acc.yMax = y + height;
          return acc;
        },
        {}
      );
      const viewBox = `${xMin} ${yMin} ${xMax - xMin} ${yMax - yMin}`;
      console.log(viewBox);
      setViewBox(viewBox);
    }
  };

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

  const getElevation = (moduleType) => {
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
  };

  const getModuleBackgroundColor = (moduleType) => {
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
  };

  const getHexagonBackgroundColor = (moduleType) => {
    switch (moduleType) {
      case "button":
        return theme.palette.hexagonRed.main;
      case "switch":
        return theme.palette.hexagonYellow.main;
      case "slider":
        return theme.palette.hexagonGreen.main;
      case "dial":
        return theme.palette.hexagonOrange.main;
      case "dpad":
        return theme.palette.hexagonPurple.main;
      case "joystick":
        return theme.palette.hexagonBlue.main;
      default:
        return theme.palette.hexagonBlack.main;
    }
  };

  const addInputHandler = (foundHexagon, type) => {
    const newHexagons = hexagons.map((hexagon) => {
      if (hexagon.id === foundHexagon.id) {
        if (type === "joystick") {
          return {
            ...hexagon,
            moduleType: type,
            configuration: {
              behavior: "default",
            },
          };
        } else {
          return {
            ...hexagon,
            moduleType: type,
          };
        }
      } else {
        return hexagon;
      }
    });
    setHexagons(newHexagons);
  };

  const removeInputHandler = () => {
    const newHexagons = hexagons.map((hexagon) => {
      if (hexagon.id === activeHexagon.id) {
        return {
          ...hexagon,
          moduleType: undefined,
          configuration: {},
        };
      } else {
        return hexagon;
      }
    });
    setHexagons(newHexagons);
    setActiveHexagon(null);
  };

  const updateInputHandler = () => {
    const newHexagons = hexagons.map((hexagon) => {
      if (hexagon.id === activeHexagon.id) {
        return {
          ...activeHexagon,
        };
      } else {
        return hexagon;
      }
    });
    handleSaveConfig(newHexagons);
    setHexagons(newHexagons);
  };

  const Dropzone = memo(
    ({ moduleID, moduleType, index }) => {
      const foundHexagon = hexagons.find((hexagon) => hexagon.id === moduleID);
      // console.log(foundHexagon);
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
              // setDragging(true);
            }}
            onDrop={(e) => {
              console.log("drop", e);
              console.log(index);
              // const result = applyDrag([foundHexagon], e);
              // console.log(result);
              if (e.addedIndex !== null || e.removedIndex !== null) {
                addInputHandler(foundHexagon, e.payload.type);
              }
            }}
            onDragEnd={(e) => {
              // setDragging(false);
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
                    setActiveHexagon({ ...foundHexagon, index });
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
              bottom: -3,
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
              display:
                foundHexagon.moduleType !== undefined ? "none" : "inherit",
              zIndex: -1,
            }}
          >
            Unassigned
          </Typography>
        </Box>
      );
    },
    (prevProps, nextProps) => {
      console.log(
        prevProps.moduleID === nextProps.moduleID &&
          prevProps.index === nextProps.index &&
          prevProps.moduleType === nextProps.moduleType
      );
      return true;
    }
  );

  // const Dropzone = useMemo(
  //   ({ moduleID, moduleType, index }) => (
  //     <MemoDropzone moduleID={moduleID} moduleType={moduleType} index={index} />
  //   ),
  //   [hexagons]
  // );

  const GeneratedHexagon = ({ hexagon, index }) => {
    return (
      <Hexagon
        id={`hexagon-${hexagon.id}`}
        q={hexagon.q}
        r={hexagon.r}
        s={hexagon.s}
        cellStyle={{ fill: getHexagonBackgroundColor(hexagon.moduleType) }}
        stroke="white"
        strokeWidth={0.25}
      />
    );
  };

  const GeneratedHexgrid = memo(
    ({ localHexagons }) => {
      return (
        <HexGrid
          id="module-grid"
          height={"80vh"}
          width={"70vw"}
          viewBox={viewBox}
          preserveAspectRatio="xMidYMid meet"
        >
          <Layout
            size={hexagonSize}
            flat={true}
            spacing={1.05}
            origin={{ x: 0, y: 0 }}
          >
            {localHexagons.map((hexagon, index) => (
              <GeneratedHexagon
                key={hexagon.id}
                hexagon={hexagon}
                index={index}
              />
            ))}
          </Layout>
        </HexGrid>
      );
    },
    (prevProps, nextProps) => {
      // console.log(prevProps.localHexagons === nextProps.localHexagons);
      return prevProps.localHexagons === nextProps.localHexagons;
    }
  );

  // const providerValues = useMemo(
  //   () => ({
  //     dropzoneRefs,
  //     panRef,
  //     activeHexagon: [activeHexagon, setActiveHexagon],
  //     hexagons: [hexagons, setHexagons],
  //   }),
  //   [
  //     dropzoneRefs,
  //     panRef,
  //     activeHexagon,
  //     setActiveHexagon,
  //     hexagons,
  //     setHexagons,
  //   ]
  // );

  const Dropzones = memo(
    () => {
      return filteredHexagons.map((hexagon, index) => {
        // <ModuleContext.Provider value={providerValues}>
        return (
          <Dropzone
            key={`dropzoneKey-${hexagon.id}`}
            moduleID={hexagon.id}
            moduleType={hexagon.moduleType}
            index={index}
          />
        );
      });
      // </ModuleContext.Provider>
    },
    () => true
  );

  const handleInputChange = (e, direction) => {
    if (
      activeHexagon.moduleType === "button" ||
      activeHexagon.moduleType === "switch"
    ) {
      setActiveHexagon({
        ...activeHexagon,
        configuration: {
          ...activeHexagon.configuration,
          input: e.target.value,
        },
      });
    } else if (
      activeHexagon.moduleType === "dpad" ||
      activeHexagon.moduleType === "joystick"
    ) {
      setActiveHexagon({
        ...activeHexagon,
        configuration: {
          ...activeHexagon.configuration,
          input: {
            ...activeHexagon.configuration.input,
            [direction]: e.target.value,
          },
        },
      });
    } else if (
      activeHexagon.moduleType === "slider" ||
      activeHexagon.moduleType === "dial"
    ) {
      setActiveHexagon({
        ...activeHexagon,
        configuration: {
          ...activeHexagon.configuration,
          input: e.target.value,
        },
      });
    }
  };

  const handleBehaviorChange = (e) => {
    setActiveHexagon({
      ...activeHexagon,
      configuration: {
        ...activeHexagon.configuration,
        behavior: e.target.value,
      },
    });
  };

  const SidePanel = ({ targetHexagon }) => {
    return (
      <>
        {activeHexagon && (
          <Button
            variant="contained"
            color={"drawerBackground"}
            style={{
              position: "absolute",
              top: "10%",
              left: 10,
              textTransform: "none",
              color: "white",
              fontSize: "1rem",
            }}
            sx={{ zIndex: 10000 }}
            onClick={() => setActiveHexagon(null)}
          >
            <ChevronLeft color="primary" />
          </Button>
        )}
        <Paper
          sx={{
            backgroundColor: "drawerBackground.main",
            position: "absolute",
            top: "19%",
            left: 10,
            display: "flex",
            alignItems: "center",
            justifyContent: "center",
            flexDirection: "column",
            flexShrink: 0,
            width: 250,
            maxHeight: "calc(100vh - 150px)",
            opacity: 0.97,
            zIndex: 10000,
            overflow: "hidden",
            pt: 1,
            pb: 1.5,
            px: activeHexagon ? 2 : 1,
          }}
        >
          {targetHexagon === null && (
            <>
              <Typography
                className="app-title"
                sx={{
                  fontWeight: 400,
                  paddingTop: 0,
                  fontSize: "1.4rem",
                  textDecoration: "underline",
                }}
              >
                Controller Setup
              </Typography>
              <Grid
                container
                spacing={2}
                sx={{
                  px: 2,
                  pt: 1,
                  pb: 2,
                }}
              >
                {inputTypes.map((inputType, index) => (
                  <Grid
                    key={inputType.id}
                    item
                    xs={6}
                    sx={{
                      display: "flex",
                      justifyContent: "center",
                      alignItems: "center",
                    }}
                  >
                    <Container
                      groupName="inputTypes"
                      behaviour="copy"
                      dragClass="inputTypeDrag"
                      dropClass="inputTypeDrop"
                      getChildPayload={(i) => {
                        return inputTypes[index];
                      }}
                      onDragStart={(e) => {
                        // setDragging(true);
                      }}
                      onDrop={(e) => {
                        setInputTypes(applyDrag(inputTypes, e));
                        console.log("dropped on something");
                        // setDragging(false);
                      }}
                      onDragEnd={(e) => {
                        // setDragging(false);
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
                      dropPlaceholderAnimation={200}
                      style={{
                        display: "flex",
                        flexDirection: "column",
                        justifyContent: "center",
                        alignItems: "center",
                        p: 5,
                      }}
                    >
                      <InputDraggableCard key={index} inputType={inputType} />
                      <Typography
                        color={"white"}
                        sx={{
                          fontSize: 16,
                          fontWeight: 500,
                          margin: "auto",
                          textAlign: "center",
                        }}
                      >
                        {`${
                          inputType.type.charAt(0).toUpperCase() +
                          inputType.type.slice(1)
                        }`}
                      </Typography>
                    </Container>
                  </Grid>
                ))}
              </Grid>
            </>
          )}
          {targetHexagon !== null && (
            <>
              <Typography
                className="app-title"
                sx={{
                  fontWeight: 400,
                  paddingTop: 0,
                  paddingBottom: 1,
                  fontSize: "1.4rem",
                  textDecoration: "underline",
                }}
              >
                {`Module ${targetHexagon?.index + 1} Settings`}
              </Typography>
              <Typography
                sx={{
                  color: "white",
                  fontWeight: 300,
                  fontSize: "1.2rem",
                  textAlign: "center",
                  width: "100%",
                  paddingBottom: 1,
                }}
              >
                {`Type - 
              ${
                targetHexagon.moduleType.slice(0, 1).toUpperCase() +
                targetHexagon.moduleType.slice(1)
              }`}
              </Typography>
              <Typography
                sx={{
                  color: "white",
                  fontWeight: 300,
                  fontSize: "1.25rem",
                  textAlign: "center",
                  width: "100%",
                  textDecoration: "underline",
                }}
              >
                Mapping
              </Typography>
              {(targetHexagon.moduleType === "button" ||
                targetHexagon.moduleType === "switch") && (
                <Box
                  sx={{
                    mt: 2,
                    display: "flex",
                    flexDirection: "column",
                    justifyContent: "center",
                    alignItems: "center",
                    mb: 1,
                  }}
                >
                  {targetHexagon.moduleType === "button" && (
                    <ButtonLogo width={80} height={80} style={{}} />
                  )}
                  {targetHexagon.moduleType === "switch" && (
                    <SwitchLogo width={100} height={100} />
                  )}
                  <FormControl
                    variant="standard"
                    color={"selectWhite"}
                    sx={{
                      width: "200px",
                      mt: targetHexagon.moduleType === "switch" ? -3 : 0,
                    }}
                  >
                    <InputLabel>Input</InputLabel>
                    <Select
                      value={targetHexagon?.configuration?.input ?? ""}
                      label="Input"
                      onChange={handleInputChange}
                      MenuProps={{
                        sx: {
                          zIndex: 100000000,
                          maxHeight: 300,
                        },
                        color: "white",
                      }}
                    >
                      {ControllerInputs.map((input, index) => (
                        <MenuItem key={input.value} value={input.value}>
                          {input.label}
                        </MenuItem>
                      ))}
                    </Select>
                  </FormControl>
                </Box>
              )}
              {(targetHexagon.moduleType === "dpad" ||
                targetHexagon.moduleType === "joystick") && (
                <Box
                  sx={{
                    display: "flex",
                    flexDirection: "column",
                    alignItems: "center",
                    justifyContent: "center",
                    pt: 1,
                    pb: 1,
                  }}
                >
                  {activeHexagon?.moduleType === "joystick" && (
                    <Box
                      sx={{
                        display: "flex",
                        flexDirection: "row",
                        alignItems: "center",
                        justifyContent: "space-evenly",
                        width: 250,
                      }}
                    >
                      <Typography
                        sx={{
                          color: "white",
                          fontWeight: 300,
                          fontSize: "1.2rem",
                        }}
                      >
                        Behavior:
                      </Typography>
                      <FormControl>
                        <RadioGroup
                          value={targetHexagon?.configuration?.behavior ?? ""}
                          onChange={(e) => handleBehaviorChange(e)}
                        >
                          <FormControlLabel
                            value="default"
                            control={<Radio color="selectWhite" size="small" />}
                            label="Default"
                            style={{
                              color: "white",
                              userSelect: "none",
                              WebkitUserSelect: "none",
                              MozUserSelect: "none",
                            }}
                          />
                          <FormControlLabel
                            value="buttons"
                            control={<Radio color="selectWhite" size="small" />}
                            label="Buttons"
                            style={{
                              color: "white",
                              userSelect: "none",
                              WebkitUserSelect: "none",
                              MozUserSelect: "none",
                            }}
                          />
                        </RadioGroup>
                      </FormControl>
                    </Box>
                  )}
                  {(activeHexagon?.configuration?.behavior === "buttons" ||
                    activeHexagon.moduleType === "dpad") && (
                    <>
                      <FormControl
                        sx={{ width: "30%" }}
                        variant="standard"
                        color={"selectWhite"}
                        // sx={{ color: "white" }}
                      >
                        <InputLabel>Up</InputLabel>
                        <Select
                          value={targetHexagon?.configuration?.input?.up ?? ""}
                          label="Input"
                          onChange={(e) => handleInputChange(e, "up")}
                          MenuProps={{
                            sx: {
                              zIndex: 100000000,
                              maxHeight: 300,
                            },
                            color: "white",
                          }}
                        >
                          {ControllerInputs.map((input, index) => (
                            <MenuItem key={input.value} value={input.value}>
                              {input.label}
                            </MenuItem>
                          ))}
                        </Select>
                      </FormControl>
                      <Box
                        sx={{
                          display: "flex",
                          flexDirection: "row",
                          alignItems:
                            targetHexagon.moduleType === "dpad"
                              ? "center"
                              : "flex-start",
                          justifyContent: "space-evenly",
                          width: 300,
                          pb: 0,
                          pt: 2,
                          mb: targetHexagon.moduleType === "dpad" ? 0 : -0.5,
                        }}
                      >
                        <FormControl
                          sx={{ width: "27%" }}
                          variant="standard"
                          color={"selectWhite"}
                          // sx={{ color: "white" }}
                        >
                          <InputLabel>Left</InputLabel>
                          <Select
                            value={
                              targetHexagon?.configuration?.input?.left ?? ""
                            }
                            label="Input"
                            onChange={(e) => handleInputChange(e, "left")}
                            MenuProps={{
                              sx: {
                                zIndex: 100000000,
                                maxHeight: 300,
                              },
                              color: "white",
                            }}
                          >
                            {ControllerInputs.map((input, index) => (
                              <MenuItem key={input.value} value={input.value}>
                                {input.label}
                              </MenuItem>
                            ))}
                          </Select>
                        </FormControl>
                        {targetHexagon.moduleType === "dpad" && (
                          <DpadLogo
                            width={65}
                            height={65}
                            style={{ paddingRight: 2 }}
                          />
                        )}
                        {targetHexagon.moduleType === "joystick" && (
                          <JoystickLogo
                            width={70}
                            height={70}
                            style={{ paddingRight: 3 }}
                          />
                        )}
                        <FormControl
                          sx={{ width: "27%" }}
                          variant="standard"
                          color={"selectWhite"}
                          // sx={{ color: "white" }}
                        >
                          <InputLabel>Right</InputLabel>
                          <Select
                            value={
                              targetHexagon?.configuration?.input?.right ?? ""
                            }
                            label="Input"
                            onChange={(e) => handleInputChange(e, "right")}
                            MenuProps={{
                              sx: {
                                zIndex: 100000000,
                                maxHeight: 300,
                              },
                              color: "white",
                            }}
                          >
                            {ControllerInputs.map((input, index) => (
                              <MenuItem key={input.value} value={input.value}>
                                {input.label}
                              </MenuItem>
                            ))}
                          </Select>
                        </FormControl>
                      </Box>
                      <FormControl
                        sx={{ width: "30%" }}
                        variant="standard"
                        color={"selectWhite"}
                        // sx={{ color: "white" }}
                      >
                        <InputLabel>Down</InputLabel>
                        <Select
                          value={
                            targetHexagon?.configuration?.input?.down ?? ""
                          }
                          label="Input"
                          onChange={(e) => handleInputChange(e, "down")}
                          MenuProps={{
                            sx: {
                              zIndex: 100000000,
                              maxHeight: 300,
                            },
                            color: "white",
                          }}
                        >
                          {ControllerInputs.map((input, index) => (
                            <MenuItem key={input.value} value={input.value}>
                              {input.label}
                            </MenuItem>
                          ))}
                        </Select>
                      </FormControl>
                    </>
                  )}
                </Box>
              )}
              {(targetHexagon.moduleType === "dial" ||
                targetHexagon.moduleType === "slider") && (
                <>
                  <Box sx={{ pt: 1, mb: -2 }}>
                    {targetHexagon.moduleType === "dial" && (
                      <DialLogo
                        width={125}
                        height={125}
                        style={{ marginBottom: -30, paddingTop: 10 }}
                      />
                    )}
                    {targetHexagon.moduleType === "slider" && (
                      <SliderLogo
                        width={100}
                        height={100}
                        style={{ paddingTop: 10, marginBottom: -2 }}
                      />
                    )}
                  </Box>
                  <FormControl
                    variant="standard"
                    color={"selectWhite"}
                    sx={{ width: "80%" }}
                  >
                    <InputLabel>Input</InputLabel>
                    <Select
                      value={targetHexagon?.configuration?.input ?? ""}
                      label="Input"
                      onChange={handleInputChange}
                      MenuProps={{
                        sx: {
                          zIndex: 100000000,
                          maxHeight: 300,
                        },
                        color: "white",
                      }}
                    >
                      {ControllerInputs.map((input, index) => (
                        <MenuItem key={input.value} value={input.value}>
                          {input.label}
                        </MenuItem>
                      ))}
                    </Select>
                  </FormControl>
                </>
              )}
              <Typography
                sx={{
                  color: "white",
                  fontWeight: 300,
                  fontSize: "1.05rem",
                  textAlign: "center",
                  width: "100%",
                  wordWrap: "break-word",
                }}
              >
                {`config: ${JSON.stringify(targetHexagon.configuration)}`}
              </Typography>
              <Box
                sx={{
                  display: "flex",
                  flexDirection: "row",
                  alignItems: "center",
                  justifyContent: "space-evenly",
                  width: "100%",
                }}
              >
                <Button
                  variant="contained"
                  color={"startButtonGreen"}
                  style={{
                    textTransform: "none",
                    color: "white",
                    fontSize: "1rem",
                  }}
                  sx={{
                    marginTop: 1,
                  }}
                  onClick={() => updateInputHandler()}
                >
                  Save
                </Button>
                <Button
                  variant="contained"
                  color={"stopButtonRed"}
                  style={{
                    textTransform: "none",
                    color: "white",
                    fontSize: "1rem",
                  }}
                  sx={{
                    marginTop: 1,
                  }}
                  onClick={() => removeInputHandler()}
                >
                  Delete
                </Button>
              </Box>
            </>
          )}
        </Paper>
      </>
    );
  };

  // let keyPressed = false;
  // window.addEventListener("keydown", function (event) {
  //   if (!keyPressed) {
  //     keyPressed = true;
  //     handleKeyPress(event);
  //     console.log("ran");
  //   }
  // });
  // window.addEventListener("keyup", function (event) {
  //   keyPressed = false;
  //   console.log("keyup");
  // });

  // Reposition the dropzones on window resize
  window.setTimeout(() => {
    console.log("resize");
    for (const [index, Hexagon] of filteredHexagons.entries()) {
      positionDropzone(index, `hexagon-${Hexagon.id}`);
    }
    window.addEventListener(
      "resize",
      () => {
        console.log("resize again");
        for (const [index, Hexagon] of filteredHexagons.entries()) {
          positionDropzone(index, `hexagon-${Hexagon.id}`);
        }
      },
      false
    );
  }, 5);

  useEffect(() => {
    dropzoneRefs.current = dropzoneRefs.current.slice(
      0,
      filteredHexagons.length
    );
    hexagonRefs.current = hexagonRefs.current.slice(0, hexagons.length);
    const tempInputTypes = new Set(
      filteredHexagons.map((hexagon) => hexagon.moduleType)
    );
    setDistinctInputTypes(Array.from(tempInputTypes));
  }, [hexagons]);

  // On initial render
  useEffect(() => {
    let tempDropzones = [];
    positionSVG();
    for (const [index, Hexagon] of filteredHexagons.entries()) {
      positionDropzone(index, `hexagon-${Hexagon.id}`);
      tempDropzones = [...tempDropzones, Hexagon.id];
    }
    const tempInputTypes = new Set(
      filteredHexagons.map((hexagon) => hexagon.moduleType)
    );
    console.log(tempInputTypes);
    setDistinctInputTypes(Array.from(tempInputTypes));
    panRef?.current?.resetTransform();
    setDropzones(tempDropzones);
    ipcRenderer.send("initialize", hexagons);
  }, []);

  // console.log("ran");

  return useMemo(() => {
    return (
      <Box
        className="App"
        sx={{
          display: "flex",
          flexDirection: "row",
          backgroundColor: "backgroundBlack.main",
        }}
      >
        {inputGlobalStyles}
        {/* <Drawer
          variant="permanent"
          open={open}
          PaperProps={{
            sx: {
              backgroundColor: "drawerBackground.main",
              opacity: 0.83,
              filter: "drop-shadow(0px 5px 5px rgba(0, 0, 0, 0.3));",
              // overflow: "hidden",
            },
          }}
        >
          <DrawerHeader sx={{ justifyContent: "flex-end" }}>
            {open ? (
              <IconButton onClick={toggleDrawer} sx={{ mr: 1.5 }}>
                <ChevronLeftIcon color="primary" />
              </IconButton>
            ) : (
              <IconButton onClick={toggleDrawer} sx={{ mr: 1.5 }}>
                <ChevronRightIcon color="primary" />
              </IconButton>
            )}
          </DrawerHeader> */}
        {/* <Divider sx={{ borderColor: "white", opacity: 0.7 }} /> */}
        {/* <Box
          sx={{
            height: "100%",
            display: "flex",
            justifyContent: "center",
            alignItems: "center",
            backgroundColor: "transparent",
          }}
        > */}
        {controllerFound && <SidePanel targetHexagon={activeHexagon} />}
        {/* </Drawer> */}
        {/* </Box> */}
        <TransformWrapper
          initialScale={1}
          ref={panRef}
          maxScale={4}
          // centerOnInit={true}
          initialPositionX={175}
          // panning={{ disabled: true }}
        >
          {({
            zoomIn,
            zoomOut,
            resetTransform,
            centerView,
            setTransform,
            ...rest
          }) => (
            <Fragment>
              {controllerFound && (
                <>
                  <Box
                    sx={{
                      display: "flex",
                      position: "absolute",
                      zIndex: 1000,
                      bottom: 10,
                      right: 10,
                      justifyContent: "space-between",
                      width: 400,
                      flexDirection: "column",
                    }}
                  >
                    <Box
                      sx={{
                        display: "flex",
                        flexDirection: "row",
                        justifyContent: "flex-end",
                        pb: 1.5,
                      }}
                    >
                      <Button
                        variant="contained"
                        disabled={gridRotate === "-30deg"}
                        color="drawerBackground"
                        onClick={() =>
                          setGridRotate((prevRotate) =>
                            gridRotate === "0deg"
                              ? "-30deg"
                              : gridRotate === "30deg"
                              ? "0deg"
                              : "-30deg"
                          )
                        }
                        sx={{ marginRight: 2.6 }}
                        // style={{ padding: 0 }}
                      >
                        <RotateLeft color="primary" />
                      </Button>

                      <Button
                        variant="contained"
                        disabled={gridRotate === "30deg"}
                        color="drawerBackground"
                        onClick={() =>
                          setGridRotate((prevRotate) =>
                            gridRotate === "-30deg"
                              ? "0deg"
                              : gridRotate === "0deg"
                              ? "30deg"
                              : "30deg"
                          )
                        }
                        // size="small"
                        // sx={{ padding: 0 }}
                      >
                        <RotateRight color="primary" />
                      </Button>
                    </Box>
                    <Box
                      sx={{
                        display: "flex",
                        flexDirection: "row",
                        justifyContent: "space-between",
                      }}
                    >
                      <Button
                        variant="contained"
                        color={
                          !controllerStatus
                            ? "startButtonGreen"
                            : "stopButtonRed"
                        }
                        style={{
                          textTransform: "none",
                          color: "white",
                          fontSize: "1rem",
                        }}
                        onClick={() => handleController()}
                      >
                        {!controllerStatus
                          ? "Start Controller"
                          : "Stop Controller"}
                      </Button>
                      <Button
                        variant="contained"
                        color="drawerBackground"
                        onClick={() => zoomIn()}
                        sx={{ padding: 0 }}
                        style={{ padding: 0 }}
                      >
                        <ZoomIn color="primary" />
                      </Button>
                      <Button
                        variant="contained"
                        color="drawerBackground"
                        onClick={() => zoomOut()}
                        size="small"
                        sx={{ padding: 0 }}
                      >
                        <ZoomOut color="primary" />
                      </Button>
                      <Button
                        variant="contained"
                        color="drawerBackground"
                        onClick={() => {
                          // centerView();
                          resetTransform();
                          // setTransform(160, 0, 1, 300, "easeInQuad");
                        }}
                        size="small"
                      >
                        <RestartAlt color="primary" />
                      </Button>
                    </Box>
                  </Box>

                  <Paper
                    elevation={6}
                    sx={{
                      display: "flex",
                      flexDirection: "row",
                      position: "absolute",
                      pl: 5,
                      pr: 3,
                      zIndex: 1000,
                      top: 40,
                      left: "50%",
                      webkitTransform: "translateX(-50%)",
                      transform: "translateX(-50%)",
                      justifyContent: "center",
                      alignItems: "center",
                      backgroundColor: "drawerBackground.main",
                      opacity: 0.97,
                      width: "max-content",
                    }}
                  >
                    <Typography
                      className="app-title"
                      sx={{
                        fontWeight: 500,
                        paddingTop: 0,
                        fontSize: "3rem",
                        mt: -0.5,
                        pr: 1,
                        width: "100%",
                      }}
                    >
                      {controllerName}
                    </Typography>
                    <IconButton onClick={() => setOpen(true)} sx={{ mt: 0 }}>
                      <EditIcon sx={{ color: "white" }} />
                    </IconButton>
                  </Paper>
                  <Paper
                    elevation={6}
                    sx={{
                      display: "flex",
                      flexDirection: "column",
                      position: "absolute",
                      pl: 4,
                      pr: 2,
                      py: 1,
                      zIndex: 1000,
                      top: 40,
                      right: 10,
                      justifyContent: "center",
                      alignItems: "center",
                      backgroundColor: "drawerBackground.main",
                      opacity: 0.97,
                    }}
                  >
                    <Typography
                      className="app-title"
                      sx={{
                        fontWeight: 400,
                        paddingTop: 0,
                        fontSize: "1.4rem",
                        textDecoration: "underline",
                      }}
                    >
                      Module Types
                    </Typography>
                    {distinctInputTypes.map((type, index) => (
                      <Box
                        key={index}
                        sx={{
                          display: "flex",
                          flexDirection: "row",
                          alignItems: "center",
                          justifyContent: "center",
                          my: 0,
                          width: "100%",
                        }}
                      >
                        <Box
                          sx={{
                            position: "absolute",
                            width: "20px",
                            height: "20px",
                            left: 17.5,
                            border: 1,
                            borderColor: "white",
                            backgroundColor: "backgroundBlack.main",
                            zIndex: 10,
                          }}
                        >
                          <Box
                            sx={{
                              position: "absolute",
                              width: "20px",
                              height: "20px",
                              top: 0,
                              left: 0,
                              backgroundColor: getHexagonBackgroundColor(type),
                            }}
                          />
                        </Box>
                        <Typography
                          sx={{
                            color: "white",
                            fontWeight: 300,
                            fontSize: "1.05rem",
                            textAlign: "center",
                            width: "100%",
                          }}
                        >
                          {type === undefined
                            ? "Unassigned"
                            : type.slice(0, 1).toUpperCase() + type.slice(1)}
                        </Typography>
                      </Box>
                    ))}
                  </Paper>
                </>
              )}
              <TransformComponent
                wrapperStyle={{
                  display: "flex",
                  flex: 1,
                  // marginLeft: 80,
                  height: "calc(100vh - 35px)",
                  width: "100vw",
                  zIndex: 1,
                  // alignItems: "center",
                  // justifyContent: "center",
                  // backgroundColor: "green",
                }}
                contentStyle={{
                  display: "flex",
                  height: "calc(100vh - 35px)",
                  width: "100vw",
                  alignItems: "center",
                  justifyContent: "center",
                  paddingTop: controllerFound ? 50 : 0,
                  paddingHorizontal: 0,
                  paddingBottom: 0,
                  zIndex: 1,
                  // backgroundColor: "white",
                }}
              >
                {!controllerFound && (
                  <Box
                    sx={{
                      display: "flex",
                      flexDirection: "column",
                      alignItems: "center",
                      justifyContent: "center",
                      padding: 10,
                    }}
                  >
                    <Typography
                      className="app-title"
                      sx={{
                        fontFamily: "bitcount-mono-single-line-ci",
                        marginTop: -10,
                        fontSize: "8rem",
                      }}
                    >
                      {controllerName}
                    </Typography>
                    <Typography
                      sx={{ color: "white", mt: -2, pb: 2, fontSize: "1.5rem" }}
                    >
                      No Controller Detected
                    </Typography>
                    <Box sx={{ width: "65%" }}>
                      <LinearProgress color="topBarBlue" />
                    </Box>
                    {/* <Button
                      variant="contained"
                      color={
                        !controllerStatus ? "startButtonGreen" : "stopButtonRed"
                      }
                      style={{
                        textTransform: "none",
                        color: "white",
                      }}
                      onClick={() => handleController()}
                    >
                      {!controllerStatus
                        ? "Start Controller"
                        : "Stop Controller"}
                    </Button> */}
                  </Box>
                )}
                {controllerFound && (
                  <Box
                    sx={{
                      rotate: gridRotate,
                      position: "relative",
                    }}
                  >
                    <GeneratedHexgrid localHexagons={hexagons} />
                    <Dropzones />
                  </Box>
                )}
              </TransformComponent>
            </Fragment>
          )}
        </TransformWrapper>
        <Dialog
          open={open}
          onClose={() => setOpen(false)}
          PaperProps={{ sx: { minWidth: 400 } }}
          sx={{ zIndex: 10000000 }}
        >
          <DialogTitle id="alert-dialog-title">{"Controller Name"}</DialogTitle>
          <DialogContent>
            <TextField
              id="controller-name-input"
              label="Name"
              helperText="Please enter a name for this controller"
              className="controller-name-input"
              variant="standard"
              defaultValue={controllerName}
              inputProps={{ maxLength: 14 }}
              ref={textFieldRef}
              fullWidth
              onChange={(e) => setTempControllerName(e.target.value)}
            />
          </DialogContent>
          <DialogActions>
            <Button
              variant="contained"
              color="selectWhite"
              style={{
                textTransform: "none",
                fontSize: "1rem",
              }}
              onClick={() => setOpen(false)}
            >
              Cancel
            </Button>
            <Button
              variant="contained"
              color="panButtons"
              disabled={tempControllerName === ""}
              style={{
                textTransform: "none",
                fontSize: "1rem",
              }}
              onClick={() => {
                setControllerName(tempControllerName);
                setOpen(false);
              }}
              autoFocus
            >
              Save
            </Button>
          </DialogActions>
        </Dialog>
      </Box>
    );
  }, [
    controllerFound,
    controllerStatus,
    hexagons,
    activeHexagon,
    distinctInputTypes,
    viewBox,
    gridRotate,
    tempControllerName,
    controllerName,
    open,
  ]);
}

export default App;
