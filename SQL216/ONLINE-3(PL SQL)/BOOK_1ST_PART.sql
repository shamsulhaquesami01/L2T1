create or REPLACE PROCEDURE sami is 
msg VARCHAR(50);
begin
msg :='fdhgsdhgfhsg'; 
DBMS_OUTPUT.PUT_LINE(msg);
END;
/

DECLARE
ENAME VARCHAR2(100) ;
BEGIN
SELECT (FIRST_NAME || LAST_NAME) INTO ENAME
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 100 ;
DBMS_OUTPUT.PUT_LINE('The name is : ' || ENAME) ;
END ;
/


DECLARE
JDATE DATE ;
MONTHS NUMBER ;
RMONTHS NUMBER ;
BEGIN
SELECT HIRE_DATE INTO JDATE
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 100 ;
MONTHS := MONTHS_BETWEEN(SYSDATE, JDATE) ;
RMONTHS := ROUND(MONTHS, 0) ;
DBMS_OUTPUT.PUT_LINE('The employee worked ' || RMONTHS || ' months.') ;
END ;
/

SHOW ERRORS ;



DECLARE
JDATE DATE ;
YEARS NUMBER ;
BEGIN
SELECT HIRE_DATE INTO JDATE
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 102 ;
YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE) / 12) ;
IF YEARS >= 100 THEN
DBMS_OUTPUT.PUT_LINE('The employee worked 10 years or more') ;
ELSE
DBMS_OUTPUT.PUT_LINE('The employee worked less than 10 years') ;
END IF ;
END ;
/

DECLARE
ESALARY NUMBER ;
BEGIN
SELECT SALARY INTO ESALARY
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 100 ;
IF ESALARY < 1000 THEN
DBMS_OUTPUT.PUT_LINE('Job grade is D') ;
ELSIF ESALARY >= 1000 AND ESALARY < 2000 THEN
DBMS_OUTPUT.PUT_LINE('Job grade is C') ;
ELSIF ESALARY >= 2000 AND ESALARY < 3000 THEN
DBMS_OUTPUT.PUT_LINE('Job grade is B') ;
ELSIF ESALARY >= 3000 AND ESALARY < 5000 THEN
DBMS_OUTPUT.PUT_LINE('Job grade is A') ;
ELSE
DBMS_OUTPUT.PUT_LINE('Job grade is A+') ;
END IF ;
END ;
/

DECLARE
JDATE DATE ;
YEARS NUMBER ;
BEGIN
--first retrieve hire_date and store the value into JDATE variable
SELECT HIRE_DATE INTO JDATE
FROM EMPLOYEES
WHERE EMPLOYEE_ID = 100 ;
--calculate years from the hire_date field
YEARS := (MONTHS_BETWEEN(SYSDATE, JDATE) / 12) ;
IF YEARS >= 10 THEN
DBMS_OUTPUT.PUT_LINE('The employee worked 10 years or more') ;
ELSE
DBMS_OUTPUT.PUT_LINE('The employee worked less than 10 years') ;
END IF ;
EXCEPTION
--handle exceptions one by one here
WHEN NO_DATA_FOUND THEN
DBMS_OUTPUT.PUT_LINE('Employee is not present in database.') ;
WHEN OTHERS THEN
DBMS_OUTPUT.PUT_LINE('I dont know what happened!') ;
END ;
/


SET SERVEROUTPUT ON;

DECLARE
    -- Define variables for the data to be inserted
    v_counterry_id   counterries.counterry_id%TYPE := 'BD'; 
    v_counterry_name counterries.counterry_name%TYPE := 'Bangladesh';
    v_region_id    counterries.region_id%TYPE := 3; -- 3 represents Asia
BEGIN
    DBMS_OUTPUT.PUT_LINE('Attempting to insert: ' || v_counterry_id);

    INSERT INTO counterries (counterry_id, counterry_name, region_id)
    VALUES (v_counterry_id, v_counterry_name, v_region_id);

    DBMS_OUTPUT.PUT_LINE('Insert successful.');
    
    -- In a real scenario, you might COMMIT here
    -- COMMIT;

EXCEPTION
    WHEN DUP_VAL_ON_INDEX THEN
        DBMS_OUTPUT.PUT_LINE('Error: Duplicate counterry ID detected (' || v_counterry_id || ').');
        DBMS_OUTPUT.PUT_LINE('Logic: The Primary Key constraint prevents duplicate IDs.');

    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('An unexpected error occurred: ' || SQLERRM);
        -- SQLERRM provides the error message associated with the error code
END;
/



BEGIN
FOR i in 1..100
LOOP
DBMS_OUTPUT.PUT_LINE(i);
END LOOP ;
End ;
/

DECLARE
i number;
BEGIN
i := 1;
WHILE i<=100
LOOP
DBMS_OUTPUT.PUT_LINE(i);
i := i + 1;
END LOOP ;
End ;
/


DECLARE
i number;
BEGIN
--this is an unconditional loop, must have EXIT WHEN inside loop
i := 1;
LOOP
DBMS_OUTPUT.PUT_LINE(i);
i := i + 1;
EXIT WHEN (i > 100) ;
END LOOP ;
End ;
/


DECLARE
YEARS NUMBER ;
counterER NUMBER ;
BEGIN
counterER := 0 ;
--the following for loop will iterate over all rows of the SELECT results
FOR R IN (SELECT HIRE_DATE FROM EMPLOYEES )
LOOP
--variable R is used to retrieve columns
YEARS := (MONTHS_BETWEEN(SYSDATE, R.HIRE_DATE) / 12) ;
IF YEARS >= 10 THEN
counterER := counterER + 1 ;
END IF ;
END LOOP ;
DBMS_OUTPUT.PUT_LINE('Number of employees worked 10 years or more: ' ||
counterER) ;
END ;
/



DECLARE
YEARS NUMBER ;
counterER NUMBER ;
OLD_SAL NUMBER;
NEW_SAL NUMBER;
BEGIN
    counterER := 0 ;
FOR R IN (SELECT EMPLOYEE_ID, SALARY, HIRE_DATE FROM EMPLOYEES )
LOOP
OLD_SAL := R.SALARY ;
YEARS := (MONTHS_BETWEEN(SYSDATE, R.HIRE_DATE) / 12) ;
IF YEARS >= 10 THEN
UPDATE EMPLOYEES SET SALARY = SALARY * 1.15
WHERE EMPLOYEE_ID = R.EMPLOYEE_ID ;
END IF ;
SELECT SALARY INTO NEW_SAL FROM EMPLOYEES
WHERE EMPLOYEE_ID = R.EMPLOYEE_ID ;
DBMS_OUTPUT.PUT_LINE('Employee id:' || R.EMPLOYEE_ID || ' Salary: '
|| OLD_SAL || ' -> ' || NEW_SAL) ;
END LOOP ;
COMMIT;
END ;
/


-- Write a PL/SQL block that will print ‘Happy Anniversary X’ for each employee X whose
-- hiring date is today. Use cursor FOR loop for the task.

DECLARE
counter number;
begin
counter :=0;
for R in (select hire_date,EMPLOYEE_ID from EMPLOYEES where to_char(hire_date,'MMDD')=to_char(sysdate,'MMDD'))
loop
 DBMS_OUTPUT.PUT_LINE('Happy Anniversary '||R.EMPLOYEE_ID);
counter := counter +1;
end loop;

DBMS_OUTPUT.put_line(counter);
end;
/