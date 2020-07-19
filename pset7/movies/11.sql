SELECT movies.title FROM movies
WHERE movies.id IN
(
	SELECT ratings.movie_id FROM ratings
	JOIN stars
	ON ratings.movie_id=stars.movie_id
	JOIN people
	ON stars.person_id=people.id
	WHERE people.name = "Chadwick Boseman"
	ORDER BY ratings.rating DESC
	LIMIT 5
);
