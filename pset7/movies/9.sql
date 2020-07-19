SELECT people.name FROM people
JOIN stars
ON people.id=stars.person_id
WHERE stars.movie_id IN (SELECT movies.id FROM movies WHERE movies.year = 2004)
GROUP BY people.id
ORDER BY people.birth;