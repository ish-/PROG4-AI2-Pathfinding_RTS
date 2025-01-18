# PROG4-AI2-Pathfinding_RTS

This repository containes Pathfinding #2 exercise for evaluation.

## Description

Every boid has avoidance, alignment and separation influence (AI1). It's possible to give an order for multiple groups of boids.

For pathfinding I've implemented **A\* algorithm that reuses already calculated by neighbors pathes for a current cell**. This makes it possible to calculate the path for large groups without significant loss of performance (60fps for 500boids). If a streight-forward way isn't busy by an obstacle boid won't calculate any path using A*.

![Screen: Order for group](https://github.com/ish-/AI2-pathfinding-rts/blob/main/doc/imgs/screen-order_for_group.jpg?raw=true)

## Controls

- LBM - selection
- RBM - move order
- D - debug

![Screen: Order for all](https://github.com/ish-/AI2-pathfinding-rts/blob/main/doc/imgs/screen-order_for_all.jpg?raw=true)