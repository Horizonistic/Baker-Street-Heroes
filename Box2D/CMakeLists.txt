cmake_minimum_required (VERSION 2.6)
project(Box2D)


file(GLOB sourceFiles
        "Box2D.h"
        "Collision/*"
        "Collision/Shapes/*"
        "Common/*"
        "Dynamics/*"
        "Dynamics/Contacts/*"
        "Dynamics/Joints/*"
        "Rope/*"
        #"*/*.h"
        #"*/*.cpp"
        #"*/*/*.h"
        #"*/*/*.cpp"
        )

add_library(Box2D SHARED ${sourceFiles})