# Orbital Diagram3D

## Version 0.1

A orbital simulation using N-body Physics written in OpenGL. Currently, is hard coded for a diagram of a simple inner solar system, but can be currently be coded (With difficulty) to any Newtonian simulation you like. Gravitational force is currently calculated with a brute force sum, as well as a Verlet Integration to update objects position and velocity

### How to use

Simulation

- , and . control simulation speed from real time to 1s = 3 days
  Camera
- Tracking

  - If not Tracking, press Tab to enter Tracking Mode
  - Press tab to cycle through bodies
  - orientation can be controlled by holding right click and moving cursor
  - zoom controlled by mouse wheel

- Freecam
  - Press ~ to enter Freecam
  - w, s for forward and back
  - a, d for left and right
  - up and down arrow control speed of camera
  - right click + cursor to change direction of camera

### Next steps

- Implement basic collisions (Inelastic with destruction)
- Lighting System
- Asteroids
