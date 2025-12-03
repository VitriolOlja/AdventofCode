okay... 


How are we supposed to do this. 


for example -> 

.??..??...?##. 1,1,3


We can conclude that the fifth ? has to be damaged. 

and then 


I feel like this is a recursive problem -> yet doing it recursive will be expensive. 

We could maybe for each damaged we have left -> we subtract the total damaged in the row from how many we want. 
and then we add a damaged tile for all those we have left  and somehow check if it is viable if it is not we return 0. 
if we reach a last damaged tile and add it and it is viable we return 1. else we return 0. 

and then += a sum for each recursive call. 

How to check viability? 

we loop over each and check that each contigous group of damaged tile is less than the specified requirement. 
if its not viable we break that loop -> and return 0. 
else for each unknown '?' we add a damaged spring and repeat. 


/*
We might add in such a way such that we have already visited a way to arrange the springs before -> so we will need to keep
track of those states in probably a hashset. 

since we only have three states Damaged, Operational and Unknown
. we need 2 bits / character to represent the seen. 
One way would be again to use a direct access table instead of a hashset. 
For example for a string of 32 characters then we would need 2^64 elements in that direct access table. 
7 * 2^64 wasted bits. 

if we instead could offset to get the correct bit. 
*/ 

maybe instead of a hashtable it would be better to use a trie here? It does add some lookup time over the best case constant time hashtable/direct access table. but might be more manageable in terms of memory. 

so for each loop we first just check if it is in the trie -> if it is not then we continue -> if it is then we return 0. 

I think trie is actually worse on second though -> since each child has so few actual children -> it adds unnecessary overhead. 


TLDR: 

send the string line to a function, with a list of the contigious regions. 

check if seen: //doing it via a trie. 
  if seen return 0. 

check if viable:
  if not viable return 0.

//The damaged tiles we have left is the sum of the contigous numbers, subtracted by how many there there are in the string. 
check if damaged_tiles_left_to_put_out == 0:
  //base condition. 
  return 1; //viable option

//else
add to seen trie. 

int sum = 0;
for each '?':
  sum += ( function recursively with a '#' nstead of the question mark )




In principle i feel like this should work. Not sure if it is optimal however.


running the file and finding the longest string gives a longest string = 20

so that means there are 3^20 = 3486784401 different strings. 
A seen bitset would take about 436 mb of data. Which is maybe possible to malloc. 
but would waste alot of memory. 
So this is probably a implementation of ahashset. 