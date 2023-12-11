% Knowledge Base for Video Games

% Facts with one argument
game(minecraft).
game(the_witcher_3).
game(legend_of_zelda).
game(super_mario).
game(doom).
game(halo).
game(final_fantasy).
game(overwatch).
game(elder_scrolls_skyrim).
game(dark_souls).
genre(action).
genre(adventure).
genre(rpg).
genre(shooter).
genre(platformer).
platform(pc).
platform(playstation).
platform(xbox).
platform(nintendo).
platform(mobile).

% Facts with two arguments
released_on(minecraft, pc).
released_on(minecraft, playstation).
released_on(minecraft, xbox).
released_on(the_witcher_3, pc).
released_on(the_witcher_3, playstation).
released_on(the_witcher_3, xbox).
released_on(legend_of_zelda, nintendo).
released_on(super_mario, nintendo).
released_on(doom, pc).
released_on(doom, xbox).
belongs_to_genre(minecraft, adventure).
belongs_to_genre(the_witcher_3, action).
belongs_to_genre(legend_of_zelda, adventure).
belongs_to_genre(super_mario, platformer).
belongs_to_genre(doom, shooter).
belongs_to_genre(halo, shooter).
belongs_to_genre(final_fantasy, rpg).
belongs_to_genre(overwatch, shooter).
belongs_to_genre(elder_scrolls_skyrim, rpg).
belongs_to_genre(dark_souls, action).

% Rules
popular_game(Game) :- game(Game), released_on(Game, pc), belongs_to_genre(Game, adventure).
available_on_multiple_platforms(Game) :- released_on(Game, Platform1), released_on(Game, Platform2), Platform1 \= Platform2.
is_rpg(Game) :- game(Game), belongs_to_genre(Game, rpg).
is_action_adventure(Game) :- game(Game), belongs_to_genre(Game, action), belongs_to_genre(Game, adventure).
classic_game(Game) :- game(Game), (belongs_to_genre(Game, platformer) ; belongs_to_genre(Game, adventure)).

genre_on_platform(Game, Genre, Platform) :- game(Game), belongs_to_genre(Game, Genre), released_on(Game, Platform).

released_on_both_platforms(Game, Platform1, Platform2) :- game(Game), released_on(Game, Platform1), released_on(Game, Platform2), Platform1 \= Platform2.



% Queries
% Simple query to find if a game is an RPG
% ?- is_rpg(the_witcher_3).

% Query using logical operators
% ?- game(Game), released_on(Game, pc), belongs_to_genre(Game, action).

% Query using variables
% ?- released_on(Game, playstation), belongs_to_genre(Game, Genre).

% Query that requires the application of rules
% ?- popular_game(Game).

% Commented documentation throughout the code explains each fact, predicate, and rule.
