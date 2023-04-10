#pragma once

#include <map>
#include <vector>
#include "coordinates.h"

typedef struct position_t {
    module_coordinates_t coordinates;
    uint8_t orientation;
} position_t;

// typedef struct module_side_t {
//     uint8_t parentAddress;
//     uint8_t childRelSide;
// } module_side_t;

typedef std::map<uint8_t, uint8_t> parent_to_side_t;

class PositionManager {
    private:
    std::map<uint8_t, position_t> knownPositions;
    // uint8_t childRelSide = parentDependencies[childAddr][parentAddr];
    std::map<uint8_t, parent_to_side_t> parentDependencies;

    public:
    

    private:
    void resolveDependency(uint8_t parentAddr, uint8_t parentRelSide, uint8_t childAddress);
    position_t newPosition(uint8_t parentAddr, uint8_t parentRelSide, uint8_t childRelSide);
    uint8_t newOrientation(position_t parentPosition, uint8_t parentRelSide, uint8_t childRelSide);
    module_coordinates_t newCoordinates(position_t parentPosition, uint8_t parentRelSide);
};

class PositionDependencyTree {
    private:
    
};