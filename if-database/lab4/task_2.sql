-- Tables: Н_ЛЮДИ, Н_ВЕДОМОСТИ, Н_СЕССИЯ.
-- Output attributes: Н_ЛЮДИ.ФАМИЛИЯ, Н_ВЕДОМОСТИ.ЧЛВК_ИД, Н_СЕССИЯ.УЧГОД.
-- Filters (AND):
-- a) Н_ЛЮДИ.ИМЯ > Николай.
-- b) Н_ВЕДОМОСТИ.ИД > 1250981.
-- c) Н_СЕССИЯ.ИД > 32199.
-- Type of join: INNER JOIN.

SELECT Н_ЛЮДИ.ФАМИЛИЯ, Н_ВЕДОМОСТИ.ЧЛВК_ИД, Н_СЕССИЯ.УЧГОД
FROM
    Н_ЛЮДИ 
    INNER JOIN Н_ВЕДОМОСТИ ON Н_ВЕДОМОСТИ.ЧЛВК_ИД = Н_ЛЮДИ.ИД
    INNER JOIN Н_СЕССИЯ ON Н_СЕССИЯ.ЧЛВК_ИД = Н_ЛЮДИ.ИД 
WHERE 
    Н_ЛЮДИ.ИМЯ > 'Николай'
    AND Н_ВЕДОМОСТИ.ИД > 1250981
    AND Н_СЕССИЯ.ИД > 32199;