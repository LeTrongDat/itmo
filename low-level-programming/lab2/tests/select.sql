SELECT id, name, age FROM students JOIN courses ON id = courses.student_id WHERE id > 10/2 AND age < 18 OR name = alex;