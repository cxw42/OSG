FILE(GLOB TARGET_SRC Billboard.cpp Camera.cpp Drawable.cpp Node.cpp)
FILE(GLOB TARGET_H *.h)

SET(TARGET_ADDED_LIBRARIES osg)

#### end var setup  ###
SETUP_PLUGIN(scripting_osg)
