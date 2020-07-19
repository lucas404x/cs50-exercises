SELECT people.name FROM people
JOIN stars
ON people.id=stars.person_id
WHERE stars.movie_id IN
(
	SELECT movies.id FROM movies
	JOIN stars
	ON movies.id=stars.movie_id
	JOIN people
	ON stars.person_id=people.id
	WHERE people.name="Kevin Bacon" AND people.birth = 1958
) AND people.name <> "Kevin Bacon"
GROUP BY people.id;
