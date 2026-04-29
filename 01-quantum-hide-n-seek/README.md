# Quantum Hide And Seek

A quick tool for a quantum hide and seek rule.

## Game Rule

* The game is set on a MxN rectangular board. 
* A hider can be in any square, but their location is quantum, with a probability to be on every square.
* At the beginning of each turn, seeker can observe a number of squares. 
  * If total probability of all squares sum up to 100%, hider is revealed in one of the observed squares.
  * Otherwise, hider is not in the square, and all observed squares' probabilities are collapsed to 0%.
    * If there are adjacent non-observed squares to an observed square, the previous probability of observed square is evenly spread out to adjacent non-observed squares.
    * If there are no adjacent non-observed squares, the previous probability of observed square is evenly spread out to all non-observed squares.
* At the end of each turn, hider's probability diffuses:
  * If a square is adjacent to exit squares, probability of that square is evenly spread to the exit squares.
  * Otherwise, in each square is spread to adjacent non-observed squares.

## GUI

* A GM panel that allows showing various states and whether hider probability can be directly modified.
* A view panel to show the current state.
* A time panel to control the timeline.