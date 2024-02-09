enum CORE_STATES {
   IDLE, // WAIT FOR "tirette"
   START, // go the right distance
   INITIAL_PICKUP, // go to pickup and pick
   GOTO_LINEFOLLOWER, // go back, and advance to line
   LINE_FOLLOWER_1, // wait for line following end
   DEPOSIT_1, // first deposit in green center 
   GOTO_PICKUP_LINE, // go back, turn around and align with pickup
   PICKUP_2, // advance to pickup and pick
   GOTO_LINEFOLLOWER_2, // go back to line line_follower
   LINE_FOLLOWER_2, // wait for line following end
   DEPOSIT_2, // second deposit in green center 
   INTERRUPT_TIMER // end the chronomter/timer
};