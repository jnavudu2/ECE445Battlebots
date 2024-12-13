# Jeevan's Work Log

# Table of Contents
- [Meeting 1 (9/7/24): Background Research](#m1)
- [Meeting 2 (9/14/24): Coming up with a design](#m2)
- [Meeting 3 (9/21/24): Starting with Design](#m3)
- [Meeting 4 (10/13/24): Iterating for Version 2](#m4)
- [Meeting 5 (10/27/24): Building Version 2](#m5)
- [Meeting 6 (11/9/24): Competing with Version 2](#m6)
- [Meeting 7 (11/17/24): Redesigning Version 3](#m7)
- [Meeting 8 (12/7/24): Competing with Version 3](#m8)





## Meeting 1 (9/7/24): Background Research 
As a team we have decided on pursuing the project that was pitched by Professor Gruev which was the antweight battlebot competition. It seemed like a lot of fun as 
we have all watched the battlebots show before and this would be like a smaller version of the real competition. I have also competed in a similar competion but I 
created a 3lb robot, so this would be very similar in some ways. There were a few major differences between the competition I competed in, and the comeptition that 
Professor Gruev is hosting.

1. The robots will have a maximum weight of 2lbs
2. All parts must be 3D printed out of PLA, PLA+, PETG, or ABS.
3. All PCBs must be custom and cant utilize any off the shelf ESCs
4. Must use a Microcontroller inside of the robot that will communicate with a computer via bluetooth or wifi
5. Maximum voltage is limited to 16V

After reading the rules, we realized that there are three main components for this project, the phsyical design of the robot, the ESCs that will power the motors, 
and the Microcontroller and communication with the robot. Given our strengths and previous experience, we split up the three parts where I will work on the robot design, 
Keegan will work on the software, and Avik will work on the PCB's, but we will all help out with other parts when possible and help is needed to meet deadlines.

With the roles assigned, we decided to split up and start working on our own parts.

## Meeting 2 (9/14/24): Coming up with a design
Being in charge of the mechanical design of the robot, I wanted to do something that would be cool and effective. From past experiences building larger robots, a vertical
spinner seemed to be the most effective style of weapon where they usually try to get under the other robot and throw them into the air. This causes them damage whey they
fall and hit the ground. The other optioins were a hammer bot, and a horizontal but these didnt seem viable due to to the printing and manufacturing constraints.

Our inspiration is the image below:

![Inspiration Image](AP1GczN6CwPzEUJMnWnTDR8tUuEhmt9LNcgfNrEuG51CfFV_M9TnVQ-5flBWqgw1796-h1347-s-no-gm.png)

This robot seemed to be a complete representation of all the characterisitics of the robot we wanted and a good baseline of what we wanted to achieve.

## Meeting 3 (9/21/24): Starting with Design
We started with a design and the cad is shown below:

![V1 Robot](v1robot.png)

After working on the robot, we realized that the robot would be a lot over the initial weight limit we were hoping to hit of 1lb, as that is the weight of most of the competions
that are hosted at other places. With this we realized that our compartments would have to be reduced drastically and the robot cant have a lot of solid parts.

## Meeting 4 (10/13/24): Iterating for Version 2

After spending some time re-evaluating the design, we realized that there were a lot of places where we can save weight and make parts hollow. After the complete design, it looks
like the cad below. This version had thinner chassis plates, the rear is hollowed out and gives a little compartment for the battery, and the robots weapon is a lot thinner and
has a larger inner diamater to make it lighter. In CAD the weight comes out to right under 1lb so hopefully after manufacturing it should be the same.

![V2](version2.png)

## Meeting 5(10/27/24): Building Version 2

We built version 2, and threw in some electroncis I had used before and it seemed to work alright, but hte tubing seems to be a little too thin and there is not enough squish from
from them to get good grip from the tangent drive motor.

![V2built](builtv2.JPG)

## Meeting 6 (11/9/24): Competing with Version 2

We competed with this version just to test it out mechanically and it performed really well and got second at the competition, although there were a few flaw that we realized.
The first is that when we turn, the robot was lifting up a lot, likely due to the gyroscopic forces. Also whenever we had to change the chassis, the entire robot had to be taken
apart so we want to make the robot more modular. With these changes I decided to scrap the current design and make the robot a lot wider.

## Meeting 7 (11/17/24): Redesigning Version 3

I wanted a robot that was a lot wider and not as deep. I made the wheels right next to each other and made the drive pods further out so that the weight further out can help keep the
robot down when turning. I came up with the robot below.

![cadv3](cadv3.png)

This robot was also a lot more modular and makes it easy to repair.

## Meeting 8 (12/7/24): Competing with Version 3

We were able to win the entire event with this new robot that utilized the PCBs we designed. This proved that the robot works mechanically and electonically. The previous issues were
fixed and the robot was a lot more stable during turns and repairs between fights were a lot easier.

![finalrobot](image001.jpg)




