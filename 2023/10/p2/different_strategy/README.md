Lets try a differnet strategy. 
where we zoom in on the map. 


then we will map our tiles to a bigger system of tiles. 

So after reading the map and saving only the loop for all tiles we zoom in so each tile becomes a grid of 3x3 tiles. 


so | maps to  .|.
              .|.
              .|.


7 maps to ...
          -7.
          .|.

F maps to ...
          .F-
          .|.

J maps to .|.
          -J.
          ...


L maps to .|.
          .L-
          ...

- maps to ...
          ---
          ...

. maps to ...
          ...
          ...


then from some corner piece that is not part of the loop we make a flood fill. 
We change the values of all those that wecome across from outside. 
Then the inside loops are those that are not affected. 

i first tried a hashmap but the implementation was too slow. 

So i switched it out for a bitset.


