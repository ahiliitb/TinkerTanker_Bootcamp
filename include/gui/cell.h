#pragma once

namespace GUI {
  // Defining the cell type
  enum class CellType
    {
      Unknown,
      Empty,
      Obstacle,
      Resource
    };


  // Defining cells of the map
  struct Cell
  {
    CellType type;
    bool scanned;
    Cell() : type(CellType::Unknown), scanned(false) {}
  };
}
