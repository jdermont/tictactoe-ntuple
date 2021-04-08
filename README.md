# tictactoe-ntuple
N-Tuple Neural Network for tic tac toe

Train N-Tuple Neural network for tic tac toe. The network allows the 1-ply cpu become 'perfect' without exhaustive search. The N-Tuple consists all eight tic-tac-toe's row-of-3s, times 2 depending of whose side it is, so a total 432 (8\*54) weights (+ bias) in this network. Due to symmetries, it can be reduced to 270 (5\*54) weights.

> qmake .
> make
> ./tictactoe-ntuple

Firstly it generates all reachable positions and labels them accordingly (win = 1, draw = 0, lose = -1). Then N-Tuple network learns on those positions for 100 epochs and learning rate 0.01. Then various cpus are playing against each other. Example output:

```
generating data from games...
there are 5478 positions
training network...
epoch 5. error: 0.125646
epoch 10. error: 0.095766
epoch 15. error: 0.0840372
epoch 20. error: 0.078112
epoch 25. error: 0.0745426
epoch 30. error: 0.0720369
epoch 35. error: 0.0701132
epoch 40. error: 0.0687367
epoch 45. error: 0.0681127
epoch 50. error: 0.0668308
epoch 55. error: 0.0660624
epoch 60. error: 0.0656345
epoch 65. error: 0.0653342
epoch 70. error: 0.0645977
epoch 75. error: 0.064195
epoch 80. error: 0.0638117
epoch 85. error: 0.0636242
epoch 90. error: 0.0632325
epoch 95. error: 0.063001
epoch 100. error: 0.0627927
perfect cpu vs random: 
CPU1 CPU2 DRAW: 
95 0 5
178 0 22
266 0 34
355 0 45
442 0 58
531 0 69
617 0 83
701 0 99
785 0 115
875 0 125
network cpu vs random: 
CPU1 CPU2 DRAW: 
94 0 6
191 0 9
286 0 14
377 0 23
469 0 31
561 0 39
655 0 45
749 0 51
841 0 59
931 0 69
network cpu vs cpu ply 3: 
CPU1 CPU2 DRAW: 
22 0 78
52 0 148
83 0 217
113 0 287
149 0 351
175 0 425
198 0 502
230 0 570
259 0 641
282 0 718
network cpu vs perfect cpu: 
CPU1 CPU2 DRAW: 
0 0 100
0 0 200
0 0 300
0 0 400
0 0 500
0 0 600
0 0 700
0 0 800
0 0 900
0 0 1000
```

Code style is a little hackatonish, but hopefully easily understood.
