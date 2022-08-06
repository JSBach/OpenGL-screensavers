# Project description

The spirograph screensaver draws epicycloids and hypocycloids.

# Features

- Settable parameters
  - stationary circla radius and position
  - moving circle radius 
  - drawing point radius and initial angular position
  - non-unity screen ratio distortion compensation
  - epicycloid/hypocycloid switch
  - point colors - either static or gradient

# TODO list

## Finish the setup

- Shader source code should not be hardcoded. Use files instead
- Wrap shaders in a dedicated class

## Finish the functionality

- Implement parameters mode (static and dynamic)
- Implement dynamic parameters behavior
- Implement fullscreen mode
- Implement settings dialog
- Implement behavior dependency on command line
- Implement sprite drawing mode (in addition to existing point mode)
- Improve parameters setting
  - Create constants for default values and use them in the one argument constructor
  - Create a constructor for setting all parameters at once
  - Create individual setters for each parameter
- Check and implement al TODOs in the source code

## Refine the documentation
- Add more coments to the source code
