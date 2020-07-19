SELECT people.name FROM people
JOIN directors ON people.id=directors.person_id
WHERE directors.movie_id IN
(SELECT movies.id FROM movies
JOIN ratings ON movies.id=ratings.movie_id
WHERE ratings.rating >= 9)
GROUP BY people.id;