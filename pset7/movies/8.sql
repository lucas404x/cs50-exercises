SELECT people.name from people
JOIN stars
ON people.id=stars.person_id
WHERE stars.movie_id IN (SELECT movies.id FROM movies WHERE movies.title LIKE "Toy Story%" LIMIT 1);