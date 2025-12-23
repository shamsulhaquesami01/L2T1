-- 1. FIND THOSE MANAGERS (ID) WHO MANAGES MORE THAN 5 EMPLOYEES.
-- 2. FIND THOSE EMPLOYEES WHO WERE HIRED IN THE MONTH OF APRIL IN ANY YEAR.
-- 3. FOR EACH JOB_TYPE, FIND THE NUMBER OF EMPLOYEES WITH AT LEAST 5000 SALARY.
-- 4. FIND THOSE EMPLOYEES WHOSE EMAIL CONTAINS "gmail".
-- 5. FIND THOSE EMPLOYEES WHOSE NAME CONTAINS STARTS WITH 'S' AND SALARY IS A MULTIPLE OF 2000.
-- 6. FOR EACH CHARACTER (A TO Z), FIND THE NUMBER OF EMPLOYEES WHOSE LAST NAME ENDS WITH THAT CHARACTER.

select manager_id, count(*) from employees
    group by manager_id
        having count(*)>5;

select * from EMPLOYEES
    where to_char(HIRE_DATE, 'MM')='04';

select job_id , count(*) from EMPLOYEES
        WHERE SALARY >= 5000
        group by JOB_ID;

select * from EMPLOYEES
        where email like '%gmail%';

select * from EMPLOYEES
        where lower(FIRST_NAME) like 's%' AND mod(SALARY,2000 )=0;

select substr(last_name,-1,1) as ending, count(*) from EMPLOYEES
        group by substr(last_name,-1,1)
        order by substr(last_name,-1,1) asc;
--yeye