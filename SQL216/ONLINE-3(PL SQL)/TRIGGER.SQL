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