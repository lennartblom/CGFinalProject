//
//  dsa.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 20.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "dsa.hpp"


++++++++++ Programm-keys: ++++++++++++

— POSITION, ROTATION and SCALING —
# + # Scale current FurnitureObject up
# - # Scale current FurnitureObject down
# w # Increase the Y-Position of the currentFurnitureObject
# s # Decrease the Y-Position of the currentFurnitureObject
# r # Rotate the current FurnitureObject Pi/2 clockwise

Additionally one can pick an axis of the currentFurnitureObject and change its properties with mouse movements


— OBJECTS, ROOMGEOMETRY —
# c # Clear the current FurnitureObject
# g # Activate RoomGeometry-Mode (for changing the roomDimensions with UP/DOWN/RIGHT/LEFT keys while pushing G)
# p # Change the wallpaper of the wall the mouse is pointed on

— SAVING —
# m # Save current scene arrangements

— HIERARCHY —
# h # While pushing this key, one is in hierachySettingMode and can add child Nodes to the currentFurnitureObject
# e # Erase all hierachy-Information

++++++++++ +++++++++++++++++++++ ++++++++++++



++++++++++ External source-code: ++++++++++++

Because of time-consuming difficulties in setting up the vertex- and index-buffers with working normals and textcoords the following
external source-code was used to concentrated on the software features instead of loosing time in finding solutions in bugs and memory conflicts in our practical Model class.

The class Model (Model.h and Model.cpp) is
external sourcecode of Torben Höhn:

Including all .obj, .bmp and .mtl files.

(excluding the additional function-idea of)
——-> void Model::vDrawWall(unsigned int &material) const;

++++++++++ ++++++++++++++++++++++++ ++++++++++