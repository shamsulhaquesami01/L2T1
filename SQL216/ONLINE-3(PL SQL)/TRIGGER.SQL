CREATE TABLE STUDENTS(
STUDENT_NAME VARCHAR2(250),
CGPA NUMBER
) ;

CREATE OR REPLACE TRIGGER HELLO_WORLD
AFTER INSERT
ON STUDENTS
DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE('Hello World');
END ;
/

INSERT INTO STUDENTS VALUES ('Fahim Hasan', 3.71);
INSERT INTO STUDENTS VALUES ('Ahmed Nahiyan', 3.80);

select * from STUDENTS;



--This trigger will run before insert on STUDENTS table
CREATE OR REPLACE TRIGGER HELLO_WORLD2
BEFORE INSERT
ON STUDENTS
DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE('Hello World2');
END ;
/

--This trigger will run after an insert or a delete statement on STUDENTS table
CREATE OR REPLACE TRIGGER HELLO_WORLD3
AFTER INSERT OR DELETE
ON STUDENTS
DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE('Hello World3');
END ;
/

--The following trigger will run after an update statement on STUDENTS table.
--Added with that, this trigger will only run when update is performed on the
--CGPA column.
CREATE OR REPLACE TRIGGER HELLO_WORLD4
AFTER UPDATE
OF CGPA
ON STUDENTS
DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE('Hello World4');
END ;
/
--The following trigger will run after an update operation on the STUDENTS table.
--Added with that, the trigger will run once for each row. The previous rows will
--run once for the whole statement, where this trigger will run N times if N rows
--are affected by the SQL statement.
CREATE OR REPLACE TRIGGER HELLO_WORLD5
AFTER UPDATE
OF CGPA
ON STUDENTS
FOR EACH ROW
DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE('Hello World5');
END ;
/

INSERT INTO STUDENTS VALUES ('Shakib Ahmed', 3.63);
delete from STUDENTS WHERE cgpa < 3.65;


UPDATE STUDENTS SET CGPA = CGPA + 0.01 WHERE STUDENT_NAME LIKE '%Shakib%';
--This will run HELLO_WORLD4, but will not run HELLO_WORLD5!!! Why? Because
--HELLO_WORLD5 is declared with FOR EACH ROW clause. This means trigger should be
--run for each row affected. Since, the above statement does not update any row
--(as the previous DELETE operation already deleted that row from the table)
--it will not run HELLO_WORLD5!
UPDATE STUDENTS SET STUDENT_NAME = 'Fahim Ahmed'
WHERE STUDENT_NAME = 'Fahim Hasan' ;
--This will not run any trigger. Although HELLO_TRIGGER4 is declared to be run
--after an update operation, the trigger will not run because the update is done
--on the column STUDENT_NAME rather than CGPA
UPDATE STUDENTS SET CGPA = CGPA + 0.01 ;
--This will run both HELLO_WORLD4 and HELLO_WORLD5 trigger. However, HELLO_WORLD5
--will run twice! Why? Because two rows will be affected by the SQL statement!



CREATE OR REPLACE TRIGGER OLD_NEW_TEST
BEFORE INSERT OR UPDATE OR DELETE
ON STUDENTS
FOR EACH ROW
DECLARE
BEGIN
DBMS_OUTPUT.PUT_LINE(':OLD.CGPA = ' || :OLD.CGPA) ;
DBMS_OUTPUT.PUT_LINE(':NEW.CGPA = ' || :NEW.CGPA) ;
END ;
/
INSERT INTO STUDENTS VALUES ('SOUMIK SARKAR', 3.85);
UPDATE STUDENTS SET CGPA = CGPA + 0.02 ;
DELETE FROM STUDENTS WHERE CGPA < 3.90;


CREATE OR REPLACE TRIGGER CORRECT_STUDENT_NAME
BEFORE INSERT
ON STUDENTS
FOR EACH ROW
DECLARE
BEGIN
:NEW.STUDENT_NAME := INITCAP(:NEW.STUDENT_NAME) ;
END ;
/
--Issue the following SQL statements and then view the rows of STUDENTS table
INSERT INTO STUDENTS VALUES ('SHAkil ahMED', 3.80);
INSERT INTO STUDENTS VALUES ('masum billah', 3.60);

select * from STUDENTS;

DROP TRIGGER OLD_NEW_TEST;

-- In Oracle, there is a function TO_NUMBER that converts a VARCHAR2 value to a numeric
-- value. If the input to this function is not a valid number, then this function throws an exception.
-- This is a problem in a SQL query because the whole query would not produce any result if one
-- row generates an exception. So, your job is to write a PL/SQL function ISNUMBER that
-- receives an input VARCHAR2 value and checks whether the input can be converted to a valid
-- number. If the input can be converted to a valid number than ISNUMBER should return ‘YES’,
-- otherwise ISNUMBER should return ‘NO’.

-- b. Write a trigger HELLO_WORLD6 that will run after a deletion operation on the STUDENTS
-- table. The trigger should be a ROW LEVEL trigger.
-- c. Write down a PL/SQL trigger on STUDENTS table. The trigger will ensure that whenever a
-- new row is inserted in the STUDENTS table, the name of the student contains only alphabetic
-- characters. Name your trigger INVALID_NAME. If the name is valid, then insertion should
-- be allowed. However, if the name is invalid, then insertion should be denied. To deny insertion,
-- you can throw an exception from the trigger that would halt the insertion operation.
-- d. Write a trigger that will save a student records in a table named LOW_CGPA_STUDENTS
-- which contain only one column to store student’s names. The trigger will work before an update
-- operation or an insert operation. Whenever the update operation results in a CGPA value less
-- than 2.0, the trigger will be fired and the trigger will save the students name in the
-- LOW_CGPA_STUDENTS table. Similarly, when an insert operation inserts a new row with
-- CGPA less than 2.0, the corresponding row must be saved in the LOW_CGPA_STUDENTS
-- table.

create or REPLACE TRIGGER HELLO_WORLD6
after DELETE
on students
for EACH ROW
DECLARE
BEGIN
    DBMS_OUTPUT.PUT_LINE('delete trigger');
    end;
/

create or replace trigger INVALID_NAME 
before INSERT
on students
for each ROW
DECLARE
BEGIN
    if TRANSLATE(:new.STUDENT_NAME, ' abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ', ' ') IS NOT NULL
    then RAISE_APPLICATION_ERROR(-20101, 'name not rules wise');
    end if;
end;
/
INSERT INTO STUDENTS VALUES ('sami haque',3.98);
INSERT INTO STUDENTS VALUES ('sami 45647',3.98);

CREATE TABLE LOW_CGPA_STUDENTS(
   STUDENT_NAME VARCHAR2(250)
);

create or REPLACE trigger cgpa_checker
before insert or UPDATE
on students
for EACH row
DECLARE
BEGIN
    if(:new.cgpa < 2.0)
    then insert into LOW_CGPA_STUDENTS VALUES(:new.STUDENT_NAME);
    end if;
end;
/

INSERT into STUDENTS VALUES('samiyousuf', 4.6);
INSERT INTO STUDENTS VALUES ('samihaque',1.7);
SELECT * from LOW_CGPA_STUDENTS;
SELECT * from STUDENTS;
UPDATE STUDENTS set cgpa = 1.5 where STUDENTS.STUDENT_NAME ='Samiyousuf';



--------22B
