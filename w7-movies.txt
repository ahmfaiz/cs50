SELECT title FROM movies WHERE year = 2008;

SELECT birth FROM people WHERE name = 'Emma Stone';

SELECT title FROM movies WHERE year >= 2018 ORDER BY title;

SELECT COUNT(*) FROM ratings WHERE rating = 10.0;

SELECT title, year FROM movies WHERE title LIKE 'Harry Potter%' ORDER BY year;

SELECT AVG(rating) FROM ratings JOIN movies ON ratings.movie_id = movies.id WHERE movies.year = 2012;

SELECT title, rating FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE year = 2010 AND rating IS NOT NULL ORDER BY rating DESC, title ASC;

SELECT name FROM people JOIN stars ON people.id = stars.person_id JOIN movies ON stars.movie_id = movies.id WHERE movies.title = 'Toy Story';

SELECT DISTINCT(name) FROM people JOIN stars ON people.id = stars.person_id JOIN movies ON stars.movie_id = movies.id WHERE movies.year = 2004 ORDER BY people.birth;

SELECT DISTINCT(name) FROM people JOIN directors ON people.id = directors.person_id JOIN ratings ON directors.movie_id = ratings.movie_id WHERE rating >= 9.0;

SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id JOIN ratings ON movies.id = ratings.movie_id WHERE stars.person_id = (SELECT id FROM people WHERE name = 'Chadwick Boseman') ORDER BY rating DESC LIMIT 5;

SELECT title FROM movies WHERE id IN (SELECT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = 'Bradley Cooper') INTERSECT SELECT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = 'Jennifer Lawrence'));

SELECT DISTINCT(name) FROM people WHERE id IN (SELECT person_id FROM stars WHERE movie_id IN (SELECT movie_id FROM stars JOIN people ON stars.person_id = people.id WHERE name = 'Kevin Bacon' AND birth = 1958)) AND NOT name = 'Kevin Bacon' ;

