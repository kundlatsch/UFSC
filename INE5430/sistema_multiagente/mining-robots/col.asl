// Beliefs

pos(boss,15,15).
checking_cells.
resource_needed(1).


// Plans

+my_pos(X,Y)
   :  checking_cells & not building_finished
   <- !check_for_resources.
 

+!check_for_resources
   :  resource_needed(R) & found(R) & my_pos(X,Y)
   <- !stop_checking;
   	  .broadcast(tell,resource(X,Y,R));
      !take(R,boss);
      !continue_mine.

+!check_for_resources
   :  resource_needed(R) & found(S) & my_pos(X,Y)
   <- .broadcast(tell,resource(X,Y,S));
      move_to(next_cell).
	  
+resource(X,Y,R)
   : not my_pos(X,Y)
   <- !go(X,Y).

+resource_needed(R) 
   : resource(X,Y,R)
   <- !go(X,Y).

+!go(X,Y) : true
   <- move_towards(X,Y).

+!check_for_resources
   :  resource_needed(R) & not found(R) & my_pos(X,Y)
   <- move_to(next_cell);
   .broadcast(untell,resource(X,Y,R)).

+!stop_checking : true
   <- ?my_pos(X,Y);
      +pos(back,X,Y);
      -checking_cells.

+!take(R,B) : true
   <- mine(R);
      !go(B);
      drop(R).

+!continue_mine : true
   <- !go(back);
      -pos(back,X,Y);
      +checking_cells;
      !check_for_resources.

+!go(Position)
   :  pos(Position,X,Y) & my_pos(X,Y)
   <- true.

+!go(Position) : true
   <- ?pos(Position,X,Y);
      move_towards(X,Y);
      !go(Position).

@psf[atomic]
+!search_for(NewResource) : resource_needed(OldResource)
   <- +resource_needed(NewResource);
      -resource(_,_,OldResource);
      -resource_needed(OldResource).

@pbf[atomic]
+building_finished : true
   <- .drop_all_desires;
      !go(boss).

