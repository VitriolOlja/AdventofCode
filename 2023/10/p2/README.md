hmm


How can we keep track of outside and inside? 

if we start from S, and move down -> we do not know yet if outside is to the left or to the right. 


can mark a different map with all the tiles of the loop and do a second search through? 
can we propagate some search along the "outside" border of the loop?
and keep track of both directions, paint one side of it 


Thinking of it for a bit -> I think we should save all corners and use them somehow. 
Something about finding all bounding boxes for the figures... 

nevermind. We should make a search for all pieces. 

we loop over the entire map, if we find a tile that is not a loop
we start a search -> for it be a enclosed area either a piece has to be connected to no part that is



to rule out if a piece is not enclosed -> we make a bfs search -> 
and if we find that any ground that it touches touches the border of the map -> we know that it is a outside piece and not enclosed.

we then stop the search -> and all of those are added to a seen hashmap or direct access table -> also if a search finds any of those pieces -> we know that it is not enclosed too -> and we also add them to the seen. 

We have an oppurtunity to eaither directly calculate the number of tiles within loops by summingthem up - 
or we can opt for calculating total-tiles minus loop-tiles minus outside-tiles.

the problem of course is the squeezing between tiles -> how should we handle this? 

I can do a fill instead from the border of the tiles. it should be enough to do from one side -> and then keep track of the state
either its an inside piece -> or it one of the searches on pieces beyond finds a map-border then it is an outside piece. 

we could quit the search if we find an outside piece and then search the other instead. or we search the entire outside piece? 


OR WE just do a floodfill from the border of the map? 
What the fuck why did i not think of that first ? 
Then we just do the subtraction above again to find inner pieces? again we need the special pieces that are half enclosed for this. 
We could use this method to find out the inside and outside though. 

we dont need a flood fill for that though. loop over all pieces from one side until we hit the first. now we know the outside is up. 
we now follow the loop from that position 

perhaps we loop over each piece. 

When we hit a piece we have not yet seen -> we do a bfs/dfs graph search -> while we have neighboars that are not
loop-pipes -> we count a sum of tiles -> if we hit a border we know this area is outside and we do not count it towards the sum -> otherwise we sum it up. 


we make a new char map same type as previous. mark all tiles of the largest loop -> so each new stack node has to have a ptr to a dynamic list -> so that we can backtrack when we find the largest loop. 




I am definitely overcomplicating things. 

Its easy when you dont really need to think about memory in other languages. having each node have its own array did not seem that terrible at first -> but it will be A LOT of copying memory. 

so its probably quite a terrible idea. 

I think we definitely dont want to do it this way. 

We want a predecessor map, So a map of nodes 



Okay though day 4 or something... 


I tihnk we want to consider them as lines of edges. 
so going back to the corner logic. 



we make a list of horizontal lines
and a list of vertical lines

the horizontal lines is as long as row_count
and if there is a horizontal line there then we add make that value 1. 

and similar for vertical lines. 



I think I have a different solution. 

That involves no such complexities. 

But with ray cast. 

The problem then is simply to decide what piece the S is. 

then we raycast to the right. 

we simply ignore all '-' as they are not lines. 

And count all '7' , '|' , 'J', 'L', 'F' basically any line tile that is not a '-'


