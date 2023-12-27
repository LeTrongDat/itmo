-- Выведите таблицу со средним возрастом студентов во всех группах (Группа, Средний возраст), где средний возраст больше минимального возраста в группе 3100.

WITH JoinedData AS (
    SELECT 
        Н_УЧЕНИКИ.ГРУППА, 
        AGE(Н_ЛЮДИ.ДАТА_РОЖДЕНИЯ) AS Age
    FROM
        Н_ЛЮДИ
        INNER JOIN Н_УЧЕНИКИ ON Н_ЛЮДИ.ИД = Н_УЧЕНИКИ.ЧЛВК_ИД
), 
MinAgeForGroup AS (
    SELECT 
        MIN(Age) AS MinAge
    FROM 
        JoinedData
    WHERE 
        ГРУППА = '3100'
)
SELECT 
    ГРУППА, 
    AVG(Age)
FROM 
    JoinedData
GROUP BY 
    ГРУППА
HAVING 
    AVG(Age) > (SELECT MinAge FROM MinAgeForGroup);
