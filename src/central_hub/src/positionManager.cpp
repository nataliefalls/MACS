#include "positionManager.h"

module_coordinates_t PositionManager::newCoordinates(position_t parentPosition, uint8_t parentRelSide) {
    uint8_t parentAbsSide = (parentRelSide - parentPosition.orientation) % 6;
    uint8_t parentU = parentPosition.coordinates.u;
    uint8_t parentV = parentPosition.coordinates.v;

    switch (parentAbsSide) {
        case 0: return {parentU - 1, parentV + 1};
        case 1: return {parentU    , parentV + 1};
        case 2: return {parentU + 1, parentV    };
        case 3: return {parentU + 1, parentV - 1};
        case 4: return {parentU    , parentV - 1};
        case 5: return {parentU - 1, parentV    };
    }
}

uint8_t PositionManager::newOrientation(position_t parentPosition, uint8_t parentRelSide, uint8_t childRelSide) {
    return (parentPosition.orientation + parentRelSide - childRelSide + 3) % 6;
}

position_t PositionManager::newPosition(uint8_t parentAddr, uint8_t parentRelSide, uint8_t childRelSide) {
    position_t parentPosition = this->knownPositions[parentAddr];
    module_coordinates_t coordinates = this->newCoordinates(parentPosition, parentRelSide);
    uint8_t orientation = this->newOrientation(parentPosition, parentRelSide, childRelSide);
    return {coordinates, orientation};
}

void PositionManager::resolveDependency(uint8_t parentAddr, uint8_t parentRelSide, uint8_t childAddress) {
    uint8_t childRelSide = this->parentDependencies[childAddress][parentAddr];
    position_t childPosition = this->newPosition(parentAddr, parentRelSide, childRelSide);
    this->knownPositions[childAddress] = childPosition;
}