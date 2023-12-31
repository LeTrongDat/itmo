CREATE TABLE students (id INT, name VARCHAR(255), age INT);

INSERT INTO students(id, name, age) VALUES (1, alex, 15);
INSERT INTO students(id, name, age) VALUES (2, misa, 16);
INSERT INTO students(id, name, age) VALUES (3, anton, 17);
SELECT id, name, age FROM students;

INSERT INTO students(id, name, age) VALUES (4, dale, 20);
INSERT INTO students(id, name, age) VALUES (5, kate, 30);
INSERT INTO students(id, name, age) VALUES (6, john, 25);

SELECT id, name, age FROM students;

SELECT id, name, age FROM students WHERE age > 18;
SELECT name, age FROM students WHERE name = misa;
SELECT name FROM students WHERE age > 16 AND age < 18;

UPDATE students SET age = 18 WHERE age > 20;
SELECT id, name, age FROM students;

UPDATE students SET name = sasha WHERE id != 3;
SELECT id, name, age FROM students;

UPDATE students SET name = john WHERE id = 4;

SELECT id, name, age FROM students;

DELETE FROM students WHERE id = 4;
SELECT id, name, age FROM students;

DELETE FROM students WHERE id != 3 AND age = 16 OR age = 17;
SELECT id, name, age FROM students;

exit