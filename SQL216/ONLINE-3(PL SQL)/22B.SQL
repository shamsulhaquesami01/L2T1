CREATE TABLE employees_copy AS
SELECT * FROM employees;
COMMIT;
DESCRIBE EMPLOYEES;

CREATE OR REPLACE FUNCTION exchange_employee(m1 in number, m2 in number)
RETURN VARCHAR2 IS
eid1 number;
eid2 number;
BEGIN
    select e.employee_id into eid1 from EMPLOYEES e
    where e.MANAGER_ID=m1 and e.salary < all(
            select e2.salary from EMPLOYEES e2
            where e2.MANAGER_ID=m1 and e2.EMPLOYEE_ID <> e.EMPLOYEE_ID);
    select e.employee_id into eid2 from EMPLOYEES e
    where e.MANAGER_ID=m2 and e.salary < all(
            select e2.salary from EMPLOYEES e2
            where e2.MANAGER_ID=m2 and e2.EMPLOYEE_ID <> e.EMPLOYEE_ID);
DBMS_OUTPUT.PUT_LINE(eid1||'  '||eid2);
return 'success';
end;
/

DECLARE
    m1 number := 100;
    m2 number := 145;
    result_msg VARCHAR2(100); -- Variable to hold the function return
BEGIN
    result_msg := exchange_employee(m1, m2);
    DBMS_OUTPUT.PUT_LINE('Function status: ' || result_msg);
END;
/



select * from EMPLOYEES;
select * from LOCATIONS;
select * from DEPARTMENTS;


create or REPLACE PROCEDURE location_salary_report IS

BEGIN
    for R in(
 select city,empct,avgsal,jname,
 rank() over (order by empct asc, avgsal desc) as rnk
    from(
        select l.city,
        (select count(*) from employees e join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
        where d.LOCATION_ID=l.LOCATION_ID) as empct,
        ROUND((select avg(e.SALARY) from employees e join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
        where d.LOCATION_ID=l.LOCATION_ID),2) as avgsal,
        (select j.job_title from jobs j 
        where j.JOB_ID = (
            select e.JOB_ID from employees e join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
        where d.LOCATION_ID=l.LOCATION_ID AND e.salary > all (
            select e2.salary from employees e2 join DEPARTMENTS d2 on d2.DEPARTMENT_ID=e2.DEPARTMENT_ID
        where d2.LOCATION_ID=l.LOCATION_ID AND e.employee_id <> e2.employee_id
        ))) as jname
         from LOCATIONS l
         where (select count(*) from employees e join DEPARTMENTS d on d.DEPARTMENT_ID=e.DEPARTMENT_ID
        where d.LOCATION_ID=l.LOCATION_ID) >=1
    )
    )
    loop 
DBMS_OUTPUT.PUT_LINE('Rank: '||R.rnk||' City: '||R.city||' emps: '||R.empct|| '  avg: '||R.avgsal||
'highest job: '||R.jname);
    end loop;
end;
/

EXECUTE LOCATION_SALARY_REPORT;



-- create or replace PROCEDURE LOCATION_SALARY_REPORT2 AS
-- RANK NUMBER ;
-- BEGIN
-- RANK:=1;
-- FOR VAR IN (
-- SELECT
-- L.CITY ,
-- ROUND(AVG(E.salary), 2) AS AVGSAL ,
-- COUNT(E.EMPLOYEE_ID) AS EMP_COUNT ,
-- (
-- SELECT j.job_title
-- FROM EMPLOYEES e2
-- JOIN JOBS j ON e2.job_id = j.job_id
-- JOIN DEPARTMENTS d2 ON e2.DEPARTMENT_ID = d2.
-- DEPARTMENT_ID
-- WHERE d2.LOCATION_ID = L.LOCATION_ID
-- AND e2.salary = (
-- SELECT MAX(e3.salary)
-- FROM EMPLOYEES e3
-- JOIN DEPARTMENTS d3 ON e3.DEPARTMENT_ID = d3.
-- DEPARTMENT_ID
-- WHERE d3.LOCATION_ID = L.LOCATION_ID
-- )
-- ) AS HIGHEST_PAID_JOB
-- FROM
-- EMPLOYEES E
-- JOIN DEPARTMENTS D ON E.DEPARTMENT_ID = D.
-- DEPARTMENT_ID
-- JOIN LOCATIONS L ON D.LOCATION_ID = L.LOCATION_ID
-- GROUP BY
-- L.LOCATION_ID , L.CITY
-- ORDER BY
-- EMP_COUNT ASC ,
-- AVGSAL DESC
-- )
-- LOOP

-- DBMS_OUTPUT.PUT_LINE('Rank: '||RANK||' City: '||VAR.CITY||' emps: '||VAR.emp_cOUNt|| 'avg: '||VAR.avgsal||
-- 'highest job: '||VAR.HIGHEST_PAID_JOB);
-- RANK:=RANK+1;
-- END LOOP;
-- END;
-- /
-- EXECUTE LOCATION_SALARY_REPORT2;