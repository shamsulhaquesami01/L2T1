drop table employees_copy;
CREATE TABLE employees_copy AS
SELECT * FROM employees_copy;
COMMIT;
DESCRIBE EMPLOYEES;

CREATE OR REPLACE FUNCTION exchange_employee(m1 in number, m2 in number)
RETURN VARCHAR2 IS
eid1 number;
eid2 number;
dept1 VARCHAR2(100);
dept2 VARCHAR2(100);
sal1 number;
sal2 number;
BEGIN
    select employee_id into eid1 from (
        select employee_id from employees_copy
        where manager_id=m1 
        order by salary asc
    )
    where ROWNUM=1;
 select employee_id into eid2 from (
        select employee_id from employees_copy
        where manager_id=m2 
        order by salary asc
    )
    where ROWNUM=1;
DBMS_OUTPUT.PUT_LINE(eid1||'  '||eid2);
update employees_copy set manager_id=m2 where employee_id=eid1;
update employees_copy set manager_id=m1 where employee_id=eid2;
select DEPARTMENT_ID into dept1 from employees_copy where employee_id=eid1;
select DEPARTMENT_ID into dept2 from employees_copy where employee_id=eid2;
update employees_copy set DEPARTMENT_ID=dept2 where employee_id=eid1;
update employees_copy set DEPARTMENT_ID=dept1 where employee_id=eid2;
select salary into sal1 from employees_copy where employee_id=eid1;
select salary into sal2 from employees_copy where employee_id=eid2;
update employees_copy set salary=sal1+ABS(sal1-sal2)*0.5 where employee_id =eid1;
update employees_copy set salary=sal2+ABS(sal1-sal2)*0.5 where employee_id =eid2;
return 'success';
EXCEPTION
    WHEN NO_DATA_FOUND THEN RETURN 'One or both managers have no employees_copy.';
    WHEN OTHERS THEN RETURN 'Error occurred: ' || SQLERRM;
end;
/

DECLARE

    result_msg VARCHAR2(1000); 
BEGIN
    result_msg := exchange_employee(100,145);
    DBMS_OUTPUT.PUT_LINE('Function status: ' || result_msg);
END;
/


select * from EMPLOYEES;
select * from LOCATIONS;
select * from DEPARTMENTS;





CREATE OR REPLACE PROCEDURE LOCATION_SALARY_REPORT IS
    RANK_NUM NUMBER := 0;
BEGIN
    FOR R IN (
        SELECT 
            L.CITY,
            COUNT(E.EMPLOYEE_ID) AS EMP_COUNT,
            ROUND(AVG(E.SALARY), 2) AS AVG_SAL,
            MAX(J.JOB_TITLE) KEEP (DENSE_RANK LAST ORDER BY E.SALARY) AS TOP_JOB
        FROM LOCATIONS L
        JOIN DEPARTMENTS D ON L.LOCATION_ID = D.LOCATION_ID
        JOIN EMPLOYEES E ON D.DEPARTMENT_ID = E.DEPARTMENT_ID
        JOIN JOBS J ON E.JOB_ID = J.JOB_ID
        GROUP BY L.CITY
        ORDER BY EMP_COUNT ASC, AVG_SAL DESC
    )
    LOOP
        RANK_NUM := RANK_NUM + 1;
        DBMS_OUTPUT.PUT_LINE(
            'Rank: '                || RANK_NUM      ||
            ' | City: '            || R.CITY         ||
            ' | Employees: '       || R.EMP_COUNT    ||
            ' | Avg Salary: '      || R.AVG_SAL      ||
            ' | Highest Paying Job: ' || R.TOP_JOB
        );
    END LOOP;

EXCEPTION
    WHEN NO_DATA_FOUND THEN 
        DBMS_OUTPUT.PUT_LINE('No data found.');
    WHEN OTHERS THEN 
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;
/

-- Execute
BEGIN
    LOCATION_SALARY_REPORT;
END;
/