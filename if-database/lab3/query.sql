

Получить список студентов, отчисленных после первого сентября 2012 года с очной или заочной формы обучения (специальность: Программная инженерия). В результат включить:
номер группы;
номер, фамилию, имя и отчество студента;
номер пункта приказа;
Для реализации использовать подзапрос с EXISTS.
Вывести список людей, не являющихся или не являвшихся студентами СПбГУ ИТМО (данные, о которых отсутствуют в таблице Н_УЧЕНИКИ). В запросе нельзя использовать DISTINCT.

SELECT Н_ОЦЕНКИ.КОД, Н_ВЕДОМОСТИ.ДАТА 
FROM Н_ОЦЕНКИ 
    LEFT JOIN Н_ВЕДОМОСТИ ON Н_ОЦЕНКИ.КОД = Н_ВЕДОМОСТИ.ОЦЕНКА
WHERE Н_ОЦЕНКИ.ПРИМЕЧАНИЕ = "освобождение" AND Н_ВЕДОМОСТИ.ИД < 1426978;

SELECT Н_ЛЮДИ.ФАМИЛИЯ, Н_ВЕДОМОСТИ.ИД, Н_СЕССИЯ.ЧЛВК_ИД
FROM Н_ЛЮДИ
    INNER JOIN Н_ВЕДОМОСТИ ON Н_ЛЮДИ.ИД = Н_ВЕДОМОСТИ.ЧЛВК_ИД
    INNER JOIN Н_СЕССИЯ ON Н_ЛЮДИ.ИД = Н_СЕССИЯ.ЧЛВК_ИД
WHERE Н_ЛЮДИ.ИД < 163484 AND Н_ВЕДОМОСТИ.ИД > 1250972 AND Н_СЕССИЯ.ИД > 1975;

SELECT COUNT(*) 
FROM Н_УЧЕНИКИ
    LEFT JOIN Н_ЛЮДИ ON Н_УЧЕНИКИ.ЧЛВК_ИД = Н_ЛЮДИ.ИД
WHERE EXTRACT(YEAR FROM AGE(Н_ЛЮДИ.ДАТА_РОЖДЕНИЯ)) > 25; 

SELECT *, COUNT(*) FROM Н_ЛЮДИ

SELECT DISTINCT TABLE_NAME 
    FROM (
        SELECT DISTINCT TABLE_NAME FROM INFORMATION_SCHEMA.COLUMNS
            WHERE COLUMN_NAME = 'ЧЛВК_ИД'
    ) as a1 WHERE COLUMN_NAME = 'ОТД_ИД';

select distinct table_name from information_schema.columns where column_name in ('ОТД_ИД', 'ЧЛВК_ИД') group by table_name having count(table_name) = 2;

ВТ


select family, number_of_families from 
    (select Н_ЛЮДИ.ФАМИЛИЯ as family, COUNT(*) as number_of_families from Н_ЛЮДИ group by Н_ЛЮДИ.ФАМИЛИЯ) as a
    join 
    (select Н_ЛЮДИ.ФАМИЛИЯ 
    from Н_ЛЮДИ 
        left join Н_ВЕДОМОСТИ on Н_ЛЮДИ.ИД = Н_ВЕДОМОСТИ.ЧЛВК_ИД
        left join Н_ОТДЕЛЫ on Н_ВЕДОМОСТИ.ОТД_ИД = Н_ОТДЕЛЫ.ИД
    where Н_ОТДЕЛЫ.КОРОТКОЕ_ИМЯ = 'ВТ'
    group by Н_ЛЮДИ.ФАМИЛИЯ having count(*) < 10) as b 
    on a.family = b.ФАМИЛИЯ;

select distinct 

select Н_ЛЮДИ.ФАМИЛИЯ as family, count(*) as number_of_families 
from Н_ЛЮДИ 
where 
    Н_ЛЮДИ.ФАМИЛИЯ not in ( 
        select Н_ЛЮДИ.ФАМИЛИЯ from
        (select distinct Н_ВЕДОМОСТИ.ЧЛВК_ИД as person_id, Н_ВЕДОМОСТИ.ОТД_ИД as department_id from Н_ВЕДОМОСТИ) as a
        left join Н_ЛЮДИ on Н_ЛЮДИ.ИД = person_id
        group by Н_ЛЮДИ.ФАМИЛИЯ having count(*) >= 10)
group by Н_ЛЮДИ.ФАМИЛИЯ;










-- Helper query to view table containing ЧЛВК_ИД and ОТД_ИД
SELECT DISTINCT TABLE_NAME 
FROM 
    INFORMATION_SCHEMA.COLUMNS  
WHERE 
    COLUMN_NAME IN ('ОТД_ИД', 'ЧЛВК_ИД') 
GROUP BY TABLE_NAME 
HAVING COUNT(*) = 2;

SELECT Н_ЛЮДИ.ФАМИЛИЯ, COUNT(*)
FROM
    Н_ЛЮДИ
WHERE 
    Н_ЛЮДИ.ФАМИЛИЯ NOT IN (
        SELECT Н_ЛЮДИ.ФАМИЛИЯ
        FROM
            Н_ЛЮДИ
            INNER JOIN Н_УЧЕНИКИ ON Н_ЛЮДИ.ИД = Н_УЧЕНИКИ.ЧЛВК_ИД
            INNER JOIN Н_ПЛАНЫ ON Н_УЧЕНИКИ.ПЛАН_ИД = Н_ПЛАНЫ.ИД
            INNER JOIN Н_ОТДЕЛЫ ON Н_ПЛАНЫ.ОТД_ИД = Н_ОТДЕЛЫ.ИД
        WHERE 
            Н_ОТДЕЛЫ.ИМЯ_В_ИМИН_ПАДЕЖЕ = 'кафедра вычислительной техники'
        GROUP BY Н_ЛЮДИ.ФАМИЛИЯ
        HAVING COUNT(*) >= 10
    )
GROUP BY Н_ЛЮДИ.ФАМИЛИЯ;

-- Выведите таблицу со средним возрастом студентов во всех группах (Группа, Средний возраст), где средний возраст больше минимального возраста в группе 3100.


-- SELECT Н_УЧЕНИКИ.ГРУППА, AVG(AGE(Н_ЛЮДИ.ДАТА_РОЖДЕНИЯ)) as AVERAGE_AGE
-- FROM 
--     Н_ЛЮДИ
--     INNER JOIN Н_УЧЕНИКИ ON Н_ЛЮДИ.ИД = Н_УЧЕНИКИ.ЧЛВК_ИД
-- GROUP BY Н_УЧЕНИКИ.ГРУППА
-- HAVING AVG(AGE(Н_ЛЮДИ.ДАТА_РОЖДЕНИЯ)) > (
--     SELECT MIN(AGE(Н_ЛЮДИ.ДАТА_РОЖДЕНИЯ))
--     FROM 
--         Н_ЛЮДИ
--         INNER JOIN Н_УЧЕНИКИ ON Н_ЛЮДИ.ИД = Н_УЧЕНИКИ.ЧЛВК_ИД
--     WHERE 
--         Н_УЧЕНИКИ.ГРУППА = '3100'
-- );






-- SELECT ГРУППА, ЧЛВК_ИД, ФАМИЛИЯ, ИМЯ, ОТЧЕСТВО, Н_УЧЕНИКИ.ИД, Н_УЧЕНИКИ.КОНЕЦ_ПО_ПРИКАЗУ
-- FROM
--     Н_УЧЕНИКИ
--     INNER JOIN Н_ЛЮДИ ON Н_ЛЮДИ.ИД = Н_УЧЕНИКИ.ЧЛВК_ИД
--     INNER JOIN Н_ПЛАНЫ ON Н_УЧЕНИКИ.ПЛАН_ИД = Н_ПЛАНЫ.ПЛАН_ИД
--     INNER JOIN Н_НАПРАВЛЕНИЯ_СПЕЦИАЛ ON Н_НАПРАВЛЕНИЯ_СПЕЦИАЛ.ИД = Н_ПЛАНЫ.НАПС_ИД
--     INNER JOIN Н_НАПР_СПЕЦ ON Н_НАПР_СПЕЦ.ИД = Н_НАПРАВЛЕНИЯ_СПЕЦИАЛ.НС_ИД
--     INNER JOIN Н_ФОРМЫ_ОБУЧЕНИЯ ON Н_ПЛАНЫ.ФО_ИД = Н_ФОРМЫ_ОБУЧЕНИЯ.ИД
-- WHERE 
--     Н_УЧЕНИКИ.КОНЕЦ_ПО_ПРИКАЗУ = '2012-08-31 00:00:00.00000'
--     AND Н_ФОРМЫ_ОБУЧЕНИЯ.НАИМЕНОВАНИЕ IN ('Заочная', 'Очная')
--     AND Н_НАПР_СПЕЦ.НАИМЕНОВАНИЕ = 'Программная инженерия';



select * from Н_ЛЮДИ where Н_ЛЮДИ.ИД not in (select ЧЛВК_ИД from Н_УЧЕНИКИ);



