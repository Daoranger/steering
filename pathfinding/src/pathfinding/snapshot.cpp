//
// Created by hoang on 3/30/2026.
//

#include "snapshot.h"

Snapshot::Snapshot() {}

void Snapshot::prepareSnapshot()
{

    for (auto& cell : frontier_)
    {
        if ((cell->getType() == CellType::start) || (cell->getType() == CellType::goal))
        {
            continue;
        }
        cell->setType(CellType::frontier);
    }

    for (auto& cell : explored_)
    {
        if ((cell->getType() == CellType::start) || (cell->getType() == CellType::goal))
        {
            continue;
        }
        cell->setType(CellType::explored);
    }
}