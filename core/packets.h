

/* OK - we have 5 players

Players send commands
ack 0,1,2,3,4 as bits
from all to all move fireball ...
1 From 1 to 4 move ,,,,,,,,, attack r
2 from 2 to 3 move ......... arrack p
3 


Sender: Once you receive ack from receiver, and 2 more, it is good.

Receiver: once you receive 2 acks from others, it is good.

others- once receiver 

possible things:
planets
robots
rock attack
paper attach
scissor attack
plain attack
rock defend
paper defend
scissor defend
plain defend



0 -> 1, 1 ->2, 2 -> 3, 4 -> 0, 
0 -> 2, 1->3, 2->4,  4->0,  5-> 1,
0 -> 3, 1->4, 2->0, 4->1, 

0 -> 1, 0 ->2

2 -> 3 (02)
1 -> 4 (about 0,1)
4 -> 0 (about 4,



0 -> 1    1 -> 2   2 -> 3  3 -> 4  4-> 0

0 -> 1(4) 1->


Generic from everyone to everyone
everyone has an array of 3 - they see rock, paper, scissor, but are something else.


from	to	attack		defend
0	0	0.75		0.75
0	rock	0.67		0.83
0	paper	0.75		0.83
0	scissors 0.75		0.83
rock	rock	0.5		0.5
rock	paper	0.25		1.0
rock	scissors 1.0		0.25
paper...
scissors ...
0 - meh but slightly less to rock, paper scissors
Rock - rock - less meh than 0
paper - rock 1.0 to 0.25  paper kicks ass
scissors - soa nd so

Each shape (4,6,8,12,20) is random.
Each color also is random
Each hater robot team is random for each screen

0(0),1(0) a rps 1 2 beat #startpos endpos (then half, quarter, eights, etc. ) time to hit.
0 - just to one - voice - no - voice allows collab, and cheating, end voice maybe.

if go there, attack

planet 0 to planet 1 - onset beam  rps color?
planet can attack, defend, or infrastructure. 3 different rates - all interdependent . x^2+y^2_z^2 = 1.
If raise x, decrease y and z, porp.
If x and y, decrease z porp
imagine rock, paper, scissors that way:

So slower latency gets less , longer gets more.

a - move/deal robot to planet robots speed versus robot kinkiness makes power taken.
speed - time to get from a to b.
kinkiness - effective distance from a to b.
b - intercept robot to robot
c - deal planet to planet lazer arrow.
planet - Attack, defend, grow

e - blow up robot

*/

/* start - Planet has ADG,*/

typedef struct hater_packet_coordinate {
short xyz[3] } hater_packet_coordinate; /* start off with 3 bytes +32768 to -32767 . Later compress */

typedef struct hater_attack_defend_energy {
float attack;
float defend;
float energy;
/* attack defend energy is sometimes a ratio for adding ade, and sometimes the actual value for ade. 
If energy gets low, attach and defend is reduced by porportion to keep it above 0 or some constant 
Attack is attack strength. Energy stays the same? 
defend is defend strength. Energy stays the same? 
attack ratio + defend ratio + energy ratio squared sqrt = 1.
energy with movement though. movement is speed / complexity.
Complexity = sum of distances of all x,y z - which is weird because x,y could be bigger or smaller if diagonal or not
yes, will have to do diagonals and deltas - delta x delta y might be diagonal in other realm, and diagonals might be delta x,delta y.
I guess delta x and delta y are /2.  but 
We do distance of diagonals + distance of x or y * 2
*/

/* do the packet with a start and end point, and a midpoint
then add other points 
*/

typedef hater_packet_piece_hater_move_to_planet_all {
char command;
char player_number_and_robot_number; /* owning_player_number*5+robot_number */

hater_packet_coordinate start_coordinate;
hater_packet_coordinate end_coordinate;
int /* time  time to get there - used to figure out energy taken */
}
