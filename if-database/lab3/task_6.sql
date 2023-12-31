-- Получить список студентов, отчисленных после первого сентября 2012 года с очной или заочной формы обучения (специальность: Программная инженерия). В результат включить:
-- номер группы;
-- номер, фамилию, имя и отчество студента;
-- номер пункта приказа;
-- Для реализации использовать подзапрос с EXISTS.

SELECT 
    U.ГРУППА, 
    U.ЧЛВК_ИД, 
    L.ФАМИЛИЯ, 
    L.ИМЯ, 
    L.ОТЧЕСТВО, 
    U.ИД
FROM 
    Н_УЧЕНИКИ U
    INNER JOIN Н_ЛЮДИ L ON L.ИД = U.ЧЛВК_ИД
WHERE 
    U.КОНЕЦ_ПО_ПРИКАЗУ = '2012-08-31 00:00:00.00000'
    AND EXISTS (
        SELECT 1
        FROM Н_ПЛАНЫ P
        INNER JOIN Н_НАПРАВЛЕНИЯ_СПЕЦИАЛ NS ON NS.ИД = P.НАПС_ИД
        INNER JOIN Н_НАПР_СПЕЦ NNS ON NNS.ИД = NS.НС_ИД
        INNER JOIN Н_ФОРМЫ_ОБУЧЕНИЯ FO ON P.ФО_ИД = FO.ИД
        WHERE 
            P.ПЛАН_ИД = U.ПЛАН_ИД
            AND FO.НАИМЕНОВАНИЕ IN ('Заочная', 'Очная')
            AND NNS.НАИМЕНОВАНИЕ = 'Программная инженерия'
    );
