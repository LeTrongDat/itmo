likes(mary, apple).
likes(mary, grapes).
likes(tom, mango).
likes(tom, apple).

fruit(apple).
fruit(mango).
fruit(grapes).

who_likes_what(X, Y) :- likes(X, Y), fruit(Y).
