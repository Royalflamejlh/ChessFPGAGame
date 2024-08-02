# FPGA Chess Game

This was my ECE 385 Final project in which I created a full chess game that could be ran completely on the DE-10 Lite FPGA.

Sadly, I had to turn in the FPGA and don't have any pictures of the game to show.

## Features

- Layered buffered rendering allowing for visually dragging pieces around.
- Checkmate and legal move detection.
- Ability to play against the computer or another player.
- Customizable board and sprite colors.

## Installation

The verilog can be compiled and installed using Quartus Prime 18.1.

The software to run is in the software directory and can be compiled to an elf using make or eclipse and then programmed onto the board.